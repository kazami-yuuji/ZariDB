#pragma once
#include "Page.h"

namespace zdb
{
	class DbRecordsPage : public Page
	{
	public:
		DbRecordsPage(utils::File* file, zint32 pageNumber, zint32 schemePage);
		~DbRecordsPage();

	private:
		zint32 schemePage;
	};
}