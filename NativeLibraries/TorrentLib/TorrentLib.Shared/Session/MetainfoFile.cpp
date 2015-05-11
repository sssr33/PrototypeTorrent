#include "pch.h"
#include "MetainfoFile.h"

MetainfoFile::MetainfoFile(const BencodeDocument &doc, const void *infoHashData, size_t infoHashSize){
	auto tmpField = doc.GetRoot();

	if (tmpField->GetType() != BencodeType::Dictionary){
		throw std::exception("bad root type");
	}

	auto &rootDict = tmpField->AsDictionary()->GetValue();

	this->ParseTrackerUrl(rootDict);

	auto finded = rootDict.find("info");

	if (finded != rootDict.end()){
		auto tmpInfo = finded->second.get();

		if (tmpInfo->GetType() == BencodeType::Dictionary){
			auto &infoDict = tmpInfo->AsDictionary()->GetValue();

			this->ParseFilePath(infoDict);
			this->ParsePieces(infoDict);
		}
		else{
			throw std::exception("bad info type");
		}
	}
	else{
		throw std::exception("no info");
	}

	if (infoHashSize != this->infoHash.size()){
		throw std::exception("bad hash size");
	}

	std::memcpy(this->infoHash.data(), infoHashData, infoHashSize);
}

MetainfoFile::~MetainfoFile(){
}

void MetainfoFile::ParseTrackerUrl(const std::map<std::string, std::unique_ptr<BencodeObject>> &rootDict){
	auto announceTmp = rootDict.find("announce");
	auto announceListTmp = rootDict.find("announce-list");

	if (announceTmp != rootDict.end()){
		this->ParseTrackerUrlList(announceTmp->second.get());
	}

	if (announceListTmp != rootDict.end()){
		this->ParseTrackerUrlList(announceListTmp->second.get());
	}
}

void MetainfoFile::ParseTrackerUrlList(const BencodeObject *obj){
	if (obj->GetType() == BencodeType::String){
		auto &tmpStr = obj->AsString()->GetValue();

		this->trackerUrl.push_back(tmpStr);
	}
	else if (obj->GetType() == BencodeType::List){
		auto &tmpLst = obj->AsList()->GetValue();

		for (auto &i : tmpLst){
			this->ParseTrackerUrlList(i.get());
		}
	}
	else{
		throw std::exception("bad url type");
	}
}

void MetainfoFile::ParseFilePath(const std::map<std::string, std::unique_ptr<BencodeObject>> &infoDict){
	auto nameFind = infoDict.find("name");
	auto lengthFind = infoDict.find("length");
	auto filesFind = infoDict.find("files");

	if (nameFind != infoDict.end()){
		FileInfo info;
		auto nameTmp = nameFind->second.get();

		if (nameTmp->GetType() != BencodeType::String){
			throw std::exception("bad name type");
		}

		if (filesFind != infoDict.end()){
			auto filesTmp = filesFind->second.get();

			if (filesTmp->GetType() != BencodeType::List){
				throw std::exception("bad files type");
			}

			auto &files = filesTmp->AsList()->GetValue();

			for (auto &i : files){
				if (i->GetType() != BencodeType::Dictionary){
					throw std::exception("bad files entry type");
				}

				auto &entry = i->AsDictionary()->GetValue();

				auto entryLengthFind = entry.find("length");
				auto entryPathFind = entry.find("path");

				if (entryLengthFind == entry.end() || entryPathFind == entry.end()){
					throw std::exception("bad files entry content");
				}

				auto entryLengthTmp = entryLengthFind->second.get();
				auto entryPathTmp = entryPathFind->second.get();

				if (entryLengthTmp->GetType() != BencodeType::Integer){
					throw std::exception("bad length type");
				}

				info.FilePath = nameTmp->AsString()->GetValue() + this->GetPath(entryPathTmp);
				info.FileSize = static_cast<uint64_t>(entryLengthTmp->AsInteger()->GetValue());

				this->fileInfo.push_back(info);
			}
		}
		else if (lengthFind != infoDict.end()){
			auto lengthTmp = lengthFind->second.get();

			if (lengthTmp->GetType() != BencodeType::Integer){
				throw std::exception("bad length type");
			}

			info.FilePath = nameTmp->AsString()->GetValue();
			info.FileSize = static_cast<uint64_t>(lengthTmp->AsInteger()->GetValue());

			this->fileInfo.push_back(info);
		}
		else{
			throw std::exception("invalid content");
		}
	}
	else{
		throw std::exception("no name field");
	}
}

std::string MetainfoFile::GetPath(const BencodeObject *obj){
	std::string path;

	if (obj->GetType() == BencodeType::String){
		path = obj->AsString()->GetValue();
	}
	else if (obj->GetType() == BencodeType::List){
		auto &tmpLst = obj->AsList()->GetValue();

		for (auto &i : tmpLst){
			path += "\\" + this->GetPath(i.get());
		}
	}
	else{
		throw std::exception("bad path type");
	}

	return path;
}

void MetainfoFile::ParsePieces(const std::map<std::string, std::unique_ptr<BencodeObject>> &infoDict){
	auto pieceLengthFind = infoDict.find("piece length");
	auto piecesFind = infoDict.find("pieces");

	if (pieceLengthFind == infoDict.end() || piecesFind == infoDict.end()){
		throw std::exception("no piece info");
	}

	auto pieceLengthTmp = pieceLengthFind->second.get();
	auto piecesTmp = piecesFind->second.get();

	if (pieceLengthTmp->GetType() != BencodeType::Integer || piecesTmp->GetType() != BencodeType::String){
		throw std::exception("bad piece info data types");
	}

	this->pieceSize = static_cast<uint64_t>(pieceLengthTmp->AsInteger()->GetValue());

	Sha1Hash hashVal;
	auto &pieces = piecesTmp->AsString()->GetValue();

	if (pieces.size() % hashVal.size() != 0){
		throw std::exception("bad piece count");
	}

	auto pieceCount = pieces.size() / hashVal.size();
	this->pieces.reserve(pieceCount);

	for (size_t i = 0; i < pieceCount; i++){
		std::memcpy(hashVal.data(), pieces.data() + i * hashVal.size(), hashVal.size());
		this->pieces.push_back(hashVal);
	}
}