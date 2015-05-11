#pragma once
#include "BencodeObject.h"

#include <string>
#include <functional>

class BencodeString : public BencodeObject{
public:
	BencodeString();
	virtual ~BencodeString();

	std::reference_wrapper<std::string> GetReference();
	std::string *GetPointer();
	const std::string &GetValue() const;
	void SetValue(const std::string &v);
private:
	std::string value;
};