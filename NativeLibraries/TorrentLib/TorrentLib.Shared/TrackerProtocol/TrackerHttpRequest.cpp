#include "pch.h"
#include "TrackerHttpRequest.h"
#include "CompactPeerData.h"
#include "Bencode/BencodeDocument.h"

#include <libhelpers/H.h>

TrackerHttpRequest::TrackerHttpRequest(){
}

TrackerHttpRequest::~TrackerHttpRequest(){
}

void TrackerHttpRequest::Send(const std::wstring &url, const TrackerRequestParameters &params){
	HRESULT hr = S_OK;
	auto announceUrl = TrackerHttpRequest::CreateAnnounceURL(url, params);

	hr = Microsoft::WRL::MakeAndInitialize<HttpRequest>(&this->httpRequest, announceUrl);
	H::System::ThrowIfFailed(hr);
}

TrackerResponse TrackerHttpRequest::Receive(){
	auto res = this->httpRequest->GetResult();
	BencodeDocument doc;
	TrackerResponseInitData respData;

	doc.Parse(res.data(), res.size());

	auto rootTmp = doc.GetRoot();
	if (!rootTmp){
		throw std::exception("Response is empty.");
	}
	if (rootTmp->GetType() != BencodeType::Dictionary){
		throw std::exception("Bad response root type.");
	}

	auto &rootDict = rootTmp->AsDictionary()->GetValue();
	auto peersFind = rootDict.find("peers");

	if (peersFind == rootDict.end()){
		throw std::exception("No peers data.");
	}
	else{
		// Peers parsing
		auto &peersTmp = peersFind->second;

		if (peersTmp->GetType() == BencodeType::String){
			// compact
			auto &peersData = peersTmp->AsString()->GetValue();
			if (peersData.size() % sizeof(CompactPeerData) != 0){
				throw std::exception("Bad compact peers data size.");
			}

			PeerId emptyId(false);
			auto count = peersData.size() / sizeof(CompactPeerData);
			auto data = reinterpret_cast<const CompactPeerData*>(peersData.data());

			respData.peers.reserve(count);

			for (size_t i = 0; i < count; i++){
				auto &tmp = data[i];
				TrackerResponsePeerData peer(emptyId, PeerAddress(tmp.GetIp(), tmp.GetPort()));

				respData.peers.push_back(std::move(peer));
			}
		}
		else if (peersTmp->GetType() == BencodeType::List){
			// default
			auto &peersList = peersTmp->AsList()->GetValue();
			// TODO check format in other clients

			respData.peers.reserve(peersList.size());

			for (auto &i : peersList){
				PeerId id(false);
				std::string ip;
				uint16_t port;

				if (i->GetType() != BencodeType::Dictionary){
					throw std::exception("Bad peer entry.");
				}
				auto &peerEntry = i->AsDictionary()->GetValue();

				auto ipFind = peerEntry.find("ip");
				if (ipFind == peerEntry.end()){
					throw std::exception("No peer ip.");
				}
				if (ipFind->second->GetType() != BencodeType::String){
					throw std::exception("Bad peer ip type.");
				}
				ip = ipFind->second->AsString()->GetValue();

				auto portFind = peerEntry.find("port");
				if (portFind == peerEntry.end()){
					throw std::exception("No peer port.");
				}
				if (portFind->second->GetType() != BencodeType::Integer){
					throw std::exception("Bad peer port type.");
				}
				// TODO check is network byte order is used
				port = static_cast<uint16_t>(portFind->second->AsInteger()->GetValue());

				auto idFind = peerEntry.find("peer id");
				if (idFind != peerEntry.end() &&
					idFind->second->GetType() == BencodeType::String)
				{
					auto &idStr = idFind->second->AsString()->GetValue();

					if (id.size() == idStr.size()){
						std::memcpy(id.data(), idStr.data(), id.size());
					}
					// else do nothing, peer id is not very important here
				}

				TrackerResponsePeerData peer(id, PeerAddress(ip, port));
				respData.peers.push_back(std::move(peer));
			}
		}
		else{
			throw std::exception("Bad peers type.");
		}
	}

	auto intervalFind = rootDict.find("interval");
	if (intervalFind == rootDict.end()){
		throw std::exception("No interval data.");
	}
	else{
		// Parse interval
		if (intervalFind->second->GetType() != BencodeType::Integer){
			throw std::exception("Bad interval data type.");
		}
		respData.interval = intervalFind->second->AsInteger()->GetValue();
	}

	auto completeFind = rootDict.find("complete");
	if (completeFind != rootDict.end() &&
		completeFind->second->GetType() == BencodeType::Integer)
	{
		respData.complete = completeFind->second->AsInteger()->GetValue();
	}

	auto incompleteFind = rootDict.find("incomplete");
	if (incompleteFind != rootDict.end() &&
		incompleteFind->second->GetType() == BencodeType::Integer)
	{
		respData.incomplete = incompleteFind->second->AsInteger()->GetValue();
	}

	return TrackerResponse(std::move(respData));
}

std::wstring TrackerHttpRequest::CreateAnnounceURL(const std::wstring &url, const TrackerRequestParameters &params){
	std::wstring announceUrl;
	std::wstring infoHash, peerId;
	std::wstring downloaded, uploaded, left;
	std::wstring ip, port;
	std::wstring event_, compact;

	auto &infoHashTmp = params.GetInfoHash();
	auto &peerIdTmp = params.GetPeerId();

	auto tmpHashUriEnc = H::Text::UriEncode(infoHashTmp.data(), infoHashTmp.size());
	auto tmpPeerIdEnc = H::Text::UriEncode(peerIdTmp.data(), peerIdTmp.size());

	infoHash = std::wstring(tmpHashUriEnc.begin(), tmpHashUriEnc.end());
	peerId = std::wstring(tmpPeerIdEnc.begin(), tmpPeerIdEnc.end());

	downloaded = std::to_wstring(params.GetDownloaded());
	uploaded = std::to_wstring(params.GetUploaded());
	left = std::to_wstring(params.GetLeft());

	ip = params.GetOptionalIp();
	port = std::to_wstring(params.GetPort());

	switch (params.GetRequestEvent()){
	case TrackerRequestEvent::Completed:
		event_ = L"completed";
		break;
	case TrackerRequestEvent::Started:
		event_ = L"started";
		break;
	case TrackerRequestEvent::Stopped:
		event_ = L"stopped";
		break;
	case TrackerRequestEvent::Empty:
	default:
		// If not specified, the request is taken to be a regular periodic request
		break;
	}

	//http://stackoverflow.com/questions/17418004/why-does-tracker-server-not-understand-my-request-bittorrent-protocol
	// BEP23 http://www.bittorrent.org/beps/bep_0023.html
	if (params.IsCompactResponse()){
		compact = '1';
	}
	else{
		compact = '0';
	}

	announceUrl = url;
	announceUrl += L"?info_hash=" + infoHash;
	announceUrl += L"&peer_id=" + peerId;
	announceUrl += L"&port=" + port;
	announceUrl += L"&uploaded=" + uploaded;
	announceUrl += L"&downloaded=" + downloaded;
	announceUrl += L"&left=" + left;
	if (!ip.empty()){
		announceUrl += L"&ip=" + ip;
	}
	if (!event_.empty()){
		announceUrl += L"&event=" + event_;
	}
	announceUrl += L"&compact=" + compact;

	return announceUrl;
}