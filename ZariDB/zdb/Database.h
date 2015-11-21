#pragma once
#include "../utils/types.h"
#include "../utils/File.h"
#include "DbScheme.h"

namespace zdb
{
	class Database
	{
	public:
		explicit Database(const zchar* dbname);
		Database(const Database&) = delete;
		~Database();

		void AddTable(const zchar* name, const std::vector<DbColumn>& columns);

		const zchar DEFAULT_PATH[4] = L"zdb";
	private:
		utils::File* file;
		const zchara SIGNATURE[4] = "ZDB";
		zint32 pageNumber;
		zint32 GetNextPageNumber();

		DbScheme* scheme;
	};
}