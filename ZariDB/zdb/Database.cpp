#include "Database.h"
#include "../utils/String.h"
#include "DbScheme.h"

zdb::Database::Database(const zchar* dbname, bool create)
{
	if (create)
	{
		utils::String path(DEFAULT_PATH);
		path = path + L"\\" + dbname;
#if _DEBUG
		file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
			utils::FileShare::Read, utils::FileMode::Create, utils::FileType::Random);
#else
		file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
			utils::FileShare::Read, utils::FileMode::CreateNew, utils::FileType::Random);
#endif
		file->Write(SIGNATURE, sizeof(SIGNATURE));
		pageNumber = 1;
	}
	else
	{
		utils::String path(DEFAULT_PATH);
		path = path + L"\\" + dbname;
		file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
			utils::FileShare::Read, utils::FileMode::Open, utils::FileType::Random);
		zchara buffer[4];
		file->Read(buffer, sizeof(*buffer) * 4);
		if (lstrcmpA(SIGNATURE, buffer))
		{
			throw DbFileNotValidException();
		}
	}
}

zdb::Database::~Database()
{
	delete file;
}

zdb::Database* zdb::Database::Create(const zchar* dbname)
{
	auto database = new Database(dbname, true);
	database->scheme = DbScheme::CreateScheme(database->file, database);
	return database;
}

zdb::Database* zdb::Database::Open(const zchar* dbname)
{
	auto database = new Database(dbname, false);
	database->scheme = DbScheme::OpenScheme(database->file, database);
	return database;
}

void zdb::Database::AddTable(const zchar* name, const std::vector<DbColumn>& columns)
{
	scheme->AddTable(GetNextPageNumber(), name, columns);
}

zint32 zdb::Database::GetNextPageNumber()
{
	return pageNumber++;
}