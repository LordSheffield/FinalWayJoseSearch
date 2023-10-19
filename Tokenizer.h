#pragma once
#include <iostream>
#include <sstream>
#include <string>

/*
/////////////////////////////////////////////////////////////
Credits to Mr. Aubrey Knight. 
Class/code stolen from Data Structures class back in the day
/////////////////////////////////////////////////////////////
*/

using std::istream;
using std::stringstream;
using std::string;

class Tokenizer
{
private:
	stringstream ss;
public:
	Tokenizer() { ss.exceptions(std::ios::failbit); }
	Tokenizer(const string& source) { ss.str(source); ss.exceptions(std::ios::failbit); }

	void setString(const string& str);
	bool isEmpty();
	bool readLine(string& line);
	bool readInteger(int& val);
	bool readWord(string& val);
	bool readDouble(double& val);
	bool readChar(char& val);

	void rewind();
	void clear();
};