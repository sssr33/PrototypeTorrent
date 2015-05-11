#include "pch.h"
#include "BencodeDocument.h"

#include <libhelpers\H.h>

BencodeDocument::BencodeDocument()
	: length(0){
}

BencodeDocument::~BencodeDocument(){
}

size_t BencodeDocument::GetLength() const{
	return this->length;
}

const BencodeObject *BencodeDocument::GetRoot() const{
	const BencodeObject *root;

	if (!this->root.empty()){
		root = this->root.front().get();
	}
	else{
		root = nullptr;
	}

	return root;
}

BencodeObject *BencodeDocument::GetRoot(){
	BencodeObject *root;

	if (!this->root.empty()){
		root = this->root.front().get();
	}
	else{
		root = nullptr;
	}

	return root;
}

std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> BencodeDocument::GetRootListReference(){
	std::reference_wrapper<std::list<std::unique_ptr<BencodeObject>>> ref(this->root);
	return ref;
}

bool BencodeDocument::Parse(const void *data, size_t size){
	ParseState parseState;

	parseState.data = static_cast<const uint8_t *>(data);
	parseState.size = size;
	parseState.i = 0;

	struct st_type{
		std::unique_ptr<BencodeObject> *obj;
	};

	CO3_BEGIN(this->coParse, st_type);

	while (parseState.i < size){
		this->root.push_back(std::unique_ptr<BencodeObject>());
		st->obj = &this->root.back();

		CO3_IN(this->coParse, st_type, this->ParseField(this->coParse, parseState, st->obj));
	}

	CO3_END(this->coParse);
}

bool BencodeDocument::ParseField(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res){
	struct st_type{
		int tmp;
	};

	CO3_BEGIN(co, st_type);

	switch (parseState.data[parseState.i]){
	case 'd':{
		CO3_IN(co, st_type, this->ParseDictionary(co, parseState, res));
		break;
	}
	case 'l':{
		CO3_IN(co, st_type, this->ParseList(co, parseState, res));
		break;
	}
	case 'i':{
		CO3_IN(co, st_type, this->ParseInteger(co, parseState, res));
		break;
	}
	default:{
		if (isdigit(parseState.data[parseState.i])){
			CO3_IN(co, st_type, this->ParseString(co, parseState, res));
		}
		else{
			throw std::exception("unknown type");
		}
		break;
	}
	}

	CO3_END(co);
}

bool BencodeDocument::ParseInteger(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res){
	struct st_type{
		BencodeInt *integer;
		std::string content;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	(*res) = std::unique_ptr<BencodeObject>(new BencodeInt());
	st->integer = (*res)->AsInteger();
	st->haveEnd = false;

	(*res)->SetStart(this->length);
	(*res)->SetContentStart(this->length + 1);

	this->length++;
	parseState.i++;

	while (!st->haveEnd){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == 'e'){
			st->haveEnd = true;
		}
		else{
			st->content.push_back(parseState.data[parseState.i]);
		}

		this->length++;
		parseState.i++;
	}

	(*res)->SetLength(this->length - (*res)->GetStart());
	(*res)->SetContentLength((this->length - 1) - (*res)->GetContentStart());

	st->integer->SetValue(std::stoll(st->content));

	CO3_END(co);
}

bool BencodeDocument::ParseString(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res){
	struct st_type{
		BencodeString *string;
		int i;
		int lengthVal;
		std::string length;
		std::string content;
	};

	CO3_BEGIN(co, st_type);

	(*res) = std::unique_ptr<BencodeObject>(new BencodeString());
	st->string = (*res)->AsString();
	(*res)->SetStart(this->length);

	// parse length
	while (true){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == ':'){
			this->length++;
			parseState.i++;
			break;
		}

		st->length.push_back(parseState.data[parseState.i]);
		this->length++;
		parseState.i++;
	}

	(*res)->SetContentStart(this->length);
	st->lengthVal = std::stoi(st->length);

	for (st->i = 0; st->i < st->lengthVal; st->i++){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		st->content.push_back(parseState.data[parseState.i]);
		this->length++;
		parseState.i++;
	}

	(*res)->SetLength(this->length - (*res)->GetStart());
	(*res)->SetContentLength(this->length - (*res)->GetContentStart());

	st->string->SetValue(st->content);

	CO3_END(co);
}

bool BencodeDocument::ParseDictionary(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res){
	struct st_type{
		std::map<std::string, std::unique_ptr<BencodeObject>> *dict;
		std::unique_ptr<BencodeObject> key;
		std::map<std::string, std::unique_ptr<BencodeObject>>::iterator valueIt;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	(*res) = std::unique_ptr<BencodeObject>(new BencodeDictionary);
	st->haveEnd = false;
	st->dict = (*res)->AsDictionary()->GetPointer();

	(*res)->SetStart(this->length);
	(*res)->SetContentStart(this->length + 1);

	this->length++;
	parseState.i++;

	while (!st->haveEnd){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == 'e'){
			st->haveEnd = true;
			this->length++;
			parseState.i++;
		}
		else{
			CO3_IN(co, st_type, this->ParseField(co, parseState, &st->key));

			if (st->key->GetType() != BencodeType::String){
				throw std::exception("bad key type");
			}

			auto keyTmp = st->key->AsString();
			st->valueIt = st->dict->insert(std::make_pair(keyTmp->GetValue(), std::unique_ptr<BencodeObject>())).first;
			st->key.reset();

			CO3_IN(co, st_type, this->ParseField(co, parseState, &st->valueIt->second));
		}
	}

	(*res)->SetLength(this->length - (*res)->GetStart());
	(*res)->SetContentLength((this->length - 1) - (*res)->GetContentStart());

	CO3_END(co);
}

bool BencodeDocument::ParseList(coroutine3 &co, ParseState &parseState, std::unique_ptr<BencodeObject> *res){
	struct st_type{
		std::list<std::unique_ptr<BencodeObject>> *list;
		std::unique_ptr<BencodeObject> *value;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	(*res) = std::unique_ptr<BencodeObject>(new BencodeList);
	st->haveEnd = false;
	st->list = (*res)->AsList()->GetPointer();

	(*res)->SetStart(this->length);
	(*res)->SetContentStart(this->length + 1);

	this->length++;
	parseState.i++;

	while (!st->haveEnd){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == 'e'){
			st->haveEnd = true;
			this->length++;
			parseState.i++;
		}
		else{
			st->list->push_back(std::unique_ptr<BencodeObject>());
			st->value = &st->list->back();

			CO3_IN(co, st_type, this->ParseField(co, parseState, st->value));
		}
	}

	(*res)->SetLength(this->length - (*res)->GetStart());
	(*res)->SetContentLength((this->length - 1) - (*res)->GetContentStart());

	CO3_END(co);
}