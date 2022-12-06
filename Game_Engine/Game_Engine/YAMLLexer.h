#pragma once

#define	TOKEND_NOTDEFINED	65536
#define	NUMBER_OF_RESERVEDWORD	10

class YAMLLexer
{
private:
	int ch_Index = 0;

	std::string word;

public:
	YAMLLexer() {};
	~YAMLLexer() {};

public:
	int GetToken(char* line);

	const std::string& GetWord() { return word; }

	void ResetChIndex() { ch_Index = 0; }
};
