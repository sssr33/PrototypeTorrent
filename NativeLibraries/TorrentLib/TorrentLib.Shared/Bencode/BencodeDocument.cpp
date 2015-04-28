#include "pch.h"
#include "BencodeDocument.h"

#include <libhelpers\H.h>

BencodeDocument::BencodeDocument(){

}

BencodeDocument::~BencodeDocument(){

}

bool BencodeDocument::Parse(const void *data, size_t size){
	ParseState parseState;

	parseState.data = static_cast<const uint8_t *>(data);
	parseState.size = size;
	parseState.i = 0;

	struct st_type{
		BencodeField field;
	};

	CO3_BEGIN(this->coParse, st_type);

	while (parseState.i < size){
		st->field = BencodeField();
		st->field.integer = st->field.string = st->field.list = st->field.dictionary = false;

		CO3_IN(this->coParse, st_type, this->ParseField(this->coParse, parseState, &st->field));
		this->rootFields.push_back(st->field);
	}

	CO3_END(this->coParse);
}

bool BencodeDocument::ParseField(coroutine3 &co, ParseState &parseState, BencodeField *res){
	struct st_type{
		int tmp;
	};

	CO3_BEGIN(co, st_type);

	switch (parseState.data[parseState.i]){
	case 'd':{
		parseState.i++;
		CO3_IN(co, st_type, this->ParseDictionary(co, parseState, res));
		break;
	}
	case 'l':{
		parseState.i++;
		CO3_IN(co, st_type, this->ParseList(co, parseState, res));
		break;
	}
	case 'i':{
		parseState.i++;
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

bool BencodeDocument::ParseInteger(coroutine3 &co, ParseState &parseState, BencodeField *res){
	struct st_type{
		std::string content;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	st->haveEnd = false;

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

		parseState.i++;
	}

	res->integer = true;
	res->intVal = std::stoi(st->content);

	CO3_END(co);
}

bool BencodeDocument::ParseString(coroutine3 &co, ParseState &parseState, BencodeField *res){
	struct st_type{
		int i;
		int lengthVal;
		std::string length;
		std::string content;
	};

	CO3_BEGIN(co, st_type);

	// parse length
	while (true){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == ':'){
			parseState.i++;
			break;
		}

		st->length.push_back(parseState.data[parseState.i]);
		parseState.i++;
	}

	st->lengthVal = std::stoi(st->length);

	if (st->lengthVal == 0){
		int stop = 24;
	}

	for (st->i = 0; st->i < st->lengthVal; st->i++){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		st->content.push_back(parseState.data[parseState.i]);
		parseState.i++;
	}

	res->string = true;
	res->strVal = st->content;

	CO3_END(co);
}

bool BencodeDocument::ParseDictionary(coroutine3 &co, ParseState &parseState, BencodeField *res){
	struct st_type{
		BencodeField key;
		BencodeField value;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	st->haveEnd = false;
	res->dictionary = true;

	while (!st->haveEnd){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == 'e'){
			st->haveEnd = true;
			parseState.i++;
		}
		else{
			st->key = BencodeField();
			st->key.integer = st->key.string = st->key.list = st->key.dictionary = false;

			CO3_IN(co, st_type, this->ParseField(co, parseState, &st->key));

			if (!st->key.string){
				throw std::exception("bad key type");
			}

			if (st->key.strVal == "pieces"){
				int stop = 432;
			}

			st->value = BencodeField();
			st->value.integer = st->value.string = st->value.list = st->value.dictionary = false;

			CO3_IN(co, st_type, this->ParseField(co, parseState, &st->value));

			if (st->key.strVal == "pieces"){
				int stop = 432;
			}

			res->dictVal.insert(std::make_pair(st->key.strVal, st->value));
		}
	}

	CO3_END(co);
}

bool BencodeDocument::ParseList(coroutine3 &co, ParseState &parseState, BencodeField *res){
	struct st_type{
		BencodeField value;
		bool haveEnd;
	};

	CO3_BEGIN(co, st_type);

	st->haveEnd = false;
	res->list = true;

	while (!st->haveEnd){
		if (parseState.i == parseState.size){
			CO3_OUT(co, st_type);
		}

		if (parseState.data[parseState.i] == 'e'){
			st->haveEnd = true;
			parseState.i++;
		}
		else{
			st->value = BencodeField();
			st->value.integer = st->value.string = st->value.list = st->value.dictionary = false;

			CO3_IN(co, st_type, this->ParseField(co, parseState, &st->value));

			res->listVal.push_back(st->value);
		}
	}

	CO3_END(co);
}