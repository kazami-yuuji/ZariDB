#include "TableScheme.h"
#include "Database.h"

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber,
	const std::vector<DbColumn>& columns, Database* db) :
	Page(file, pageNumber, DbPageType::TableScheme, true, db)
{
	recordsPage = new DbRecordsPage(file, db->GetNextPageNumber(), true, db);
	file->Seek(firstFreeByteOffset);
	auto recordsPageOffset = recordsPage->GetPageOffset();
	file->Write(&recordsPageOffset, sizeof(recordsPageOffset));
	this->columns = columns;
	for (auto iterator = columns.begin(); iterator != columns.end(); ++iterator)
	{
		auto currentDataSize = sizeof(iterator->nameLength) + sizeof(*iterator->name) * iterator->nameLength
			+ sizeof(iterator->dataType);
		if (currentDataSize > PAGE_SIZE)
		{
			throw TooLongColumnNameException();
		}
		if (currentDataSize > freeBytesCount)
		{
			auto nextPageNumber = db->GetNextPageNumber();
			nextScheme = new TableScheme(file, nextPageNumber, columns, db, iterator);
			SetNextPage(nextPageNumber);
			return;
		}
		file->Write(&iterator->nameLength, sizeof(iterator->nameLength));
		file->Write(iterator->name, sizeof(*iterator->name) * iterator->nameLength);
		file->Write(&iterator->dataType, sizeof(iterator->dataType));
		RewriteFreeBytesValues(static_cast<zint32>(currentDataSize));
		file->Seek(firstFreeByteOffset);
	}
	nextScheme = nullptr;
}

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber, Database* db) :
	Page(file, pageNumber, DbPageType::TableScheme, false, db)
{
	auto position = file->Tell();
	recordsPage = new DbRecordsPage(file, db->GetNextPageNumber(), true, db);
	file->Seek(firstFreeByteOffset);
	auto recordsPageOffset = recordsPage->GetPageOffset();
	file->Write(&recordsPageOffset, sizeof(recordsPageOffset));
	while (position != firstFreeByteOffset)
	{
		zint32 nameLength;
		file->Read(&nameLength, sizeof(nameLength));
		zchar* name = static_cast<zchar*>(HeapAlloc(hHeap,
			HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(*name) * (nameLength + 1)));
		file->Read(name, sizeof(zchar) * nameLength);
		DbDataType dataType;
		file->Read(&dataType, sizeof(dataType));
		bool nullable;
		file->Read(&nullable, sizeof(nullable));
		DbColumn record(name, dataType, nullable);
		columns.push_back(record);
		position = file->Tell();
	}
	if (nextPageOffset != 0)
	{
		nextScheme = new TableScheme(file, static_cast<zint32>((nextPageOffset - 4) / PAGE_SIZE), db, columns);
	} 
	else
	{
		nextScheme = nullptr;
	}
}

zdb::TableScheme::TableScheme(const TableScheme& tableScheme) : Page(tableScheme)
{
	this->columns = tableScheme.columns;
	this->nextScheme = tableScheme.nextScheme;
}

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber, const std::vector<DbColumn>& columns, 
	Database* db, std::vector<DbColumn>::const_iterator& columnIterator) : 
	Page(file, pageNumber, DbPageType::TableScheme, true, db)
{
	file->Seek(firstFreeByteOffset);
	for (auto iterator = columnIterator; iterator != columns.end(); ++iterator)
	{
		auto currentDataSize = sizeof(iterator->nameLength) + sizeof(*iterator->name) * iterator->nameLength
			+ sizeof(iterator->dataType);
		if (currentDataSize > PAGE_SIZE)
		{
			throw TooLongColumnNameException();
		}
		if (currentDataSize > freeBytesCount)
		{
			auto nextPageNumber = db->GetNextPageNumber();
			nextScheme = new TableScheme(file, nextPageNumber, columns, db, iterator);
			SetNextPage(nextPageNumber);
			return;
		}
		file->Write(&iterator->nameLength, sizeof(iterator->nameLength));
		file->Write(iterator->name, sizeof(*iterator->name) * iterator->nameLength);
		file->Write(&iterator->dataType, sizeof(iterator->dataType));
		RewriteFreeBytesValues(static_cast<zint32>(currentDataSize));
		file->Seek(firstFreeByteOffset);
	}
	nextScheme = nullptr;
}

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber, 
	Database* db, std::vector<DbColumn>& columns) : 
	Page(file, pageNumber, DbPageType::TableScheme, false, db)
{
	auto position = file->Tell();
	while (position != firstFreeByteOffset)
	{
		zint32 nameLength;
		file->Read(&nameLength, sizeof(nameLength));
		zchar* name = static_cast<zchar*>(HeapAlloc(hHeap,
			HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(*name) * (nameLength + 1)));
		file->Read(name, sizeof(zchar) * nameLength);
		DbDataType dataType;
		file->Read(&dataType, sizeof(dataType));
		bool nullable;
		file->Read(&nullable, sizeof(nullable));
		DbColumn record(name, dataType, nullable);
		columns.push_back(record);
		position = file->Tell();
	}
	if (nextPageOffset != 0)
	{
		nextScheme = new TableScheme(file, static_cast<zint32>((nextPageOffset - 4) / PAGE_SIZE), db, columns);
	}
	else
	{
		nextScheme = nullptr;
	}
}