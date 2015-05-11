#pragma once

#include <cstdint>
#include <array>

class PeerId : public std::array<uint8_t, 20>{
public:
	PeerId(bool random = true);
	~PeerId();
};