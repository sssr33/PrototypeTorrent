#include "pch.h"
#include "PeerId.h"

#include <cstdlib>
#include <limits>

PeerId::PeerId(bool random){
	if (random){
		/*
		Currently No ID is name for this client.
		PeerId format:
		-noidvvvv-rrrrrrrrrr
		v - version
		r - random
		*/
		auto data = this->data();
		data[0] = '-';
		data[1] = 'n'; data[2] = 'o'; data[3] = 'i'; data[4] = 'd';
		data[5] = '0'; data[6] = '0'; data[7] = '0'; data[8] = '1';
		data[9] = '-';
		for (int i = 10; i < 20; i++){
			data[i] = std::rand() % std::numeric_limits<uint8_t>::max();
		}
	}
	else{
		this->fill(0);
	}
}

PeerId::~PeerId(){
}