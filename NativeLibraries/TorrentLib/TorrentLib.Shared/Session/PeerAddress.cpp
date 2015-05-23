#include "pch.h"
#include "PeerAddress.h"

PeerAddress::PeerAddress(const std::string &ip, uint16_t port)
	: ip(ip), port(port){
}

PeerAddress::~PeerAddress(){
}

const std::string &PeerAddress::GetIp() const{
	return this->ip;
}

const uint16_t PeerAddress::GetPort() const{
	return this->port;
}