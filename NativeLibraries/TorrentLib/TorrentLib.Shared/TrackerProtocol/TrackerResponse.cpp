#include "pch.h"
#include "TrackerResponse.h"

TrackerResponseInitData::TrackerResponseInitData()
	: complete(-1), incomplete(-1), interval(-1){
}




TrackerResponse::TrackerResponse(TrackerResponseInitData &&data)
	: data(std::move(data)){
}

TrackerResponse::~TrackerResponse(){
}

int64_t TrackerResponse::GetComplete() const{
	return this->data.complete;
}

int64_t TrackerResponse::GetIncomplete() const{
	return this->data.incomplete;
}

int64_t TrackerResponse::GetInterval() const{
	return this->data.interval;
}

const std::vector<TrackerResponsePeerData> &TrackerResponse::GetPeers() const{
	return this->data.peers;
}