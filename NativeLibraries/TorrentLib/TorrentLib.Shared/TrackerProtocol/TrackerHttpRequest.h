#pragma once
#include "HttpRequest.h"
#include "TrackerRequestParameters.h"
#include "TrackerResponse.h"

class TrackerHttpRequest{
public:
	TrackerHttpRequest();
	~TrackerHttpRequest();

	void Send(const std::wstring &url, const TrackerRequestParameters &params);
	TrackerResponse Receive();

private:
	Microsoft::WRL::ComPtr<HttpRequest> httpRequest;

	static std::wstring CreateAnnounceURL(const std::wstring &url, const TrackerRequestParameters &params);
};