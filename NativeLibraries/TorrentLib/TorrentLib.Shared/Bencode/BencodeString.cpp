#include "pch.h"
#include "BencodeString.h"

BencodeString::BencodeString(){
	this->SetType(BencodeType::String);
}

BencodeString::~BencodeString(){
}

std::reference_wrapper<std::string> BencodeString::GetReference(){
	std::reference_wrapper<std::string> ref(this->value);
	return ref;
}

std::string *BencodeString::GetPointer(){
	return &this->value;
}

const std::string &BencodeString::GetValue() const{
	return this->value;
}

void BencodeString::SetValue(const std::string &v){
	this->value = v;
}