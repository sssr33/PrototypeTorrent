#pragma once

#include <libhelpers\coroutine3.h>

#include <cstdint>
#include <string>
#include <list>
#include <map>

struct BencodeField{
	bool integer;
	bool string;
	bool list;
	bool dictionary;

	int intVal;
	std::string strVal;
	std::list<BencodeField> listVal;
	std::map<std::string, BencodeField> dictVal;
};

struct ParseState{
	const uint8_t *data;
	size_t size;
	size_t i;
};

class BencodeDocument{
public:
	BencodeDocument();
	~BencodeDocument();

	bool Parse(const void *data, size_t size);
private:
	coroutine3 coParse;

	std::list<BencodeField> rootFields;

	bool ParseField(coroutine3 &co, ParseState &parseState, BencodeField *res);
	bool ParseInteger(coroutine3 &co, ParseState &parseState, BencodeField *res);
	bool ParseString(coroutine3 &co, ParseState &parseState, BencodeField *res);
	bool ParseDictionary(coroutine3 &co, ParseState &parseState, BencodeField *res);
	bool ParseList(coroutine3 &co, ParseState &parseState, BencodeField *res);
};