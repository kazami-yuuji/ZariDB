#pragma once
#include "../utils/types.h"
#include "../utils/File.h"
#include "DbScheme.h"
#include "DbColumn.h"

namespace zdb
{
	class Database
	{
	public:
		~Database();

		static Database* Create(const zchar* dbname);
		static Database* Open(const zchar* dbname);

		void AddTable(const zchar* name, const std::vector<DbColumn>& columns);
		zint32 GetNextPageNumber();

		const zchar DEFAULT_PATH[4] = L"zdb";
	private:
		explicit Database(const zchar* dbname, bool create);
		Database(const Database&);

		utils::File* file;
		const zchara SIGNATURE[4] = "ZDB";
		zint32 pageNumber;

		DbScheme* scheme;
	};

	class DbFileNotValidException : public std::exception
	{
	public:
		char const* what() const override {
			return "That's not a zdb file.";
		}
	};
}