#pragma once

#include <cstdint>
#include <string>

/*
BEP 23 - http://www.bittorrent.org/beps/bep_0023.html
"The first 4 bytes contain the 32-bit ipv4 address. The remaining two bytes contain the port number. Both address and port use network-byte order."
*/
#pragma pack(push, 1) // disable padding
class CompactPeerData{
public:
	CompactPeerData();
	~CompactPeerData();

	std::string GetIp() const;
	uint16_t GetPort() const;
private:
	uint32_t ip;
	uint16_t port;
};
#pragma pack(pop)