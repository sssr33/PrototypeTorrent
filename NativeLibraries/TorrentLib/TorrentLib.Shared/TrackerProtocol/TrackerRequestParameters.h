#pragma once
#include "..\Session\Sha1Hash.h"
#include "..\Session\PeerId.h"

#include <string>
#include <cstdint>

enum class TrackerRequestEvent{
	Empty,
	Started,
	Completed,
	Stopped
};

class TrackerRequestParameters{
public:
	TrackerRequestParameters();

	const Sha1Hash &GetInfoHash() const;
	void SetInfoHash(const Sha1Hash &v);

	const PeerId &GetPeerId() const;
	void SetPeerId(const PeerId &v);

	const std::string &GetOptionalIp() const;
	void SetOptionalIp(const std::string &v);

	uint16_t GetPort() const;
	void SetPort(uint16_t v);

	uint64_t GetUploaded() const;
	void SetUploaded(uint64_t v);

	uint64_t GetDownloaded() const;
	void SetDownloaded(uint64_t v);

	uint64_t GetLeft() const;
	void SetLeft(uint64_t v);

	TrackerRequestEvent GetRequestEvent() const;
	void SetRequestEvent(TrackerRequestEvent v);

	bool IsCompactResponse() const;
	void SetCompactResponse(bool v);
private:
	Sha1Hash infoHash;
	PeerId peerId;
	std::string optionalIp;
	uint16_t port;
	uint64_t uploaded;
	uint64_t downloaded;
	uint64_t left;
	TrackerRequestEvent requestEvent;
	bool compactResponse;
};