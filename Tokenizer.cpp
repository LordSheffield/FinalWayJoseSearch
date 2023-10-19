#include "Tokenizer.h"

/*
/////////////////////////////////////////////////////////////
Credits to Mr. Aubrey Knight.
Class/code stolen from Data Structures class back in the day
/////////////////////////////////////////////////////////////
*/

void Tokenizer::setString(const string& str)
{
	ss.str(str);
	ss.clear();
}
bool Tokenizer::isEmpty()
{
	try
	{
		ss.peek();
		//if (ss.eof()) return true;
		return false;
	}
	catch (...)
	{
		return true;
	}
}
bool Tokenizer::readLine(string& line)
{
	if (isEmpty()) return false;
	if (getline(ss, line)) return true;
	return false;
}
bool Tokenizer::readInteger(int& val)
{
	if (isEmpty()) return false;
	try
	{
		ss >> val;
	}
	catch (std::ios_base::failure)
	{
		ss.clear();
		ss.unget();
		if (ss.bad()) ss.clear();
		return false;
	}
	return true;
}
bool Tokenizer::readWord(string& val)
{
	if (isEmpty()) return false;
	try
	{
		ss >> val;
	}
	catch (std::ios_base::failure)
	{
		ss.clear();
		return false;
	}
	return true;
}
bool Tokenizer::readDouble(double& val)
{
	if (isEmpty()) return false;
	try
	{
		ss >> val;
	}
	catch (std::ios_base::failure)
	{
		ss.clear();
		ss.unget();
		if (ss.bad()) ss.clear();
		return false;
	}
	return true;
}

bool Tokenizer::readChar(char& val)
{
	if (isEmpty()) return false;
	try
	{
		ss >> val;
	}
	catch (std::ios_base::failure)
	{
		ss.clear();
		return false;
	}
	return true;
}

void Tokenizer::rewind()
{
	ss.seekg(0, ss.beg);
}
void Tokenizer::clear()
{
	setString("");
	ss.setstate(std::ios::eofbit);
}

