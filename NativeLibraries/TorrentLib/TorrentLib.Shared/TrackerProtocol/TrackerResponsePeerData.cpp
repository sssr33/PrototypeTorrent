#include "pch.h"
#include "TrackerResponsePeerData.h"

TrackerResponsePeerData::TrackerResponsePeerData(const PeerId &id, const PeerAddress &address)
	: id(id), address(address){
}

TrackerResponsePeerData::~TrackerResponsePeerData(){
}

const PeerId &TrackerResponsePeerData::GetId() const{
	return this->id;
}

const PeerAddress &TrackerResponsePeerData::GetAddress() const{
	return this->address;
}