#include <string>
#include <vector>
#include "provided.h"
#include "MyHash.h"
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	~TranslatorImpl();
	bool pushMapping(std::string ciphertext, std::string plaintext);
	bool popMapping();
	std::string getTranslation(std::string ciphertext);

private:
	struct mapList
	{
		MyHash<char, char> m_hash;
		mapList* next;
		mapList* prev;
		mapList() {
			next = nullptr;
		}
	};
	mapList* head;
	mapList* tail;
	mapList* headReverse;
	mapList* tailReverse;
};

TranslatorImpl::TranslatorImpl() {
	head = tail = nullptr;
	headReverse = tailReverse = nullptr;
}

TranslatorImpl::~TranslatorImpl() {
	mapList* p = head;
	while(p != nullptr) {
		mapList* tmp = p;
		p = p->next;
		delete tmp;
	}
	p = headReverse;
	while(p != nullptr) {
		mapList* tmp = p;
		p = p->next;
		delete tmp;
	}
}

bool TranslatorImpl::pushMapping(std::string ciphertext, std::string plaintext) {
	for (int i = 0; i < ciphertext.size(); i++){
		ciphertext[i] = toupper(ciphertext[i]);
		plaintext[i] = toupper(plaintext[i]);
	}

	if (head == nullptr) {
		head = new mapList;
		tail = head;
		head->next = head->prev = nullptr;

		headReverse = new mapList;
		tailReverse = headReverse;
		headReverse->next = headReverse->prev = nullptr;
		for (char i = 'A'; i <= 'Z'; i++) {
			tail->m_hash.associate(i, '?');
		}
		for (char i = 'A'; i <= 'Z'; i++) {
			tailReverse->m_hash.associate(i, '?');
		}
		for (int i = 0; i < ciphertext.size(); i++) {
			if (*(tail->m_hash.find(ciphertext[i])) != '?') {
				if (*(tail->m_hash.find(ciphertext[i])) == plaintext[i]){
				}
				else {
					delete headReverse;
					headReverse = tailReverse = nullptr;
					delete head;
					head = tail = nullptr;
					return false;
				}
			}
			else {
				if (*(tailReverse->m_hash.find(plaintext[i])) != '?') {
					if (*(tailReverse->m_hash.find(plaintext[i])) != ciphertext[i]) {
						delete headReverse;
						headReverse = tailReverse = nullptr;
						delete head;
						head = tail = nullptr;
						return false;
					}
				}
				tail->m_hash.associate(ciphertext[i], plaintext[i]);
				tailReverse->m_hash.associate(plaintext[i], ciphertext[i]);
			}

		}
	}
	else {
		tail->next = new mapList;
		mapList* prevtailptr = tail;
		tail = tail->next;
		tail->prev = prevtailptr;

		tailReverse->next = new mapList;
		mapList* prevtailptrRev = tailReverse;
		tailReverse = tailReverse->next;
		tailReverse->prev = prevtailptrRev;

		for (char i = 'A'; i <= 'Z'; i++) {
			if (tail->prev == nullptr) {
				tail->m_hash.associate(i, '?');
			}
			else {
				tail->m_hash.associate(i, *(tail->prev->m_hash.find(i)));
			}

		}

		for (char i = 'A'; i <= 'Z'; i++) {
			if (tailReverse->prev == nullptr) {
				tailReverse->m_hash.associate(i, '?');
			}
			else {
				tailReverse->m_hash.associate(i, *(tailReverse->prev->m_hash.find(i)));
			}
		}
		for (int i = 0; i < ciphertext.size(); i++) {
			if (*(tail->m_hash.find(ciphertext[i])) != '?') {
				if (*(tail->m_hash.find(ciphertext[i])) == plaintext[i]) {
					
				}
				else {
					delete tailReverse;
					prevtailptrRev->next = nullptr;
					tailReverse = prevtailptrRev;
					delete tail;
					prevtailptr->next = nullptr;
					tail = prevtailptr;
					return false;
				}
			}
			else {
				if (*(tailReverse->m_hash.find(plaintext[i])) != '?') {
					if (*(tailReverse->m_hash.find(plaintext[i])) != ciphertext[i]) {
						delete tailReverse;
						prevtailptrRev->next = nullptr;
						tailReverse = prevtailptrRev;
						delete tail;
						prevtailptr->next = nullptr;
						tail = prevtailptr;
						return false;
					}
				}
				tail->m_hash.associate(ciphertext[i], plaintext[i]);
				tailReverse->m_hash.associate(plaintext[i], ciphertext[i]);
			}

			

		}
	}

	return true;
}

bool TranslatorImpl::popMapping() {
	if (tail == nullptr) {
		return false;
	}
	else {
		if (tail->prev != nullptr) {
			tail->prev->next = nullptr;
		}
		else {
			head = nullptr;
		}
		mapList* holder = tail;
		tail = tail->prev;
		delete holder;
	}

	if (tailReverse == nullptr) {
		return false;
	}
	else {
		if (tailReverse->prev != nullptr) {
			tailReverse->prev->next = nullptr;
		}
		else {
			headReverse = nullptr;
		}
		mapList* holder = tailReverse;
		tailReverse = tailReverse->prev;
		delete holder;
	}
	
	return true;
}

std::string TranslatorImpl::getTranslation(std::string ciphertext){
	string plaintext;
	for (int i = 0; i < ciphertext.size(); i++) {
		plaintext += '?';
	}

	if (tail == nullptr) {
		for (int i = 0; i < ciphertext.size(); i++) {
			if (isalpha(ciphertext[i])) {

			}
			else {
				plaintext[i] = ciphertext[i];
			}
		}
		return plaintext;
	}

	for (int i = 0; i < ciphertext.size(); i++){
		if (isalpha(ciphertext[i])) {
			if (tail->m_hash.find(toupper(ciphertext[i])) != nullptr) {
				if (isupper(ciphertext[i])) {
					plaintext[i] = *(tail->m_hash.find(toupper(ciphertext[i])));
				}
				else {
					plaintext[i] = tolower(*(tail->m_hash.find(toupper(ciphertext[i]))));
				}
			}
		}
		else {
			plaintext[i] = ciphertext[i];
		}
	}

	return plaintext;
}


Translator::Translator() {
	m_impl = new TranslatorImpl;
}

Translator::~Translator() {
	delete m_impl;
}

bool Translator::pushMapping(std::string ciphertext, std::string plaintext) {
	return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping() {
	return m_impl->popMapping();
}

std::string Translator::getTranslation(const std::string& ciphertext) const {
	return m_impl->getTranslation(ciphertext);
}
