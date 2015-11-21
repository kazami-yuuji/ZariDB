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
			utils::FileShare::Write, utils::FileMode::Create, utils::FileType::Random);
#else
		file = new utils::File(path.ToString(), utils::FileAccess::ReadWrite,
			utils::FileShare::Write, utils::FileMode::CreateNew, utils::FileType::Random);
#endif
		file->Write(SIGNATURE, sizeof(SIGNATURE));
		scheme = new DbScheme(file, 0);
	}
}

zdb::Database::~Database()
{
	delete file;
}