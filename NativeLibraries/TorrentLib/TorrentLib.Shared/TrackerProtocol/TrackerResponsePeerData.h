#pragma once
#include "..\Session\PeerId.h"
#include "..\Session\PeerAddress.h"

class TrackerResponsePeerData{
public:
	TrackerResponsePeerData(const PeerId &id, const PeerAddress &address);
	~TrackerResponsePeerData();

	const PeerId &GetId() const;
	const PeerAddress &GetAddress() const;
private:
	PeerId id;
	PeerAddress address;
};