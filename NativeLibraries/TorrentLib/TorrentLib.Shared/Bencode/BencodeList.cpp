#include "pch.h"
#include "BencodeList.h"

BencodeList::BencodeList(){
	this->SetType(BencodeType::List);
}

BencodeList::~BencodeList(){
}

std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> BencodeList::GetReference(){
	std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> ref(this->value);
	return ref;
}

std::list<std::unique_ptr<BencodeObject>> *BencodeList::GetPointer(){
	return &this->value;
}

const std::list<std::unique_ptr<BencodeObject>> &BencodeList::GetValue() const{
	return this->value;
}

void BencodeList::SetValue(std::list<std::unique_ptr<BencodeObject>> &&v){
	this->value = std::move(v);
}