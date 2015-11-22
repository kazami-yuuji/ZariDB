#include "DbScheme.h"

zdb::DbScheme::DbScheme(utils::File* file, bool create, Database* db) : Page(file, 0, DbPageType::DbScheme, create, db)
{
	if (create)
	{
		nextScheme = nullptr;
		return;
	}
	auto position = file->Tell();
	std::vector<DbSchemeRecord> tablesRecords;
	while (position != firstFreeByteOffset)
	{
		zint32 nameLength;
		file->Read(&nameLength, sizeof(nameLength));
		zchar* name = static_cast<zchar*>(HeapAlloc(hHeap,
			HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(*name) * (nameLength + 1)));
		file->Read(name, sizeof(zchar) * nameLength);
		zint64 offset;
		file->Read(&offset, sizeof(offset));
		utils::String strName(name);
		DbSchemeRecord record(strName, offset);
		tablesRecords.push_back(record);
		position = file->Tell();
	}
	for (auto it = tablesRecords.begin(); it != tablesRecords.end(); ++it)
	{
		TableScheme tableScheme(file, (it->schemeOffset - 4) / PAGE_SIZE, db);
		tables.push_back(tableScheme);
	}
	if (nextPageOffset > 0)
	{
		nextScheme = new DbScheme(file, db, (nextPageOffset - 4) / PAGE_SIZE, &tables);
	}
}

zdb::DbScheme::DbScheme(utils::File* file, Database* db, zint32 pageNumber) :
	Page(file, pageNumber, DbPageType::DbScheme, true, db)
{
	nextScheme = nullptr;
}

zdb::DbScheme::DbScheme(utils::File* file, Database* db, zint32 pageNumber, 
	std::vector<TableScheme>* schemes) : 
	Page(file, pageNumber, DbPageType::DbScheme, false, db)
{
	auto position = file->Tell();
	std::vector<DbSchemeRecord> tablesRecords;
	while (position != firstFreeByteOffset)
	{
		zint32 nameLength;
		file->Read(&nameLength, sizeof(nameLength));
		zchar* name = static_cast<zchar*>(HeapAlloc(hHeap,
			HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(*name) * (nameLength + 1)));
		file->Read(name, sizeof(zchar) * nameLength);
		zint64 offset;
		file->Read(&offset, sizeof(offset));
		utils::String strName(name);
		DbSchemeRecord record(strName, offset);
		tablesRecords.push_back(record);
		position = file->Tell();
	}
	for (auto it = tablesRecords.begin(); it != tablesRecords.end(); ++it)
	{
		TableScheme tableScheme(file, (it->schemeOffset - 4) / PAGE_SIZE, db);
		schemes->push_back(tableScheme);
	}
	if (nextPageOffset > 0)
	{
		nextScheme = new DbScheme(file, db, (nextPageOffset - 4) / PAGE_SIZE, schemes);
	}
}

void zdb::DbScheme::AddTable(TableScheme& scheme, zint32 nameLength, const zchar* name)
{
	auto tableSchemeOffset = scheme.GetPageOffset();
	auto dataSize = sizeof(nameLength) + sizeof(zchar) * nameLength + sizeof(tableSchemeOffset);
	if (freeBytesCount >= dataSize)
	{
		file->Seek(firstFreeByteOffset);
		file->Write(&nameLength, sizeof(nameLength));
		file->Write(name, sizeof(zchar) * nameLength);
		file->Write(&tableSchemeOffset, sizeof(tableSchemeOffset));
		RewriteFreeBytesValues(dataSize);
	}
	else
	{
		if (nextScheme == nullptr)
		{
			nextScheme = new DbScheme(file, db, db->GetNextPageNumber());
		}
		nextScheme->AddTable(scheme, nameLength, name);
	}
}

zdb::DbScheme::~DbScheme()
{

}

void zdb::DbScheme::AddTable(zint32 pageNumber, const zchar* name, const std::vector<DbColumn>& columns)
{
	TableScheme scheme(file, pageNumber, columns, db);
	tables.push_back(scheme);
	auto tableSchemeOffset = scheme.GetPageOffset();
	auto nameLength = lstrlenW(name);
	auto dataSize = sizeof(nameLength) + sizeof(zchar) * nameLength + sizeof(tableSchemeOffset);
	if (dataSize > PAGE_SIZE)
		throw TooLongTableNameException();
	if (freeBytesCount >= dataSize)
	{
		file->Seek(firstFreeByteOffset);
		file->Write(&nameLength, sizeof(nameLength));
		file->Write(name, sizeof(zchar) * nameLength);
		file->Write(&tableSchemeOffset, sizeof(tableSchemeOffset));
		RewriteFreeBytesValues(dataSize);
	} 
	else
	{
		if (nextScheme == nullptr)
		{
			nextScheme = new DbScheme(file, db, db->GetNextPageNumber());
		}
		nextScheme->AddTable(scheme, nameLength, name);
	}
	
}

zdb::DbScheme* zdb::DbScheme::OpenScheme(utils::File* file, Database* db)
{
	auto dbScheme = new DbScheme(file, false, db);
	return dbScheme;
}

zdb::DbScheme* zdb::DbScheme::CreateScheme(utils::File* file, Database* db)
{
	auto dbScheme = new DbScheme(file, true, db);
	return dbScheme;
}