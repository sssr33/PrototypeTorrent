#include "pch.h"
#include "Class1.h"

#include <libhelpers\VectorIBuffer.h>

using namespace TorrentLib;
using namespace Platform;

Class1::Class1()
{
}

Windows::Foundation::IAsyncAction ^Class1::ParseMetainfoFile(Windows::Storage::Streams::IRandomAccessStream ^stream){
	return concurrency::create_async([=](){
		Microsoft::WRL::ComPtr<VectorIBuffer> readBuf[2];

		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&readBuf[0]);
		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&readBuf[1]);


	});
}