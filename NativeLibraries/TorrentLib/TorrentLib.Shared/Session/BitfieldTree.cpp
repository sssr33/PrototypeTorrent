#include "pch.h"
#include "BitfieldTree.h"

/*
If you want to understand this then you need to start from:
- element count for Power Of Two(1,2,4,8,16,32...) == 2 * POT - 1
- then substitute -1 with k and you will get: 2 * elements + k
- then find difference between real_node_count - 2 * elements
- real_node_count:
real_node_count = elements;
while ((elements / 2) > 0 && (elements = (elements / 2) + !!(elements % 2)) > 0){
real_node_count += elements;
}
- then you can find differences for elements = [512...256];
- then group numbers as matrix 16*16 and you will see "progressive-quad-symmetry"
- this algorithm uses k16 acceleration table for finding k inside 16*16 quad at once
which helps reduce iterations, to reduce more you need to increase k16 size from 16 to 32
fill it with valid coefs and accordingly reduce k = k16[(tmpElements & 0xFFFFFFFF) >> 28] iterations
*/
const uint32_t BitfieldTree::k16[16] = {
	0, 1, 1, 2,
	1, 2, 2, 3,
	1, 2, 2, 3,
	2, 3, 3, 4
};

BitfieldTree::BitfieldTree(){
}

BitfieldTree::~BitfieldTree(){
}

uint32_t BitfieldTree::RoundPOT(uint32_t v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

uint32_t BitfieldTree::Msb32(uint32_t v){
	v |= (v >> 1);
	v |= (v >> 2);
	v |= (v >> 4);
	v |= (v >> 8);
	v |= (v >> 16);
	return(v & ~(v >> 1));
}

int32_t BitfieldTree::GetK(uint32_t levelCount){
	int32_t k, tmpElements;

	tmpElements = RoundPOT(levelCount) - levelCount;
	k = k16[(tmpElements & 0xFFFFFFFF) >> 28];
	k += k16[(tmpElements & 0x0FFFFFFF) >> 24];
	k += k16[(tmpElements & 0x00FFFFFF) >> 20];
	k += k16[(tmpElements & 0x000FFFFF) >> 16];
	k += k16[(tmpElements & 0x0000FFFF) >> 12];
	k += k16[(tmpElements & 0x00000FFF) >> 8];
	k += k16[(tmpElements & 0x000000FF) >> 4];
	k += k16[(tmpElements & 0x0000000F)];
	k--;

	return k;
}

uint32_t BitfieldTree::CountBitTreeElements(uint32_t levelCount){
	int32_t k, count, tmpElements;

	tmpElements = RoundPOT(levelCount) - levelCount;
	k = k16[(tmpElements & 0xFFFFFFFF) >> 28];
	k += k16[(tmpElements & 0x0FFFFFFF) >> 24];
	k += k16[(tmpElements & 0x00FFFFFF) >> 20];
	k += k16[(tmpElements & 0x000FFFFF) >> 16];
	k += k16[(tmpElements & 0x0000FFFF) >> 12];
	k += k16[(tmpElements & 0x00000FFF) >> 8];
	k += k16[(tmpElements & 0x000000FF) >> 4];
	k += k16[(tmpElements & 0x0000000F)];
	k--;

	count = 2 * levelCount + k;

	return count;
}

uint32_t BitfieldTree::LevelCount(uint32_t treeElementCount){
	uint32_t count = 0;
	auto tmp = RoundPOT(treeElementCount);

	while (tmp = tmp >> 1){
		count++;
	}

	return count;
}

uint32_t BitfieldTree::ElementCount(uint32_t treeSize, int32_t k, uint32_t level){
	auto count = (treeSize - k + (1 << level) + ((1 << level) - 1)) >> (level + 1);
	return count;
}