#include "pch.h"
#include "Class1.h"
#include "Bencode\BencodeDocument.h"

#include <libhelpers\H.h>
#include <libhelpers\VectorIBuffer.h>

using namespace TorrentLib;
using namespace Platform;

Class1::Class1()
{
}

Windows::Foundation::IAsyncAction ^Class1::ParseMetainfoFile(Windows::Storage::Streams::IRandomAccessStream ^stream){
	return concurrency::create_async([=](){
		BencodeDocument doc;
		Microsoft::WRL::ComPtr<VectorIBuffer> readBuffer, parseBuffer;

		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&readBuffer);
		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&parseBuffer);

		readBuffer->SetSize(1024 * 8);
		parseBuffer->SetSize(1024 * 8);

		H::System::PerformSyncThrow(stream->ReadAsync(readBuffer->AsIBuffer(), static_cast<uint32_t>(readBuffer->GetSize()), Windows::Storage::Streams::InputStreamOptions::None));
		std::swap(readBuffer, parseBuffer);

		try{
			while (parseBuffer->GetUsedSize() != 0){
				readBuffer->put_Length(0);
				auto readTask = stream->ReadAsync(readBuffer->AsIBuffer(), static_cast<uint32_t>(readBuffer->GetSize()), Windows::Storage::Streams::InputStreamOptions::None);

				auto &parseData = parseBuffer->GetBuffer();
				doc.Parse(parseData.data(), parseBuffer->GetUsedSize());

				H::System::PerformSyncThrow(readTask);
				std::swap(readBuffer, parseBuffer);
			}
		}
		catch (const std::exception &e){
			auto msg = e.what();
			int stop = 423;
		}
		catch (...){
			int stop = 243;
		}

		int stop = 432;
	});
}