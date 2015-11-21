#include "TableScheme.h"

zdb::TableScheme::TableScheme(utils::File* file, zint32 pageNumber, zint32 columnNumber,
	const DbColumn** columns) :
	Page(file, pageNumber, DbPageType::TableScheme)
{
	file->Seek(firstFreeByteOffset);
	for (auto i = 0; i < columnNumber; i++)
	{
		// TODO some code creating next page if neccessary
		file->Write(&columns[i]->nameLength, sizeof(columns[i]->nameLength));
		file->Write(&columns[i]->name, sizeof(*columns[i]->name) * columns[i]->nameLength);
		file->Write(&columns[i]->dataType, sizeof(columns[i]->dataType));
	}
}