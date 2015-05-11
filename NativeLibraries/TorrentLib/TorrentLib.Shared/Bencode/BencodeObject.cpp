#include "pch.h"
#include "BencodeObject.h"

BencodeObject::BencodeObject()
	: start(-1), contentStart(-1),
	length(-1), contentLength(-1), type(BencodeType::Unknown){
}

BencodeObject::~BencodeObject(){
}

int BencodeObject::GetStart() const{
	return this->start;
}

void BencodeObject::SetStart(int v){
	this->start = v;
}

int BencodeObject::GetContentStart() const{
	return this->contentStart;
}

void BencodeObject::SetContentStart(int v){
	this->contentStart = v;
}

int BencodeObject::GetLength() const{
	return this->length;
}

void BencodeObject::SetLength(int v){
	this->length = v;
}

int BencodeObject::GetContentLength() const{
	return this->contentLength;
}

void BencodeObject::SetContentLength(int v){
	this->contentLength = v;
}

BencodeType BencodeObject::GetType() const{
	return this->type;
}

BencodeDictionary *BencodeObject::AsDictionary(){
	auto v = reinterpret_cast<BencodeDictionary *>(this);
	return v;
}

const BencodeDictionary *BencodeObject::AsDictionary() const{
	auto v = reinterpret_cast<const BencodeDictionary *>(this);
	return v;
}

BencodeList *BencodeObject::AsList(){
	auto v = reinterpret_cast<BencodeList *>(this);
	return v;
}

const BencodeList *BencodeObject::AsList() const{
	auto v = reinterpret_cast<const BencodeList *>(this);
	return v;
}

BencodeString *BencodeObject::AsString(){
	auto v = reinterpret_cast<BencodeString *>(this);
	return v;
}

const BencodeString *BencodeObject::AsString() const{
	auto v = reinterpret_cast<const BencodeString *>(this);
	return v;
}

BencodeInt *BencodeObject::AsInteger(){
	auto v = reinterpret_cast<BencodeInt *>(this);
	return v;
}

const BencodeInt *BencodeObject::AsInteger() const{
	auto v = reinterpret_cast<const BencodeInt *>(this);
	return v;
}

void BencodeObject::SetType(BencodeType v){
	this->type = v;
}