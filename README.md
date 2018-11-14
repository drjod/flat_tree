# flat_tree
An AVL tree for fast binary search

It is faster than the Red-Black tree in std::map in the C++ standard library when searching and slightly slower when inserting (try e.g. the example in benchmark folder). Although differences are not huge, they matter in situations with many look-ups.

Usage is similar to std::map:

```
jod::flat_tree<std::string, long> cities;                                                             
cities.insert({"Berlin", 3711930});                                                                       
cities.insert({"London", 8825000});                                                                       
cities.insert({"Mumbai", 12442373});
```

Tree traversal:
```
for(auto city: cities)                                                                                    
    std::cout << city.first << ": "<< city.second << '\n';                                                
```

or with iterator
```
for(auto it = cities.begin(); it !=  cities.end(); ++it)                                                  
    std::cout << it->first << ": "<< it->second << '\n';                                                  
```

which is also used for searching
```
auto it = cities.find("London");                                                                          
    std::cout << it->second << '\n';
```
besides the access functions
```
std::cout << cities["Berlin"] << '\n';  // causes segmentation fault if Berlin does not exist as an element
std::cout << cities.at("Berlin") << '\n';  // throws exception if Berlin does not exist
```

Traverse backwards with the reverse iterator
```
for(auto rit = cities.rbegin(); rit !=  cities.rend(); ++rit)                                             
    std::cout << rit->first << ": "<< rit->second << '\n';                                                
```                                                                                                           
 
 or, as an alternative, insert the elements in opposite direction
```                                                                                                        
jod::flat_tree<std::string, long, std::greater<std::string> > cities;

```
Traversal is actually faster with the member function traverse
```
 cities.traverse([](jod::flat_tree<std::string, long>::node p)
     { std::cout << p.data().first << ": " << p.data().second <<  '\n'; })

```
Remove elements:
```
cities.erase("Karthago");
```
