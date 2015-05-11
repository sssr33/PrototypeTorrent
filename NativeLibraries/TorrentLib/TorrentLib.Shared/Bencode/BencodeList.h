#pragma once
#include "BencodeObject.h"

#include <list>
#include <memory>

class BencodeList : public BencodeObject{
public:
	BencodeList();
	virtual ~BencodeList();

	std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> GetReference();
	std::list<std::unique_ptr<BencodeObject>> *GetPointer();
	const std::list<std::unique_ptr<BencodeObject>> &GetValue() const;
	void SetValue(std::list<std::unique_ptr<BencodeObject>> &&v);
private:
	std::list<std::unique_ptr<BencodeObject>> value;
};