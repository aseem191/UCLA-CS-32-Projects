#include <string>
#include <vector>
#include <algorithm>
#include "provided.h"
#include "MyHash.h"
using namespace std;

class DecrypterImpl
{
public:
	DecrypterImpl();
	~DecrypterImpl();
	bool load(std::string filename);
	std::vector<std::string> crack(const std::string& ciphertext);
private:
	std::vector<std::string> cracker(const std::string& cipherSentence, std::vector<std::string> ciphertext, std::vector<std::string> plaintext);
	WordList wordList;
	Tokenizer tokenizer;
	Translator translator;
};

DecrypterImpl::DecrypterImpl() :tokenizer("1234567890 ,;:.!()[]{}-#$%^&") {

}

DecrypterImpl::~DecrypterImpl() {

}

bool DecrypterImpl::load(std::string filename) {
	return wordList.loadWordList(filename);
}

std::vector<std::string> DecrypterImpl::crack(const std::string& ciphertext) {
	for (int i = 0; i < ciphertext.size(); i++) {
		if (!isalpha(ciphertext[i]) && !isdigit(ciphertext[i]) && !(ciphertext[i] == ' ' || ciphertext[i] == ',' || ciphertext[i] == ';' || ciphertext[i] == ':' || ciphertext[i] == '.' || ciphertext[i] == '!' || ciphertext[i] == '(' || ciphertext[i] == ')' || ciphertext[i] == '[' || ciphertext[i] == ']' || ciphertext[i] == '{' || ciphertext[i] == '}' || ciphertext[i] == '-' || ciphertext[i] == '#' || ciphertext[i] == '$' || ciphertext[i] == '%' || ciphertext[i] == '^' || ciphertext[i] == '&' || ciphertext[i] == '\'')) {
			vector<string> wrong;
			return wrong;
		}
	}

	vector<string> tokenized = tokenizer.tokenize(ciphertext);
	
	vector<string> plaintext;

	for (int i = 0; i < tokenized.size(); i++) {
		plaintext.push_back(translator.getTranslation(tokenized[i]));
	}

	return cracker(ciphertext, tokenized, plaintext);
}

std::vector<std::string> DecrypterImpl::cracker(const std::string& cipherSentence, std::vector<std::string> ciphertext, std::vector<std::string> plaintext) {
	vector<string> possibleSolns;
	int largest = 0;
	int largestnum = 0;
	for (int i = 0; i < plaintext.size(); i++) {
		int currCount = 0;
		for (int j = 0; j < plaintext[i].size(); j++) {
			if (plaintext[i][j] == '?') {
				currCount++;
			}
		}
		if (currCount > largestnum) {
			largest = i;
			largestnum = currCount;
		}
	}
	string translated = translator.getTranslation(ciphertext[largest]);
	vector<string> candidates = wordList.findCandidates(ciphertext[largest], translated);
	if (candidates.empty()) {
		vector<string> wrong;
		return wrong;
		
	}

	for (int i = 0; i < candidates.size(); i++) {
		if (translator.pushMapping(ciphertext[largest], candidates[i]) == false) {
			continue;
		}
		for (int j = 0; j < ciphertext.size(); j++) {
			plaintext[j] = translator.getTranslation(ciphertext[j]);
		}
		vector<string> fullyTranslated;
		for (int j = 0; j < plaintext.size(); j++) {
			for (int k = 0; k < plaintext[j].size(); k++) {
				if (plaintext[j][k] == '?') {
					break;
				}

				if (k == plaintext[j].size() - 1 && plaintext[j][k] != '?') {
					fullyTranslated.push_back(plaintext[j]);
				}
			}
		}

		bool doesntwork = false;
		for (int j = 0; j < fullyTranslated.size(); j++){
			if (!wordList.contains(fullyTranslated[j])) {
				doesntwork = true;
			}
		}

		if (doesntwork == true) {
			translator.popMapping();
			continue;
		}

		if (doesntwork == false && fullyTranslated.size() != plaintext.size()) {
			vector<string> result = cracker(cipherSentence, ciphertext, plaintext);
			for (int j = 0; j < result.size(); j++) {
				possibleSolns.push_back(result[j]);
			}
			translator.popMapping();
		}

		if (doesntwork == false && fullyTranslated.size() == plaintext.size()) {
			translator.popMapping();
			string currSoln;
			for (int j = 0; j < fullyTranslated.size(); j++) {
				currSoln += fullyTranslated[j];
				int separatorindex = -1;
				for (int k = 0; k <= j; k++) {
					separatorindex += fullyTranslated[k].size();
					separatorindex++;
					while (!(separatorindex >= cipherSentence.size()) && !(k == j)) {
						separatorindex++;
						if (isalpha(cipherSentence[separatorindex])) {
							separatorindex--;
							break;
						}
					}
				}
				while (!(separatorindex >= cipherSentence.size()) && !isalpha(cipherSentence[separatorindex])) {
					currSoln += cipherSentence[separatorindex];
					separatorindex++;
				}

			}
			possibleSolns.push_back(currSoln);
		}

	}
	sort(possibleSolns.begin(), possibleSolns.end());
	return possibleSolns;
}

Decrypter::Decrypter() {
	m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter() {
	delete m_impl;
}

bool Decrypter::load(std::string filename) {
	return m_impl->load(filename);
}

std::vector<std::string> Decrypter::crack(const std::string& ciphertext) {
	return m_impl->crack(ciphertext);
}
