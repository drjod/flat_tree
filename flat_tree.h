#ifndef JOD_FLAT_TREE_H
#define JOD_FLAT_TREE_H

#include <iostream>
#include <utility>
#include <functional>
#include <stdexcept>


namespace jod
{

using _size_type = int;
const _size_type _empty = 0;

template<typename Key, typename T, typename Compare> struct flat_tree_iterator;
template<typename Key, typename T, typename Compare> struct const_flat_tree_iterator;
template<typename Key, typename T, typename Compare> struct reverse_flat_tree_iterator;
template<typename Key, typename T, typename Compare> struct const_reverse_flat_tree_iterator;

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare=std::less<Key> >
struct flat_tree
{
	using key_type			= Key;
	using mapped_type		= T;
	using value_type		= std::pair<key_type, mapped_type>;
	using size_type			= _size_type;

	using iterator					= flat_tree_iterator<Key, T, Compare>;
	using const_iterator			= const_flat_tree_iterator<Key, T, Compare>;
	using reverse_iterator			= reverse_flat_tree_iterator<Key, T, Compare>;
	using const_reverse_iterator	= const_reverse_flat_tree_iterator<Key, T, Compare>;

	class node
	{
		node(const value_type& _ddata, node* _parent=nullptr, const size_type& _hheight=_empty) : 
			_data(_ddata), _height(_hheight), left(nullptr), right(nullptr), parent(_parent) {}
		// node(const key_type& _key, node* _parent=nullptr, const size_type& __height=_empty) : 
		//	_height(_hheight), left(nullptr), right(nullptr), parent(_parent) 
		// 		{_data.key = _key; }
		value_type _data;
		size_type _height;
		node* left;
		node* right;
		node* parent;
		
		const node* previous() const;
		const node* next() const;

		node* previous();
		node* next();
		// operator std::pair<key_type, value_type>() const { return _data; }

		friend struct flat_tree_iterator<Key, T, Compare>;
		friend struct const_flat_tree_iterator<Key, T, Compare>;
		friend struct reverse_flat_tree_iterator<Key, T, Compare>;
		friend struct const_reverse_flat_tree_iterator<Key, T, Compare>;
		friend struct flat_tree<Key, T, Compare>;
	public:
		value_type data() const { return _data; }
		value_type& data() { return _data; }
		size_type height() { return _height; }
	};

	flat_tree() : root(nullptr), _size(_empty) {}
	~flat_tree() { if(root != nullptr) clear(root); }
	// -----------------------------------------------------
	iterator begin();
	iterator end();
	const_iterator cbegin();
	const_iterator cend();

	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator crbegin();
	const_reverse_iterator crend();
	// -----------------------------------------------------
	void insert(const value_type& _data) { root = insert(root, nullptr, _data); ++_size; }
	bool erase(const key_type& key);
	void clear() { clear(root); _size = _empty; }

	iterator find(const key_type& key) const { return find(root, key); }
	mapped_type& operator[](const key_type& key); 
	// const mapped_type operator[](const key_type& key) const { return find(root, key)->second; }
	mapped_type& at(const key_type& key);
	const mapped_type at(const key_type& key) const; 
	template<typename F> void traverse(F f)	const { if(root != nullptr) traverse(root, f); }

	mapped_type first_value() const { return first_node(root).second; }
	mapped_type last_value() const { return last_node(root).second; }
	size_type _height() const { return _height(root); }
	size_type size() const { return _size; }
	bool empty() const { return _size == _empty; }
private:
	node* root;
	size_type _size;  // provisional - threads and exceptions can mess this up
	Compare compare;

