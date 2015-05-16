#include "pch.h"
#include "TrackerRequestParameters.h"

TrackerRequestParameters::TrackerRequestParameters()
	: port(0), uploaded(0), downloaded(0), left(0),
	requestEvent(TrackerRequestEvent::Empty), compactResponse(true){
}

const Sha1Hash &TrackerRequestParameters::GetInfoHash() const{
	return this->infoHash;
}

void TrackerRequestParameters::SetInfoHash(const Sha1Hash &v){
	this->infoHash = v;
}

const PeerId &TrackerRequestParameters::GetPeerId() const{
	return this->peerId;
}

void TrackerRequestParameters::SetPeerId(const PeerId &v){
	this->peerId = v;
}

const std::string &TrackerRequestParameters::GetOptionalIp() const{
	return this->optionalIp;
}

void TrackerRequestParameters::SetOptionalIp(const std::string &v){
	this->optionalIp = v;
}

uint16_t TrackerRequestParameters::GetPort() const{
	return this->port;
}

void TrackerRequestParameters::SetPort(uint16_t v){
	this->port = v;
}

uint64_t TrackerRequestParameters::GetUploaded() const{
	return this->uploaded;
}

void TrackerRequestParameters::SetUploaded(uint64_t v){
	this->uploaded = v;
}

uint64_t TrackerRequestParameters::GetDownloaded() const{
	return this->downloaded;
}

void TrackerRequestParameters::SetDownloaded(uint64_t v){
	this->downloaded = v;
}

uint64_t TrackerRequestParameters::GetLeft() const{
	return this->left;
}

void TrackerRequestParameters::SetLeft(uint64_t v){
	this->left = v;
}

TrackerRequestEvent TrackerRequestParameters::GetRequestEvent() const{
	return this->requestEvent;
}

void TrackerRequestParameters::SetRequestEvent(TrackerRequestEvent v){
	this->requestEvent = v;
}

bool TrackerRequestParameters::IsCompactResponse() const{
	return this->compactResponse;
}

void TrackerRequestParameters::SetCompactResponse(bool v){
	this->compactResponse = v;
}