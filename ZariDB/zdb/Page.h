#pragma once
#include "DbPageType.h"
#include "../utils/File.h"

namespace zdb
{
	class Page
	{
	public:
		explicit Page(utils::File* file, zint32 pageNumber, DbPageType pageType)
		{
			this->file = file;
			this->pageType = pageType;
			this->nextPageOffset = 0;
			this->freeBytesCount = PAGE_FREE_BYTES_INITIAL;
			pageOffset = sizeof(zchara) * 4 + static_cast<zint64>(pageNumber) * PAGE_SIZE;
			this->firstFreeByteOffset = pageOffset + FIRST_FREE_BYTE_INITIAL_FROM_PAGE_HEAD;
			hHeap = GetProcessHeap();
			auto rawZeros = static_cast<zbyte*>(HeapAlloc(hHeap, 
				HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, sizeof(zbyte) * PAGE_FREE_BYTES_INITIAL));
			file->Seek(pageOffset);
			file->Write(&this->pageType, sizeof(this->pageType));
			file->Write(&this->nextPageOffset, sizeof(this->nextPageOffset));
			freeBytesCountOffset = file->Tell();
			file->Write(&this->freeBytesCount, sizeof(this->freeBytesCount));
			file->Write(&this->firstFreeByteOffset, sizeof(this->firstFreeByteOffset));
			file->Write(rawZeros, PAGE_FREE_BYTES_INITIAL);
			HeapFree(hHeap, NULL, rawZeros);
		}

		zint64 GetPageOffset() const
		{ return pageOffset; }

		virtual ~Page() {}

	protected:
		utils::File* file;

		zint64 pageOffset;
		zint64 freeBytesCountOffset;
		DbPageType pageType;
		zint64 nextPageOffset;
		zint32 freeBytesCount;
		zint64 firstFreeByteOffset;

		HANDLE hHeap;

		void RewriteFreeBytesValues(zint32 dataSize)
		{
			file->Seek(freeBytesCountOffset);
			firstFreeByteOffset += dataSize;
			freeBytesCount -= dataSize;
			file->Write(&this->freeBytesCount, sizeof(this->freeBytesCount));
			file->Write(&this->firstFreeByteOffset, sizeof(this->firstFreeByteOffset));
		}

		static const zint32 PAGE_SIZE = 8192;
		static const zint32 PAGE_FREE_BYTES_INITIAL = PAGE_SIZE - sizeof(zint32) 
			- sizeof(DbPageType) - sizeof(zint64) * 2;
		static const zint32 FIRST_FREE_BYTE_INITIAL_FROM_PAGE_HEAD = PAGE_SIZE - PAGE_FREE_BYTES_INITIAL;
	};
}