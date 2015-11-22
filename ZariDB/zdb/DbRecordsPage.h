#pragma once
#include "../utils/types.h"
#include "../utils/File.h"
#include "Page.h"

namespace zdb
{
	class DbRecordsPage : public Page
	{
	public:
		DbRecordsPage(utils::File* file, zint32 pageNumber, zint32 schemePage, bool create, Database* db);
		~DbRecordsPage();

	private:
		zint32 schemePage;
	};
}