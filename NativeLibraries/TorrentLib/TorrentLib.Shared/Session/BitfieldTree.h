#pragma once
#include <libhelpers/bit_view.h>

#include <cstdint>
#include <vector>

class BitfieldTree{
	// see description in .cpp
	static const int32_t k16[16];
public:
	BitfieldTree(uint32_t level0Count);
	~BitfieldTree();

	uint32_t LevelCount() const;
	uint32_t ElementCount(uint32_t level) const;

	static uint32_t RoundPOT(uint32_t v);
	static uint32_t Msb32(uint32_t v);
	static int32_t GetK(uint32_t level0Count);
	// levelCount - element count at some tree level
	static uint32_t CountBitTreeElements(uint32_t levelCount);
	static uint32_t LevelCount(uint32_t treeElementCount);
	static uint32_t ElementCount(uint32_t treeSize, int32_t k, uint32_t level);
	static uint32_t LevelOffset();
private:
	int32_t k;
	std::vector<uint8_t> data;
	bit_view<big_endian_block_order> bits;
};