#pragma once
#include "BencodeObject.h"

#include <cstdint>

class BencodeInt : public BencodeObject{
public:
	BencodeInt();
	virtual ~BencodeInt();

	int64_t GetValue() const;
	void SetValue(int64_t v);
private:
	int64_t value;
};