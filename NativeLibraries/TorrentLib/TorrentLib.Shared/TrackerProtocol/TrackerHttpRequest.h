#pragma once
#include "Bencode/BencodeDocument.h"

class TrackerHttpRequest{
public:
	TrackerHttpRequest();
	~TrackerHttpRequest();

	void Send(const BencodeDocument *doc);
};