#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>

template <typename T>
class BinarySearchTree {
private:

public:
	struct node {
		T key;
		node* left;
		node* right;

		node(const T& key = T(), node* left = nullptr, node* right = nullptr)
			: key(key), left(left), right(right) {}

		void inorder() const {
			if (left)
				left->inorder();
			std::cout << " " << key << " ";
			if(right)
				right->inorder();
		}

		T sumOfElements() {
			T result = T();
			if(left)
				result += left->sumOfElements();

			result += key;

			if(right) 
				result += right->sumOfElements();
		
			return result;
		}
	};

	BinarySearchTree() : root(nullptr), size(0) {}

	BinarySearchTree(const BinarySearchTree& other)
		:root(copy(other.root)), size(other.size) {}

	BinarySearchTree& operator=(const BinarySearchTree& other) {
		if (this != &other) {
			clear(root);
			root = copy(other.root);
			size = other.size;
		}
		return *this;
	}

	~BinarySearchTree() { clear(root); }

	bool find(const T& key) const { return find(root, key); }
	void insert(const T& key) { insert(root, key); }
	bool remove(const T& key) { return remove(root, key); }

	size_t get_size() const { return size; }
	bool is_balanced() const { return is_balanced(root); }
	bool is_perfectly_balanced() const { return is_perfectly_balanced(root); }
	static BinarySearchTree<T>* buildTree(char const*);
	void inorder() const;
	T sumOfElements() const;

private:
	void clear(node* root);
	node* copy(node* root);

	bool find(const node* root, const T& key) const;
	void insert(node*& root, const T& key);
	bool remove(node*& root, const T& key);
	node* extract_min(node*& root);

	int height(const node* root) const;
	int weight(const node* root) const;

	bool is_balanced(const node* root) const;
	bool is_perfectly_balanced(const node* root) const;
	static node* buildTreeHelper(char const*&);

private:
	node* root;
	size_t size;
};

template <>
inline BinarySearchTree<int>::node* BinarySearchTree<int>::buildTreeHelper(char const*& letter) {
	//letter = (
	++letter;
	
	// digit or )
	if(*letter == ')') {
		++letter;
		return nullptr;
	}

	int value = 0; 
    while(isdigit(*letter)) {
		value = value*10 + (*letter-'0');
		++letter;
	}

	// letter = (
	node* left = buildTreeHelper(letter);
	// letter = (
	node* right = buildTreeHelper(letter);

	while(*letter == ')') {
		++letter;
	}

	return new node(value, left, right);
}

template <typename T>
inline BinarySearchTree<T>* BinarySearchTree<T>::buildTree(char const * letters) {
	BinarySearchTree<T>* result = new BinarySearchTree<T>();

	result->root = buildTreeHelper(letters);
	result->size = result->weight(result->root);

    return result;
}

template <typename T>
inline void BinarySearchTree<T>::inorder() const {
	this->root->inorder();
}

template <>
inline int BinarySearchTree<int>::sumOfElements() const {
	if(!root) {
		return 0;
	}

	return root->sumOfElements();
}

template <typename T>
inline void BinarySearchTree<T>::clear(node *root)
{
    if (!root) 
		return;

	clear(root->left);
	clear(root->right);
	delete root;
}

template<typename T>
inline typename BinarySearchTree<T>::node* BinarySearchTree<T>::copy(node* root) {
	if (!root)
		return nullptr;

	return new node(root->key, copy(root->left), copy(root->right));
}

template<typename T>
inline bool BinarySearchTree<T>::find(const node* root, const T& key) const {
	if (!root)
		return false;

	if (root->key == key)
		return true;

	return key < root->key ? 
		find(root->left, key) : 
		find(root->right, key);
}

template<typename T>
inline void BinarySearchTree<T>::insert(node*& root, const T& key) {
	if (!root) {
		root = new node(key);
		size++;
		return;
	}

	insert(key < root->key ?
		root->left :
		root->right,
		key);
}

template<typename T>
inline bool BinarySearchTree<T>::remove(node*& root, const T& key) {
	if (!root)
		return false;

	if (root->key == key) {
		node* to_remove = root;
		if (!root->left) {
			root = root->right;
		}
		else if (!root->right) {
			root = root->left;
		}
		else {
			// Ако имаме 2 наследника
			node* min_right = extract_min(root->right);
			min_right->left = root->left;
			min_right->right = root->right;
			root = min_right;
		}
		size--;
		delete to_remove;
		return true;
	}
	else {
		return remove(key < root->key ?
			root->left :
			root->right,
			key);
	}
}

template<typename T>
inline typename BinarySearchTree<T>::node* BinarySearchTree<T>::extract_min(node*& root) {
	// В десния наследник вървим по всички леви, за да стигнем до най-малкото
	if (root->left)
		return extract_min(root->left);

	node* min = root;
	root = root->right;
	return min;
}

template<typename T>
inline int BinarySearchTree<T>::height(const node* root) const {
	if (!root)
		return 0;
	
	return 1 + std::max(height(root->left), height(root->right));
}

template<typename T>
inline int BinarySearchTree<T>::weight(const node* root) const {
	if (!root)
		return 0;

	return 1 + weight(root->left) + weight(root->right);
}

template<typename T>
inline bool BinarySearchTree<T>::is_balanced(const node* root) const {
	if (!root)
		return true;

	return is_balanced(root->left) 
		&& is_balanced(root->right) 
		&& abs(height(root->left) - height(root->right)) <= 1;
}

template<typename T>
inline bool BinarySearchTree<T>::is_perfectly_balanced(const node* root) const {
	if (!root)
		return true;

	return is_perfectly_balanced(root->left)
		&& is_perfectly_balanced(root->right)
		&& abs(weight(root->left) - weight(root->right)) <= 1;
}
