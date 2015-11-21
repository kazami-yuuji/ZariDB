#pragma once
#include "DbPageType.h"
#include "../utils/File.h"

namespace zdb
{
	class Page
	{
	public:
		explicit Page(utils::File* file, DbPageType pageType) :
			file(file),
			pageType(pageType),
			nextPageOffset(0),
			freeBytesCount(0),
			firstFreeByteOffset(0)
		{}
		virtual ~Page() = 0;

	protected:
		utils::File* file;

		DbPageType pageType;
		zint64 nextPageOffset;
		zint32 freeBytesCount;
		zint64 firstFreeByteOffset;

		const zint32 PAGE_SIZE = 8192;
	};
}