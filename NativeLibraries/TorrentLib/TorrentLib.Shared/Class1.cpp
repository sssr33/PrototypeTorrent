#include "pch.h"
#include "Class1.h"
#include "Bencode\BencodeDocument.h"
#include "Session\MetainfoFile.h"
#include "TrackerProtocol\TrackerHttpRequest.h"
#include "Session\BitfieldTree.h"

#include <memory>
#include <libhelpers\H.h>
#include <libhelpers\VectorIBuffer.h>
#include <libhelpers\ArrayIBuffer.h>

using namespace TorrentLib;
using namespace Platform;

Class1::Class1()
{
}

Windows::Foundation::IAsyncAction ^Class1::ParseMetainfoFile(Windows::Storage::Streams::IRandomAccessStream ^stream){
	return concurrency::create_async([=](){

		std::vector<uint32_t> elc;
		uint32_t level0Cnt = 117;
		auto treeSize = BitfieldTree::CountBitTreeElements(level0Cnt);
		auto levelCount = BitfieldTree::LevelCount(treeSize);
		auto k = BitfieldTree::GetK(level0Cnt);

		for (uint32_t i = 0; i < levelCount; i++){
			elc.push_back(BitfieldTree::ElementCount(treeSize, k, i));
		}

		BencodeDictionary *infoDict = nullptr;
		BencodeDocument doc;
		Microsoft::WRL::ComPtr<VectorIBuffer> readBuffer, parseBuffer;
		Microsoft::WRL::ComPtr<ArrayIBuffer> hashBuffer;
		auto hashProvider = Windows::Security::Cryptography::Core::HashAlgorithmProvider::OpenAlgorithm("SHA1");
		auto sha1 = hashProvider->CreateHash();

		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&readBuffer);
		Microsoft::WRL::MakeAndInitialize<VectorIBuffer>(&parseBuffer);
		Microsoft::WRL::MakeAndInitialize<ArrayIBuffer>(&hashBuffer);

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

				if (!infoDict && doc.GetRoot()){
					if (doc.GetRoot()->GetType() != BencodeType::Dictionary){
						throw std::exception("bad root type");
					}

					auto rootTmp = doc.GetRoot()->AsDictionary();
					auto &rootDict = rootTmp->GetValue();

					auto finded = rootDict.find("info");

					if (finded != rootDict.end() && finded->second){
						if (finded->second->GetType() != BencodeType::Dictionary){
							throw std::exception("bad root type");
						}

						infoDict = finded->second->AsDictionary();
					}
				}
				
				if (infoDict && infoDict->GetStart() > -1){
					auto bufferStart = static_cast<int>(doc.GetLength() - parseBuffer->GetUsedSize());
					int start = (std::max)(0, infoDict->GetStart() - bufferStart);
					int end;

					if (infoDict->GetLength() > -1){
						end = (std::min)(static_cast<int>(parseBuffer->GetUsedSize()), (infoDict->GetStart() + infoDict->GetLength()) - bufferStart);
					}
					else{
						end = static_cast<int>(parseBuffer->GetUsedSize());
					}

					int length = end - start;

					if (length > 0){
						uint8_t *buf = nullptr;

						parseBuffer->Buffer(&buf);
						buf += start;

						hashBuffer->SetBuffer(buf, length, length);

						sha1->Append(hashBuffer->AsIBuffer());
					}
				}

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

		auto hash = sha1->GetValueAndReset();

		Microsoft::WRL::ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(hash));

		Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> bufferByteAccess;
		insp.As(&bufferByteAccess);

		uint8_t *hashBytes = nullptr;
		size_t hashSize = hash->Length;

		bufferByteAccess->Buffer(&hashBytes);

		std::unique_ptr<MetainfoFile> metainfoFile = std::unique_ptr<MetainfoFile>(new MetainfoFile(doc, hashBytes, hashSize));

		auto &infoHash = metainfoFile->GetInfoHash();
		auto &trackers = metainfoFile->GetTrackers();
		PeerId peerId(true);

		for (auto &i : trackers){
			if (i.find("http:") == 0){
				TrackerHttpRequest request;
				TrackerRequestParameters requestParams;
				auto url = H::Text::ConvertUTF8ToWString(i);

				requestParams.SetCompactResponse(true);
				requestParams.SetDownloaded(0);
				requestParams.SetUploaded(0);
				requestParams.SetLeft(metainfoFile->GetTotalSize());
				requestParams.SetPort(6881);
				requestParams.SetRequestEvent(TrackerRequestEvent::Started);
				requestParams.SetInfoHash(infoHash);
				requestParams.SetPeerId(peerId);

				request.Send(url, requestParams);
				auto response = request.Receive();
				int stop = 342;
			}
		}

		int stop = 432;
	});
}