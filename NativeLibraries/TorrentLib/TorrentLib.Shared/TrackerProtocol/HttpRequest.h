#pragma once

#include <wrl.h>
#include <MsXml6.h>
#include <string>
#include <future>

class HttpRequest : public Microsoft::WRL::RuntimeClass <
	Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>,
	IXMLHTTPRequest2Callback > {
public:
	HRESULT RuntimeClassInitialize(const std::wstring &url);

	std::vector<uint8_t> GetResult();

	// IXMLHTTPRequest2Callback
	STDMETHOD(OnRedirect(
		/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
		/* [string][in] */ __RPC__in_string const WCHAR *pwszRedirectUrl)) override;

	STDMETHOD(OnHeadersAvailable(
		/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
		/* [in] */ DWORD dwStatus,
		/* [string][in] */ __RPC__in_string const WCHAR *pwszStatus)) override;

	STDMETHOD(OnDataAvailable(
		/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
		/* [in] */ __RPC__in_opt ISequentialStream *pResponseStream)) override;

	STDMETHOD(OnResponseReceived(
		/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
		/* [in] */ __RPC__in_opt ISequentialStream *pResponseStream)) override;

	STDMETHOD(OnError(
		/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
		/* [in] */ HRESULT hrError)) override;
private:
	Microsoft::WRL::ComPtr<IXMLHTTPRequest2> request;
	std::promise<std::vector<uint8_t>> prom;
};