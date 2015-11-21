#include "Database.h"
#include "../utils/String.h"
#include "DbScheme.h"

zdb::Database::Database(const zchar* dbname)
{
	utils::String path(DEFAULT_PATH);
	path = path + L"\\" + dbname;
#if _DEBUG
	file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
		utils::FileShare::Write, utils::FileMode::Create, utils::FileType::Random);
#else
	file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
		utils::FileShare::Write, utils::FileMode::CreateNew, utils::FileType::Random);
#endif
	file->Write(SIGNATURE, sizeof(SIGNATURE));
	pageNumber = 0;
	scheme = new DbScheme(file, GetNextPageNumber());
}

zdb::Database::~Database()
{
	delete file;
}

void zdb::Database::AddTable(const zchar* name, const std::vector<DbColumn>& columns)
{
	scheme->AddTable(GetNextPageNumber(), name, columns);
}

zint32 zdb::Database::GetNextPageNumber()
{
	return pageNumber++;
}