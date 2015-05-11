#pragma once
#include "BencodeType.h"

class BencodeDictionary;
class BencodeList;
class BencodeString;
class BencodeInt;

class BencodeObject{
public:
	BencodeObject();
	virtual ~BencodeObject();

	int GetStart() const;
	void SetStart(int v);

	int GetContentStart() const;
	void SetContentStart(int v);

	int GetLength() const;
	void SetLength(int v);

	int GetContentLength() const;
	void SetContentLength(int v);

	BencodeType GetType() const;

	BencodeDictionary *AsDictionary();
	const BencodeDictionary *AsDictionary() const;

	BencodeList *AsList();
	const BencodeList *AsList() const;

	BencodeString *AsString();
	const BencodeString *AsString() const;

	BencodeInt *AsInteger();
	const BencodeInt *AsInteger() const;
protected:
	void SetType(BencodeType v);
private:
	int start, contentStart;
	int length, contentLength;
	BencodeType type;
};

