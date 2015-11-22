#include "TableScheme.h"

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber,
	const std::vector<DbColumn>& columns, Database* db) :
	Page(file, pageNumber, DbPageType::TableScheme, true, db)
{
	file->Seek(firstFreeByteOffset);
	auto dataSize = 0;
	for (auto iterator = columns.begin(); iterator != columns.end(); ++iterator)
	{
		// TODO some code creating next page if neccessary
		file->Write(&iterator->nameLength, sizeof(iterator->nameLength));
		file->Write(iterator->name, sizeof(*iterator->name) * iterator->nameLength);
		file->Write(&iterator->dataType, sizeof(iterator->dataType));
		dataSize += sizeof(iterator->nameLength) + sizeof(*iterator->name) * iterator->nameLength
			+ sizeof(iterator->dataType);
	}
	RewriteFreeBytesValues(dataSize);
}

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber, Database* db) :
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
		DbColumn record(name, dataType);
		columns.push_back(record);
		position = file->Tell();
	}
}

zdb::TableScheme::TableScheme(const TableScheme& tableScheme) : Page(tableScheme)
{
	this->columns = tableScheme.columns;
}