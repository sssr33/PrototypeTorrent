#pragma once

namespace TorrentLib{
	public ref class Class1 sealed{
	public:
		Class1();

		static Windows::Foundation::IAsyncAction ^ParseMetainfoFile(Windows::Storage::Streams::IRandomAccessStream ^stream);
	};
}
