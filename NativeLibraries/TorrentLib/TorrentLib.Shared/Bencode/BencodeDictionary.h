#pragma once
#include "BencodeObject.h"

#include <map>
#include <string>
#include <memory>

class BencodeDictionary : public BencodeObject{
public:
	BencodeDictionary();
	virtual ~BencodeDictionary();

	std::reference_wrapper<std::map<std::string, std::unique_ptr<BencodeObject>>> GetReference();
	std::map<std::string, std::unique_ptr<BencodeObject>> *GetPointer();
	const std::map<std::string, std::unique_ptr<BencodeObject>> &GetValue() const;
	void SetValue(std::map<std::string, std::unique_ptr<BencodeObject>> &&v);
private:
	std::map<std::string, std::unique_ptr<BencodeObject>> value;
};