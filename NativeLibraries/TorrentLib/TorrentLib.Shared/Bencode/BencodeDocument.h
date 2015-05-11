#pragma once
#include "BencodeObject.h"
#include "BencodeDictionary.h"
#include "BencodeList.h"
#include "BencodeString.h"
#include "BencodeInt.h"

#include <libhelpers\coroutine3.h>

#include <cstdint>
#include <string>
#include <list>
#include <map>

struct ParseState{
	const uint8_t *data;
	size_t size;
	size_t i;
};

class BencodeDocument{
public:
	BencodeDocument();
	~BencodeDocument();

	size_t GetLength() const;

	const BencodeObject *GetRoot() const;
	BencodeObject *GetRoot();

	std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> GetRootListReference();

	bool Parse(const void *data, size_t size);
private:
	coroutine3 coParse;

	size_t length;
	std::list<std::unique_ptr<BencodeObject>> root;

	bool ParseField(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res);
	bool ParseInteger(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res);
	bool ParseString(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res);
	bool ParseDictionary(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res);
	bool ParseList(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res);
};