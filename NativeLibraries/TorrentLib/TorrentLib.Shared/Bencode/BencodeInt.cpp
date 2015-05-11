#include "pch.h"
#include "BencodeInt.h"

BencodeInt::BencodeInt(){
	this->SetType(BencodeType::Integer);
}

BencodeInt::~BencodeInt(){
}

int64_t BencodeInt::GetValue() const{
	return this->value;
}

void BencodeInt::SetValue(int64_t v){
	this->value = v;
}