#pragma once
#include "Bencode/BencodeDocument.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <cstdlib>

typedef std::array<uint8_t, 20> Sha1Hash;

struct FileInfo{
	std::string FilePath;
	uint64_t FileSize;
};

class MetainfoFile{
public:
	MetainfoFile(const BencodeDocument &doc, const void *infoHashData, size_t infoHashSize);
	~MetainfoFile();
private:
	Sha1Hash infoHash;
	std::vector<std::string> trackerUrl;
	std::vector<FileInfo> fileInfo;
	std::vector<Sha1Hash> pieces;
	uint64_t pieceSize;

	void ParseTrackerUrl(const std::map<std::string, std::unique_ptr<BencodeObject>> &rootDict);
	void ParseTrackerUrlList(const BencodeObject *obj);
	void ParseFilePath(const std::map<std::string, std::unique_ptr<BencodeObject>> &infoDict);
	std::string GetPath(const BencodeObject *obj);
	void ParsePieces(const std::map<std::string, std::unique_ptr<BencodeObject>> &infoDict);
};