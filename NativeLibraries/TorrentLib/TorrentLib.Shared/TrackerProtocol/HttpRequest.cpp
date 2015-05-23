#include "pch.h"
#include "HttpRequest.h"

#include <libhelpers/H.h>

HRESULT HttpRequest::RuntimeClassInitialize(const std::wstring &url){
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_FreeThreadedXMLHTTP60,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&this->request));
	if (FAILED(hr)){
		return hr;
	}

	hr = this->request->Open(L"GET",              // Method.
		url.c_str(),            // Url.
		this, // Callback.
		NULL,                // Username.
		NULL,                // Password.
		NULL,                // Proxy username.
		NULL);               // Proxy password.
	if (FAILED(hr)){
		return hr;
	}

	hr = this->request->Send(NULL, 0);
	if (FAILED(hr)){
		return hr;
	}

	return hr;
}

std::vector<uint8_t> HttpRequest::GetResult(){
	auto fut = this->prom.get_future();
	auto res = fut.get();
	return res;
}

STDMETHODIMP HttpRequest::OnRedirect(
	/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
	/* [string][in] */ __RPC__in_string const WCHAR *pwszRedirectUrl){
	HRESULT hr = S_OK;
	return hr;
}

STDMETHODIMP HttpRequest::OnHeadersAvailable(
	/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
	/* [in] */ DWORD dwStatus,
	/* [string][in] */ __RPC__in_string const WCHAR *pwszStatus){
	HRESULT hr = S_OK;
	return hr;
}

STDMETHODIMP HttpRequest::OnDataAvailable(
	/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
	/* [in] */ __RPC__in_opt ISequentialStream *pResponseStream){
	HRESULT hr = S_OK;
	return hr;
}

STDMETHODIMP HttpRequest::OnResponseReceived(
	/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
	/* [in] */ __RPC__in_opt ISequentialStream *pResponseStream)
{
	HRESULT hr = S_OK;
	ULONG readed = 0;
	std::array<uint8_t, 1024 * 4> buffer;
	std::vector<uint8_t> data;

	while (SUCCEEDED(hr = pResponseStream->Read(buffer.data(), buffer.size(), &readed)) && readed > 0){
		data.insert(data.end(), buffer.data(), buffer.data() + readed);
	}

	this->prom.set_value(std::move(data));

	return hr;
}

STDMETHODIMP HttpRequest::OnError(
	/* [in] */ __RPC__in_opt IXMLHTTPRequest2 *pXHR,
	/* [in] */ HRESULT hrError)
{
	HRESULT hr = S_OK;

	auto exception = std::make_exception_ptr(ref new Platform::Exception(hrError));
	this->prom.set_exception(exception);

	return hr;
}