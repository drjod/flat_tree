#include <iostream>
#include "flat_tree.h"
#include <random>
#include "timer.h"
#include <map>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <algorithm>


const int n = 1000000;


template<typename SeqCont, typename D, typename E>                                                                  
std::vector<int> fill_tree(SeqCont &cont, D dist, E engine)                                                           
{                                                                                                             
	std::vector<int> v;    
    int nl = n;                                                                                                      
    while(nl--)                                                                                                
    {
		int key = dist(engine);                                                                                      
        cont.insert({key, 1});
		if(nl%10 == 0)
			v.push_back(key);                                                                                  
    }                                                                                                         
    
	return v;                                                                       
                                                                                                              
}


template<typename AssocCont, typename D, typename E>                                                                  
void fill_sequence(AssocCont &cont, D dist, E engine)                                                           
{                                                                                                             
	std::vector<int> v;    
    int nl = n;                                                                                                      
                                                                                                          
    while(nl--)                                                                                                
    {
		int key = dist(engine);                                                                                      
        cont.push_back({key, 1});
		if(nl%10 == 0)
			v.push_back(key);                                                                                  
    }                                                                                                         
    
                                                                                                              
}

int main()
{
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine engine(seed);
	std::uniform_int_distribution<int> dist;

	Timer timer;
	std::vector<int> vf;

	jod::flat_tree<int, int> t;
	std::map<int, int> m;
	std::unordered_map<int, int> um;
	std::vector<std::pair<int, int> > v;

	std::stringstream state;
	state << engine;

	std::cout << "*********************************************\n";
	std::cout << "*********************************************\n";
	std::cout << "***** insert - tree\n";
	timer.start();
	vf = fill_tree(t, dist, engine);
	timer.stop();

	std::cout << "***********************************\n";
	state >> engine;
	std::cout << "***** insert - map\n";
	timer.start();
	vf = fill_tree(m, dist, engine);
	timer.stop();

	std::cout << "***********************************\n";
	state >> engine;
	std::cout << "***** insert - unordered map\n";
	timer.start();
	vf = fill_tree(um, dist, engine);
	timer.stop();

/*
	state >> engine;
	std::cout << "vector - insert \n";
	timer.start();
	fill_sequence(v, dist, engine);
	timer.stop();
*/

	std::default_random_engine engine2(seed);
	std::shuffle(vf.begin(), vf.end(), engine2);


	std::cout << "*********************************************\n";
	std::cout << "*********************************************\n";
	std::cout << "***** search - tree\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = t.find(i);
	}
	timer.stop();


	std::cout << "***********************************\n";
	std::cout << "***** search - map\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = m.find(i);
	}
	timer.stop();


	std::cout << "***********************************\n";
	std::cout << "***** search - unordered map\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = um.find(i);
	}
	timer.stop();

/*
	std::cout << "***********************************\n";
	std::cout << "***** search - vector\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = std::find_if(v.begin(), v.end(),
			[&i](std::pair<int, int> elem) { return elem.first == i; });
	}
	timer.stop();
*/

	std::cout << "*********************************************\n";
	std::cout << "*********************************************\n";
	std::cout << "***** traverse - tree iterator\n";
	timer.start();
	
	for(auto i: t) {}
	timer.stop();

	std::cout << "***********************************\n";
	std::cout << "***** traverse - tree\n";
	timer.start();
	
	t.traverse([](auto i) {});
	timer.stop();

	std::cout << "***********************************\n";
	std::cout << "***** traverse - map\n";
	timer.start();
	for(auto i: m) {}
	timer.stop();

	std::cout << "***********************************\n";
	std::cout << "***** traverse - unordered map\n";
	timer.start();
	for(auto i: um) {}
	timer.stop();

/*
	std::cout << "vector - traverse \n";
	timer.start();
	for(auto i: v) {}
	timer.stop();
*/

	std::default_random_engine engine3(seed);
	std::shuffle(vf.begin(), vf.end(), engine3);

	std::cout << "*********************************************\n";
	std::cout << "*********************************************\n";
	std::cout << "***** erase - tree\n";
	timer.start();
	for(auto i: vf)
	{
		t.erase(i);
	}
	timer.stop();

	std::cout << "***********************************\n";
	std::cout << "***** erase - map\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = m.erase(i);
	}
	timer.stop();


	std::cout << "***********************************\n";
	std::cout << "***** erase - unordered map\n";
	timer.start();
	for(auto i: vf)
	{
		auto value = um.erase(i);
	}
	timer.stop();

	return 0;
}
