#pragma once
#include "Page.h"
#include "TableScheme.h"
#include <vector>
#include "Database.h"

namespace zdb
{
	class DbScheme : public Page
	{
	public:
		~DbScheme();

		void AddTable(zint32 pageNumber, const zchar* name, const std::vector<DbColumn>& columns);

		static DbScheme* OpenScheme(utils::File* file, Database* db);
		static DbScheme* CreateScheme(utils::File* file, Database* db);

	private:
		std::vector<TableScheme> tables;
		explicit DbScheme(utils::File* file, bool create, Database* db);
		explicit DbScheme(utils::File* file, Database* db, zint32 pageNumber);
		explicit DbScheme(utils::File* file, Database* db, zint32 pageNumber, 
			std::vector<TableScheme>* schemes);
		DbScheme* nextScheme;

		void AddTable(TableScheme& scheme, zint32 nameLength, const zchar* name);
	};

	struct DbSchemeRecord
	{
		utils::String name;
		zint64 schemeOffset;

		DbSchemeRecord(const utils::String& name, zint64 scheme_offset)
			: name(name),
			  schemeOffset(scheme_offset)
		{
		}
	};

	struct TooLongTableNameException : std::exception
	{
		char const* what() const override {
			return "Name of table is too long (4078 max).";
		}
	};
}
