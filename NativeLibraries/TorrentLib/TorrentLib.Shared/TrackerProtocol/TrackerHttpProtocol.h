#pragma once
#include "Bencode/BencodeDocument.h"

class TrackerHttpProtocol{
public:
	TrackerHttpProtocol();
	~TrackerHttpProtocol();

	void Send(const BencodeDocument *doc);
};