#include "pch.h"
#include "Class1.h"

#include <libhelpers\H.h>
#include <libhelpers\VectorIBuffer.h>

using namespace TorrentLib;
using namespace Platform;

Class1::Class1()
{
}

Windows::Foundation::IAsyncAction ^Class1::ParseMetainfoFile(Windows::Storage::Streams::IRandomAccessStream ^stream){
	return concurrency::create_async([=](){
		Microsoft::WRL::ComPtr<VectorIBuffer> readBuffer, parseBuffer;

		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&readBuffer);
		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&parseBuffer);

		readBuffer->SetSize(1024 * 8);
		parseBuffer->SetSize(1024 * 8);

		H::System::PerformSyncThrow(stream->ReadAsync(readBuffer->AsIBuffer(), static_cast<uint32_t>(readBuffer->GetSize()), Windows::Storage::Streams::InputStreamOptions::None));
		std::swap(readBuffer, parseBuffer);

		while (parseBuffer->GetUsedSize() != 0){
			readBuffer->put_Length(0);
			auto readTask = stream->ReadAsync(readBuffer->AsIBuffer(), static_cast<uint32_t>(readBuffer->GetSize()), Windows::Storage::Streams::InputStreamOptions::None);

			// parse data in parseBuffer...

			H::System::PerformSyncThrow(readTask);
			std::swap(readBuffer, parseBuffer);
		}
	});
}