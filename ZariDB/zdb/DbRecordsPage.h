#pragma once
#include <vector>
#include "../utils/types.h"
#include "../utils/File.h"
#include "DbStructures.h"
#include "Page.h"

namespace zdb
{
	class DbRecordsPage : public Page
	{
	public:
		DbRecordsPage(utils::File* file, zint32 pageNumber, bool create, Database* db);
		~DbRecordsPage();

		void Insert(Tuple& tuple, const std::vector<DbColumn>& columns, const std::vector<zchar*>& columnNames);
		Tuple* GetNextRecord(const std::vector<DbColumn>& columns);

	private:
		void Insert(std::vector<DbValue>::const_iterator& dbValuesIterator, const std::vector<DbValue>& dbValues);

		DbRecordsPage* nextScheme;
		DbRecordsPage* iterator;
		zint64 iteratorOffset;
		zint64 initialIteratorOffset;
	};
}