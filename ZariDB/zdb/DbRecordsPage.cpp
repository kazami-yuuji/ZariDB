#include "DbRecordsPage.h"
#include "Database.h"
#include <assert.h>

zdb::DbRecordsPage::DbRecordsPage(utils::File* file, zint32 pageNumber, 
	bool create, Database* db)
	: Page(file, pageNumber, DbPageType::Records, create, db)
{
	nextScheme = nullptr;
	initialIteratorOffset = pageOffset + FIRST_FREE_BYTE_INITIAL_FROM_PAGE_HEAD;
	iteratorOffset = initialIteratorOffset;
	iterator = this;
	if (nextPageOffset != 0)
	{
		assert(create == false);
		nextScheme = new DbRecordsPage(file, static_cast<zint32>((nextPageOffset - 4) / PAGE_SIZE),
			false, db);
	}
}

zdb::DbRecordsPage::~DbRecordsPage()
{

}



void zdb::DbRecordsPage::Insert(Tuple& tuple, const std::vector<DbColumn>& columns, const std::vector<zchar*>& columnNames)
{
	std::vector<DbValue> toInsert;
	for (auto iterator = columns.begin(); iterator != columns.end(); ++iterator)
	{
		auto found = false;
		for (auto i = 0; i < columnNames.size(); i++)
		{
			if (!lstrcmpW(iterator->name, columnNames[i]))
			{
				found = true;
				toInsert.push_back(tuple.values[i]);
				break;
			}
		}
		if (!found)
		{
			toInsert.push_back(DbValue());
		}
	}
	for (auto iterator = toInsert.begin(); iterator != toInsert.end(); ++iterator)
	{
		int dataSize = iterator->size + sizeof(iterator->size);
		if (dataSize > freeBytesCount)
		{
			if (nextScheme == nullptr)
			{
				auto nextPageNumber = db->GetNextPageNumber();
				nextScheme = new DbRecordsPage(file, nextPageNumber, true, db);
				SetNextPage(nextPageNumber);
			}
			nextScheme->Insert(iterator, toInsert);
			break;
		}
		file->Seek(firstFreeByteOffset);
		file->Write(&iterator->size, sizeof(iterator->size));
		file->Write(&iterator->data, iterator->size);
		RewriteFreeBytesValues(dataSize);
	}
}

zdb::Tuple* zdb::DbRecordsPage::GetNextRecord(const std::vector<DbColumn>& columns)
{
	if (iterator == nullptr)
	{
		iteratorOffset = initialIteratorOffset;
		iterator = this;
		return nullptr;
	}
	auto tuple = new Tuple();
	for (auto i = 0; i < columns.size(); i++)
	{
		if (iterator->iteratorOffset != iterator->firstFreeByteOffset)
		{
			file->Seek(iterator->iteratorOffset);
			zint32 size;
			file->Read(&size, sizeof(size));
			DbValue value;
			value.size = size;
			if (size != 0)
			{
				auto data = HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, size);
				file->Read(&data, size);
				value.data = data;
			}
			else
				value.data = nullptr;
			tuple->values.push_back(value);
			iterator->iteratorOffset = file->Tell();
		}
		else
		{
			iterator = iterator->nextScheme;
			iteratorOffset = initialIteratorOffset;
		}
	}
	return tuple;
}

void zdb::DbRecordsPage::Insert(std::vector<DbValue>::const_iterator& dbValuesIterator, const std::vector<DbValue>& dbValues)
{
	for (auto iterator = dbValuesIterator; iterator != dbValues.end(); ++iterator)
	{
		int dataSize = iterator->size + sizeof(iterator->size);
		if (dataSize > freeBytesCount)
		{
			if (nextScheme == nullptr)
			{
				auto nextPageNumber = db->GetNextPageNumber();
				nextScheme = new DbRecordsPage(file, nextPageNumber, true, db);
				SetNextPage(nextPageNumber);
			}
			nextScheme->Insert(iterator, dbValues);
			break;
		}
		file->Seek(firstFreeByteOffset);
		file->Write(&iterator->size, sizeof(iterator->size));
		file->Write(&iterator->data, iterator->size);
		RewriteFreeBytesValues(dataSize);
	}
}