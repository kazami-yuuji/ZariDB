#include "TableScheme.h"

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber,
	const std::vector<DbColumn>& columns) :
	Page(file, pageNumber, DbPageType::TableScheme)
{
	file->Seek(firstFreeByteOffset);
	auto dataSize = 0;
	for (auto iterator = columns.begin(); iterator != columns.end(); ++iterator)
	{
		// TODO some code creating next page if neccessary
		file->Write(&iterator->nameLength, sizeof(iterator->nameLength));
		file->Write(&iterator->name, sizeof(*iterator->name) * iterator->nameLength);
		file->Write(&iterator->dataType, sizeof(iterator->dataType));
		dataSize += sizeof(iterator->nameLength) + sizeof(*iterator->name) * iterator->nameLength
			+ sizeof(iterator->dataType);
		RewriteFreeBytesValues(dataSize);
	}
}