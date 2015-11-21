#pragma once
#include "Page.h"
#include "TableScheme.h"
#include <vector>

namespace zdb
{
	class DbScheme : public Page
	{
	public:
		explicit DbScheme(utils::File* file, zint32 pageNumber);
		~DbScheme();

		void AddTable(zint32 pageNumber, const zchar* name, const std::vector<DbColumn>& columns);

	private:
		std::vector<TableScheme> tables;
	};
}
