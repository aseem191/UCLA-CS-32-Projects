#include <string>
#include <vector>
#include "MyHash.h"
#include "provided.h"
#include <fstream>
#include <cctype>
using namespace std;

unsigned int hasher(const	std::string&	s)
{
	return std::hash<std::string>()(s);
}
unsigned int hasher(const	int&	i)
{
	return std::hash<int>()(i);
}
unsigned int hasher(const	char&	c)
{
	return std::hash<char>()(c);
}

class WordListImpl
{
public:
	WordListImpl();
	bool loadWordList(std::string filename);
	bool contains(std::string word);
	std::vector<std::string>	findCandidates(std::string cipherWord,
		std::string currTranslation);

private:
	MyHash<std::string, vector<std::string>> wordMap;
	MyHash<std::string, int> quickWordMap;
};


WordListImpl::WordListImpl() {

}

bool WordListImpl::loadWordList(std::string filename) {
	ifstream infile(filename);
	if (!infile)		        // Did opening the file fail?
	{
		return false;
	}

	string currWord;
	while (getline(infile, currWord))
	{
		for (int i = 0; i < currWord.size(); i++) {
			currWord[i] = tolower(currWord[i]);
		}
		quickWordMap.associate(currWord, 0);
		string pattern;
		for (int i = 0; i < currWord.size(); i++) {
			pattern += '?';
		}

		bool alreadyletter = false;
		char currletter = 'A';

		for (int i = 0; i < currWord.size(); i++) {
			alreadyletter = false;
			for (int j = 0; j < i; j++) {
				if (currWord[j] == currWord[i]) {
					pattern[i] = pattern[j];
					alreadyletter = true;
					break;
				}
			}

			if (alreadyletter == false) {
				pattern[i] = currletter;
				currletter++;
			}
		}

		vector<string>* result = wordMap.find(pattern);
		if(result != nullptr) {
			vector<string> first = *result;
			first.push_back(currWord);
			wordMap.associate(pattern, first);
		}
		else {
			vector<string> first;
			first.push_back(currWord);
			wordMap.associate(pattern, first);
		}
		
	}
	return true;
}

bool WordListImpl::contains(std::string word){
	for (int i = 0; i < word.size(); i++) {
		word[i] = tolower(word[i]);
	}
	
	if (quickWordMap.find(word) != nullptr) {
		return true;
	}
	return false;
}

std::vector<std::string>	WordListImpl::findCandidates(std::string cipherWord, std::string currTranslation){
	string pattern;
	string currWord = cipherWord;
	for (int i = 0; i < currWord.size(); i++) {
		currWord[i] = tolower(currWord[i]);
	}
	for (int i = 0; i < currWord.size(); i++) {
		pattern += '?';
		
	}

	bool alreadyletter = false;
	char currletter = 'A';

	for (int i = 0; i < currWord.size(); i++) {
		alreadyletter = false;
		for (int j = 0; j < i; j++) {
			if (currWord[j] == currWord[i]) {
				pattern[i] = pattern[j];
				alreadyletter = true;
				break;
			}
		}

		if (alreadyletter == false) {
			pattern[i] = currletter;
			currletter++;
		}
	}

	vector<string>* desiredptr = wordMap.find(pattern);
	if (desiredptr == nullptr) {
		vector<string> eligiblewords;
		return eligiblewords;
	}
	vector<string> desired = *desiredptr;
	
	bool works = true;
	vector<string> eligiblewords;

	for (int i = 0; i < cipherWord.size(); i++) {
		currTranslation[i] = tolower(currTranslation[i]);
	}
	for(int i = 0; i < desired.size(); i++){
		for (int j = 0; j < desired[i].size(); j++) {
			desired[i][j] = tolower(desired[i][j]);
		}
		works = true;
		for (int j = 0; j < desired[i].size(); j++) {
			if (isalpha(currTranslation[j])) {
				if (currTranslation[j] != desired[i][j]) {
					works = false;
				}
			}
			else if (currTranslation[j] == '?') {
				if (!isalpha(desired[i][j])) {
					works = false;
				}
			}
			else if (currTranslation[j] == '\'') {
				if ('\'' != desired[i][j]) {
					works = false;
				}
			}
		}

		if (works == true) {
			eligiblewords.push_back(desired[i]);
		}
	}
	return eligiblewords;
}


WordList::WordList() {
	m_impl = new WordListImpl;
}

WordList::~WordList() {
	delete m_impl;
}

bool WordList::loadWordList(std::string dictFilename) {
	return m_impl->loadWordList(dictFilename);
}

bool WordList::contains(std::string word)	const {
	return m_impl->contains(word);
}

std::vector<std::string> WordList::findCandidates(std::string cipherWord, std::string currTranslation)	const {
	return m_impl->findCandidates(cipherWord, currTranslation);
}