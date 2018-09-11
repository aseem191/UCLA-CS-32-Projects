#ifndef MYHASH

#define MYHASH

#include "provided.h"

template <class KeyType, class ValueType>
class MyHash
{
public:
	MyHash(double maxLoadFactor = 0.5);
	~MyHash();
	void reset();
	void associate(const KeyType& key, const ValueType& value);
	int getNumItems() const;
	double getLoadFactor() const;

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;

private:
	int currItemNum;
	double maxLoad;
	struct Node {
		KeyType key;
		ValueType value;
		Node* next;
		Node(KeyType p_key, ValueType p_val) {
			key = p_key;
			value = p_val;
			next = nullptr;
		}
	};
	void createNewArray(int size);
	Node** buckets;
	int arraySize;
};

template <class KeyType, class ValueType>
MyHash<KeyType,ValueType>::MyHash(double maxLoadFactor) {
	currItemNum = 0;
	if (maxLoadFactor <= 0) {
		maxLoad = 0.5;
	}
	else if (maxLoadFactor > 2) {
		maxLoad = 2;
	}
	else {
		maxLoad = maxLoadFactor;
	}
	arraySize = 100;
	buckets = new Node*[arraySize];
	for (int i = 0; i < arraySize; i++) {
		buckets[i] = nullptr;
	}
}


template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
	for (int i = 0; i < arraySize; i++) {
		if (buckets[i] != nullptr) {
			Node* tmp;
			while(buckets[i]->next != nullptr) {
				tmp = buckets[i]->next;
				delete buckets[i];
				buckets[i] = tmp;
			}
			delete buckets[i];
		}
	}

	delete[] buckets;
}


template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reset() {
	for (int i = 0; i < arraySize; i++) {
		if (buckets[i] != nullptr) {
			Node* tmp;
			while (buckets[i]->next != nullptr) {
				tmp = buckets[i]->next;
				delete buckets[i];
				buckets[i] = tmp;
			}
			delete buckets[i];
		}
	}

	delete[] buckets;

	arraySize = 100;
	currItemNum = 0;
	buckets = new Node*[arraySize];
	for (int i = 0; i < arraySize; i++) {
		buckets[i] = nullptr;
	}
}


template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const	ValueType& value) {
	unsigned int hasher(const KeyType& k);  // prototype
	unsigned int index =  hasher(key) % arraySize;
	if (buckets[index] == nullptr) {
		buckets[index] = new Node(key, value);
		currItemNum += 1;
		if (1.0*currItemNum / arraySize > maxLoad) {
			createNewArray(arraySize * 2);
		}
	}
	else {
		Node* p = buckets[index];
		Node* prev = nullptr;
		while (p != nullptr) {
			if (p->key == key) {
				p->value = value;
				return;
			}
			prev = p;
			p = p->next;
		}
		p = new Node(key, value);
		prev->next = p;
		currItemNum += 1;
		if (1.0*currItemNum / arraySize > maxLoad) {
			createNewArray(arraySize * 2);
		}
	}
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key)	const {
	unsigned	int	hasher(const	KeyType&	k);
	int index = hasher(key) % arraySize;
	if (buckets[index] == nullptr) {
		return nullptr;
	}
	else {
		if (buckets[index]->key == key) {
			return &buckets[index]->value;
		}
		else {
			Node* p = buckets[index];
			while (p != nullptr) {
				if (p->key == key) {
					return &(p->value);
				}
				p = p->next;
			}
			return nullptr;
		}
	}
}

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems()	const {
	return currItemNum;
}



template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor()	const {
	return 1.0*currItemNum / arraySize;
}


template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::createNewArray(int size) {
	unsigned	int	hasher(const	KeyType&	k);
	currItemNum = 0;
	Node** newBuckets = new Node*[size];
	for (int i = 0; i < size; i++) {
		newBuckets[i] = nullptr;
	}

	for (int i = 0; i < arraySize; i++) {
		if (buckets[i] != nullptr) {
			Node* p = buckets[i];
			int index;
			while (p != nullptr) {
				index = hasher(p->key) % size;
				if (newBuckets[index] == nullptr) {
					newBuckets[index] = new Node(p->key, p->value);
					currItemNum += 1;
				}
				else {
					Node* q = newBuckets[index];
					Node* prev = nullptr;
					while (q != nullptr) {
						prev = q;
						q = q->next;
					}
					q = new Node(p->key, p->value);
					prev->next = q;
					currItemNum += 1;
				}
				p = p->next;
			}
		}
	}

	for (int i = 0; i < arraySize; i++) {
		if (buckets[i] != nullptr) {
			Node* tmp;
			while (buckets[i]->next != nullptr) {
				tmp = buckets[i]->next;
				delete buckets[i];
				buckets[i] = tmp;
			}
			delete buckets[i];
		}
	}

	delete[] buckets;
	buckets = newBuckets;
	arraySize = size;
}

#endif