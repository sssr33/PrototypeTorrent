#include "pch.h"
#include "BencodeDictionary.h"

BencodeDictionary::BencodeDictionary(){
	this->SetType(BencodeType::Dictionary);
}

BencodeDictionary::~BencodeDictionary(){
}

std::reference_wrapper<std::map<std::string, std::unique_ptr<BencodeObject>>> BencodeDictionary::GetReference(){
	std::reference_wrapper<std::map<std::string, std::unique_ptr<BencodeObject>>> ref(this->value);
	return ref;
}

std::map<std::string, std::unique_ptr<BencodeObject>> *BencodeDictionary::GetPointer(){
	return &this->value;
}

const std::map<std::string, std::unique_ptr<BencodeObject>> &BencodeDictionary::GetValue() const{
	return this->value;
}

void BencodeDictionary::SetValue(std::map<std::string, std::unique_ptr<BencodeObject>> &&v){
	this->value = std::move(v);
}