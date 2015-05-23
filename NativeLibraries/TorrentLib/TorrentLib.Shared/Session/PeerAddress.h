#pragma once

#include <string>
#include <cstdint>

class PeerAddress{
public:
	PeerAddress(const std::string &ip, uint16_t port);
	~PeerAddress();

	const std::string &GetIp() const;
	const uint16_t GetPort() const;
private:
	std::string ip;
	uint16_t port;
};