#include "DbScheme.h"

zdb::DbScheme::DbScheme(utils::File* file, zint32 pageNumber) : Page(file, pageNumber, DbPageType::DbScheme)
{
}

zdb::DbScheme::~DbScheme()
{
}

void zdb::DbScheme::AddTable(zint32 pageNumber, const zchar* name, const std::vector<DbColumn>& columns)
{
	TableScheme scheme(file, pageNumber, columns);
	tables.push_back(scheme);
	auto tableSchemeOffset = scheme.GetPageOffset();
	auto nameLength = lstrlenW(name);
	file->Seek(firstFreeByteOffset);
	file->Write(&nameLength, sizeof(nameLength));
	file->Write(name, sizeof(zchar) * nameLength);
	file->Write(&tableSchemeOffset, sizeof(tableSchemeOffset));
	auto dataSize = sizeof(nameLength) + sizeof(zchar) * nameLength + sizeof(tableSchemeOffset);
	RewriteFreeBytesValues(dataSize);
}