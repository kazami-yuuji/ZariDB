#pragma once
#include "../utils/types.h"
#include "../utils/File.h"
#include "DbScheme.h"

namespace zdb
{
	class Database
	{
	public:
		explicit Database(const zchar* dbname, bool create);
		Database(const Database&) = delete;
		~Database();

		const zchar DEFAULT_PATH[4] = L"zdb";
	private:
		utils::File* file;
		const zchara SIGNATURE[4] = "ZDB";

		DbScheme* scheme;
	};
}