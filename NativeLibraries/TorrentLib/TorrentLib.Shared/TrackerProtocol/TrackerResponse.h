#pragma once
#include "TrackerResponsePeerData.h"

#include <vector>

class TrackerResponseInitData{
public:
	int64_t complete;
	int64_t incomplete;
	int64_t interval;
	std::vector<TrackerResponsePeerData> peers;

	TrackerResponseInitData();
};

class TrackerResponse{
public:
	TrackerResponse(TrackerResponseInitData &&data);
	~TrackerResponse();

	int64_t GetComplete() const;
	int64_t GetIncomplete() const;
	int64_t GetInterval() const;
	const std::vector<TrackerResponsePeerData> &GetPeers() const;
private:
	TrackerResponseInitData data;
};