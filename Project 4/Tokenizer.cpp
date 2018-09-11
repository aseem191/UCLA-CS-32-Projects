#include <string>
#include <vector>
#include "provided.h"
using namespace std;

class TokenizerImpl
{
public:
	TokenizerImpl(string separators);
	vector<string> tokenize(const std::string& s) const;
private:
	string separator;
};

TokenizerImpl::TokenizerImpl(std::string separators) {
	for (int i = 0; i < separators.size(); i++) {
		separator += separators[i];
	}
}

std::vector<std::string> TokenizerImpl::tokenize(const std::string& s) const {
	std::vector<std::string> separated;
	std::string tmp;
	bool isSeparator = false;

	for (int i = 0; i < s.size(); i++) {
		for (int j = 0; j < separator.size(); j++){
			if (s[i] == separator[j]) {
				if (tmp != "") {
					separated.push_back(tmp);
				}
				tmp = "";
				isSeparator = true;
				break;
			}
		}
		if (isSeparator == true) {
			isSeparator = false;
		}
		else {
			tmp += s[i];
		}
	}

	if (tmp != "") {
		separated.push_back(tmp);
	}

	return separated;
}

Tokenizer::Tokenizer(string separators)
{
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
	return m_impl->tokenize(s);
}