	// balance flat_tree branch
	node* rotate_left(node* _node);
	node* rotate_right(node* _node);
	node* balance_flat_tree(node* _node);
	// modify flat_tree
	node* insert(node* _node, node* _parent, const value_type& _data);
	node* erase(node* _node, const key_type& key);
	void clear(node* _node);
	// traverse branch
	iterator find(node* _node, const key_type& key) const;
	node* first_node(node* _node) const;
	node* last_node(node* _node) const;
	template<typename F> void traverse(node* _node, F f) const ;
	size_type _height(node* _node) const { return (_node == nullptr) ? _empty-1 : _node->_height; }
};

// ------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
bool flat_tree<Key, T, Compare>::erase(const flat_tree::key_type& key) 
{ 
	if(erase(root, key) != nullptr) 
	{ 
		--_size; 
		return true; 
	}
	else
		return false; 
}


// balance flat_tree branch
template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::rotate_left(
											flat_tree<Key, T, Compare>::node* _node)
{
	// std::cout << "rotate left\n";
	node* root_new = _node->right;
	node* node_moved = root_new->left;

	if(root == _node)
		root = root_new;
	root_new->left = _node;
	_node->right = node_moved;

	root_new->parent = _node->parent;
	_node->parent = root_new;
	if(node_moved)
		node_moved->parent = _node;

	_node->_height = std::max(_height(_node->left), _height(_node->right)) + 1;
	root_new->_height = std::max(_height(root_new->left), _height(root_new->right)) + 1;
	return root_new;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::rotate_right(
											flat_tree<Key, T, Compare>::node* _node)
{
	// std::cout << "rotate right\n";
	node* root_new = _node->left;
	node* node_moved = root_new->right;

	if(root == _node)
		root = root_new;
	root_new->right = _node;
	_node->left = node_moved;

	root_new->parent = _node->parent;
	_node->parent = root_new;
	if(node_moved)
		node_moved->parent = _node;

	_node->_height = std::max(_height(_node->left), _height(_node->right)) + 1;
	root_new->_height = std::max(_height(root_new->left), _height(root_new->right)) + 1;
	return root_new;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::balance_flat_tree(
											flat_tree<Key, T, Compare>::node* _node)
{
	size_type balance = _height(_node->right) - _height(_node->left); 

	if(balance > 1)
	{
		if(_height(_node->right->right) >= _height(_node->right->left))
			return rotate_left(_node);
		else
		{
			_node->right = rotate_right(_node->right);
			return rotate_left(_node);
		}
	}

	if(balance < -1)
	{
		if(_height(_node->left->left) >= _height(_node->left->right))
			return rotate_right(_node);
		else
		{
			_node->left = rotate_left(_node->left);
			return rotate_right(_node);
		}
	}

	return _node;
}

// ------------------------------------------------------------------------------
// modify branch
template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::insert(
				flat_tree<Key, T, Compare>::node* _node, 
				flat_tree<Key, T, Compare>::node* _parent, 
				const value_type& _data)
{	
	if(_node == nullptr)
	{
		return new node(_data, _parent);	
	}
	if(compare(_data.first, _node->_data.first))
		_node->left = insert(_node->left, _node, _data);
	else if(compare(_node->_data.first, _data.first))
		_node->right = insert(_node->right, _node, _data);

	_node->_height = std::max(_height(_node->left), _height(_node->right)) + 1;
	_node = balance_flat_tree(_node);
	
	return _node;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::erase(
					flat_tree<Key, T, Compare>::node* _node, const key_type& key)
{

	if(_node == nullptr) return _node;

	if(compare(key, _node->_data.first)) // smaller
		_node->left = erase(_node->left, key);  // larger
	else if(compare(_node->_data.first, key))
		_node->right = erase(_node->right, key);
	else  // equal
	{
		if(_node->left == nullptr)
		{
			node* tmp = _node->right;
			if(root == _node)
				root = tmp;
			delete(_node);
			return tmp;
		}
		else if(_node->right == nullptr)
		{
			node* tmp = _node->left;
			if(root == _node)
				root = tmp;
			delete(_node);
			return tmp;
		}

		node* tmp = first_node(_node->right);
		_node->_data.first = tmp->_data.first;
		erase(_node->right, _node->_data.first);
	}

	_node->_height = std::max(_height(_node->left), _height(_node->right)) + 1;
	_node = balance_flat_tree(_node);

	return _node;
}


template<typename Key, typename T, typename Compare>
void flat_tree<Key, T, Compare>::clear(node* _node)
{
	if(_node == nullptr)
		return;
	clear(_node->left);
	clear(_node->right);
	delete _node;
}

// -----------------------------------------------------------
// traverse branch
template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::iterator flat_tree<Key, T, Compare>::find(
					flat_tree<Key, T, Compare>::node* _node, const key_type& key) const
{
	if(_node == nullptr || key == _node->_data.first)
		return iterator(_node);

	if(compare(key, _node->_data.first))
		return find(_node->left, key); 
	else
		return find(_node->right, key);
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::first_node(
					flat_tree<Key, T, Compare>::node* _node) const
{
	if(_node->left)
		return first_node(_node->left);
	return _node;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node* flat_tree<Key, T, Compare>::last_node(
					flat_tree<Key, T, Compare>::node* _node) const
{
	if(_node->right)
		return last_node(_node->right);
	return _node;
}


template<typename Key, typename T, typename Compare>
template<typename F>
void flat_tree<Key, T, Compare>::traverse(
					flat_tree<Key, T, Compare>::node* _node, F f) const
{
	if(_node == nullptr)
		return;
	traverse(_node->left, f);
	f(*_node);
	traverse(_node->right, f);
}


// -----------------------------------------------------------
// access
template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::mapped_type&
		flat_tree<Key, T, Compare>::operator[](const key_type& key) 
{ 
	iterator it = find(root, key); 
	if(it != this->end())
	{	
		return (*it).second; 
	}
	else
	{
		node* _node  = insert(root, nullptr, std::pair<key_type, mapped_type>{key, {}});
		//return _node->data().second;
		iterator it = find(root, key);  // slow, but works 
		return (*it).second; 
	}
	
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::mapped_type& 
		flat_tree<Key, T, Compare>::at(const key_type& key) 
{ 
	auto data = find(root, key); 
	if(data == nullptr) throw std::out_of_range("at() failed"); 
	return data->second; 
}

	
template<typename Key, typename T, typename Compare>
const typename flat_tree<Key, T, Compare>::mapped_type
		flat_tree<Key, T, Compare>::at(const key_type& key) const 
		
{ 
	auto data = find(root, key); 
	if(data == nullptr) 
		throw std::out_of_range("at() failed"); 
	return data->second; 
}

// ------------------------------------------------------------------------------------------
// node
// ------------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
const typename flat_tree<Key, T, Compare>::node*  flat_tree<Key, T, Compare>::node::previous() const
{
	const node* _node;
	if(left)
	{
		_node = left;
		while(_node->right)
			_node = _node->right;

		return _node;
	}
	_node = this;

	while(_node->parent)
	{
		if(_node->parent->right == _node)
			return _node->parent;
		_node = _node->parent;
	}
	return nullptr;
}


template<typename Key, typename T, typename Compare>
const typename flat_tree<Key, T, Compare>::node*  flat_tree<Key, T, Compare>::node::next() const
{
	const node* _node;
	if(right)
	{
		_node = right;
		while(_node->left)
			_node = _node->left;

		return _node;
	}
	_node = this;

	while(_node->parent)
	{
		if(_node->parent->left == _node)
			return _node->parent;
		_node = _node->parent;
	}
	return nullptr;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node*  flat_tree<Key, T, Compare>::node::previous()
{
	node* _node;
	if(left)
	{
		_node = left;
		while(_node->right)
			_node = _node->right;

		return _node;
	}
	_node = this;

	while(_node->parent)
	{
		if(_node->parent->right == _node)
			return _node->parent;
		_node = _node->parent;
	}
	return nullptr;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::node*  flat_tree<Key, T, Compare>::node::next()
{
	node* _node;
	if(right)
	{
		_node = right;
		while(_node->left)
			_node = _node->left;

		return _node;
	}
	_node = this;

	while(_node->parent)
	{
		if(_node->parent->left == _node)
			return _node->parent;
		_node = _node->parent;
	}
	return nullptr;
}


// ------------------------------------------------------------------------------------------
// iterator
// ------------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
struct flat_tree_iterator : public std::iterator<std::bidirectional_iterator_tag, Key>
{
	flat_tree_iterator(typename flat_tree<Key, T, Compare>::node* _nnode) : _node(_nnode) {}

	std::pair<Key, T>& operator*();
	std::pair<Key, T>* operator->();
	flat_tree_iterator& operator++();
	//flat_tree_iterator operator++(int);
	flat_tree_iterator& operator--();
	//flat_tree_iterator operator--(int);

	bool operator==(const flat_tree_iterator& other) const { return _node == other._node; }
	bool operator!=(const flat_tree_iterator& other) const { return !(*this == other); }
private:
	typename flat_tree<Key, T, Compare>::node* _node;
};

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
std::pair<Key, T>& flat_tree_iterator<Key, T, Compare>::operator*()
{
	return _node->_data;
}


template<typename Key, typename T, typename Compare>
std::pair<Key, T>* flat_tree_iterator<Key, T, Compare>::operator->()
{
	return &(_node->_data);
}


template<typename Key, typename T, typename Compare>
flat_tree_iterator<Key, T, Compare>& flat_tree_iterator<Key, T, Compare>::operator++()
{
	_node = _node->next();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
flat_tree_iterator<Key, T, Compare> flat_tree_iterator<Key, T, Compare>::operator++(int)
{
	flat_tree_iterator<Key, T, Compare> iter(_node->next());
	return iter;
}
*/


template<typename Key, typename T, typename Compare>
flat_tree_iterator<Key, T, Compare>& flat_tree_iterator<Key, T, Compare>::operator--()
{
	_node = _node->previous();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
flat_tree_iterator<Key, T, Compare> flat_tree_iterator<Key, T, Compare>::operator--(int)
{
	flat_tree_iterator<Key, T, Compare> iter(_node->previous());
	return iter;
}
*/

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::iterator flat_tree<Key, T, Compare>::begin()
{
	flat_tree_iterator<Key, T, Compare> iter(first_node(root));
	return iter;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::iterator flat_tree<Key, T, Compare>::end()
{
	flat_tree_iterator<Key, T, Compare> iter(nullptr);
	return iter;
}

// ------------------------------------------------------------------------------------------
// const_iterator
// ------------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
struct const_flat_tree_iterator : public std::iterator<std::bidirectional_iterator_tag, Key>
{
	const_flat_tree_iterator(typename flat_tree<Key, T, Compare>::node* _nnode) : _node(_nnode) {}

	const std::pair<Key, T>& operator*() const;
	const std::pair<Key, T>* operator->() const;
	const_flat_tree_iterator& operator++();
	//const_flat_tree_iterator operator++(int);
	const_flat_tree_iterator& operator--();
	//const_flat_tree_iterator operator--(int);

	bool operator==(const const_flat_tree_iterator& other) const { return _node == other._node; }
	bool operator!=(const const_flat_tree_iterator& other) const { return !(*this == other); }
private:
	const typename flat_tree<Key, T, Compare>::node* _node;
};

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
const std::pair<Key, T>& const_flat_tree_iterator<Key, T, Compare>::operator*() const
{
	return _node->_data;
}


template<typename Key, typename T, typename Compare>
const std::pair<Key, T>* const_flat_tree_iterator<Key, T, Compare>::operator->() const
{
	return &(_node->_data);
}


template<typename Key, typename T, typename Compare>
const_flat_tree_iterator<Key, T, Compare>& const_flat_tree_iterator<Key, T, Compare>::operator++()
{
	_node = _node->next();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
const_flat_tree_iterator<Key, T, Compare> const_flat_tree_iterator<Key, T, Compare>::operator++(int)
{
	const_flat_tree_iterator<Key, T, Compare> iter(_node->next());
	return iter;
}
*/


template<typename Key, typename T, typename Compare>
const_flat_tree_iterator<Key, T, Compare>& const_flat_tree_iterator<Key, T, Compare>::operator--()
{
	_node = _node->previous();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
const_flat_tree_iterator<Key, T, Compare> const_flat_tree_iterator<Key, T, Compare>::operator--(int)
{
	const_flat_tree_iterator<Key, T, Compare> iter(_node->previous());
	return iter;
}
*/

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::const_iterator flat_tree<Key, T, Compare>::cbegin()
{
	const_flat_tree_iterator<Key, T, Compare> iter(first_node(root));
	return iter;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::const_iterator flat_tree<Key, T, Compare>::cend()
{
	const_flat_tree_iterator<Key, T, Compare> iter(nullptr);
	return iter;
}

// ------------------------------------------------------------------------------------------
// reverse_iterator
// ------------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
struct reverse_flat_tree_iterator : public std::iterator<std::bidirectional_iterator_tag, Key>
{
	reverse_flat_tree_iterator(typename flat_tree<Key, T, Compare>::node* _nnode) : _node(_nnode) {}

	std::pair<Key, T>& operator*();
	std::pair<Key, T>* operator->();
	reverse_flat_tree_iterator& operator++();
	//reverse_flat_tree_iterator operator++(int);
	reverse_flat_tree_iterator& operator--();
	//reverse_flat_tree_iterator operator--(int);

	bool operator==(const reverse_flat_tree_iterator& other) const { return _node == other._node; }
	bool operator!=(const reverse_flat_tree_iterator& other) const { return !(*this == other); }
private:
	typename flat_tree<Key, T, Compare>::node* _node;
};

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
std::pair<Key, T>& reverse_flat_tree_iterator<Key, T, Compare>::operator*()
{
	return _node->_data;
}


template<typename Key, typename T, typename Compare>
std::pair<Key, T>* reverse_flat_tree_iterator<Key, T, Compare>::operator->()
{
	return &(_node->_data);
}


template<typename Key, typename T, typename Compare>
reverse_flat_tree_iterator<Key, T, Compare>& reverse_flat_tree_iterator<Key, T, Compare>::operator++()
{
	_node = _node->previous();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
reverse_flat_tree_iterator<Key, T, Compare> reverse_flat_tree_iterator<Key, T, Compare>::operator++(int)
{
	reverse_flat_tree_iterator<Key, T, Compare> iter(_node->previous());
	return iter;
}
*/


template<typename Key, typename T, typename Compare>
reverse_flat_tree_iterator<Key, T, Compare>& reverse_flat_tree_iterator<Key, T, Compare>::operator--()
{
	_node = _node->next();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
reverse_flat_tree_iterator<Key, T, Compare> reverse_flat_tree_iterator<Key, T, Compare>::operator--(int)
{
	reverse_flat_tree_iterator<Key, T, Compare> iter(_node->next());
	return iter;
}
*/

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::reverse_iterator flat_tree<Key, T, Compare>::rbegin()
{
	reverse_flat_tree_iterator<Key, T, Compare> iter(last_node(root));
	return iter;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::reverse_iterator flat_tree<Key, T, Compare>::rend()
{
	reverse_flat_tree_iterator<Key, T, Compare> iter(nullptr);
	return iter;
}

// ------------------------------------------------------------------------------------------
// const_reverse_iterator
// ------------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
struct const_reverse_flat_tree_iterator : public std::iterator<std::bidirectional_iterator_tag, Key>
{
	const_reverse_flat_tree_iterator(typename flat_tree<Key, T, Compare>::node* _nnode) : _node(_nnode) {}

	const std::pair<Key, T>& operator*() const;
	const std::pair<Key, T>* operator->() const;
	const_reverse_flat_tree_iterator& operator++();
	//const_reverse_flat_tree_iterator operator++(int);
	const_reverse_flat_tree_iterator& operator--();
	//const_reverse_flat_tree_iterator operator--(int);

	bool operator==(const const_reverse_flat_tree_iterator& other) const { return _node == other._node; }
	bool operator!=(const const_reverse_flat_tree_iterator& other) const { return !(*this == other); }
private:
	const typename flat_tree<Key, T, Compare>::node* _node;
};

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
const std::pair<Key, T>& const_reverse_flat_tree_iterator<Key, T, Compare>::operator*() const
{
	return _node->_data;
}


template<typename Key, typename T, typename Compare>
const std::pair<Key, T>* const_reverse_flat_tree_iterator<Key, T, Compare>::operator->() const
{
	return &(_node->_data);
}


template<typename Key, typename T, typename Compare>
const_reverse_flat_tree_iterator<Key, T, Compare>& const_reverse_flat_tree_iterator<Key, T, Compare>::operator++()
{
	_node = _node->previous();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
const_reverse_flat_tree_iterator<Key, T, Compare> const_reverse_flat_tree_iterator<Key, T, Compare>::operator++(int)
{
	const_reverse_flat_tree_iterator<Key, T, Compare> iter(_node->previous());
	return iter;
}
*/


template<typename Key, typename T, typename Compare>
const_reverse_flat_tree_iterator<Key, T, Compare>& const_reverse_flat_tree_iterator<Key, T, Compare>::operator--()
{
	_node = _node->next();
	return *this;
}

/*
template<typename Key, typename T, typename Compare>
const_reverse_flat_tree_iterator<Key, T, Compare> const_reverse_flat_tree_iterator<Key, T, Compare>::operator--(int)
{
	const_reverse_flat_tree_iterator<Key, T, Compare> iter(_node->next());
	return iter;
}
*/

// -------------------------------------------------------------------------------------

template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::const_reverse_iterator flat_tree<Key, T, Compare>::crbegin()
{
	const_reverse_flat_tree_iterator<Key, T, Compare> iter(last_node(root));
	return iter;
}


template<typename Key, typename T, typename Compare>
typename flat_tree<Key, T, Compare>::const_reverse_iterator flat_tree<Key, T, Compare>::crend()
{
	const_reverse_flat_tree_iterator<Key, T, Compare> iter(nullptr);
	return iter;
}


} // end namespace jod

#endif
