#pragma once
#include "..\Helpers\coroutine3.h"

class BencodeDocument{
public:
	BencodeDocument();
	~BencodeDocument();

	bool Parse(const void *data, size_t size);
private:
	coroutine3 coParse;
};