#include "pch.h"
#include "CompactPeerData.h"

#include <libhelpers/H.h>
#include <sstream>

CompactPeerData::CompactPeerData()
	: ip(0), port(0){
}

CompactPeerData::~CompactPeerData(){
}

std::string CompactPeerData::GetIp() const{
	std::stringstream ip;
	const uint8_t *ipData = reinterpret_cast<const uint8_t *>(&this->ip);

	ip << 
		static_cast<uint32_t>(ipData[0]) << '.' << 
		static_cast<uint32_t>(ipData[1]) << '.' << 
		static_cast<uint32_t>(ipData[2]) << '.' << 
		static_cast<uint32_t>(ipData[3]);

	return ip.str();
}

uint16_t CompactPeerData::GetPort() const{
	uint16_t port = H::Data::SwapBytes(this->port);
	return port;
}