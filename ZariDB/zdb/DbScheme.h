#pragma once
#include <vector>
#include "../utils/types.h"
#include "../utils/File.h"
#include "../utils/String.h"
#include "DbStructures.h"
#include "TableScheme.h"

namespace zdb
{
	struct DbSchemeRecord;

	class DbScheme : public Page
	{
	public:
		~DbScheme();

		void AddTable(zint32 pageNumber, const zchar* name, const std::vector<DbColumn>& columns);

		static DbScheme* OpenScheme(utils::File* file, Database* db);
		static DbScheme* CreateScheme(utils::File* file, Database* db);

		std::vector<Tuple> Select(const std::vector<zchar*>& selectedTables,
			const std::vector<zchar*>& selectedColumns, const std::vector<zchar*>& whereColumns,
			const std::vector<DbRelationalOperationsType>& whereOperations, const std::vector<void*>& whereValues,
			const std::vector<DbBooleanOperationsType>& whereBoolOperations, const std::vector<zchar*>& sortColumns,
			const std::vector<zchar*>& sortOrder, const std::vector<zchar*>& groupbyColumns);

		std::vector<Tuple> CortesianProduct(const std::vector<TableScheme>& foundTables);
	private:
		std::vector<TableScheme> tables;
		std::vector<DbSchemeRecord> tablesRecords;
		explicit DbScheme(utils::File* file, bool create, Database* db);
		explicit DbScheme(utils::File* file, Database* db, zint32 pageNumber);
		explicit DbScheme(utils::File* file, Database* db, zint32 pageNumber, 
			std::vector<TableScheme>& schemes);
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

	struct AmbigiousTableNameException : std::exception
	{
		char const* what() const override {
			return "There is a table with such name. Please, choose another one.";
		}
	};

	struct AmbigiousColumnNameException : std::exception
	{
		char const* what() const override {
			return "There is a column with such name in the table. Please, choose another one.";
		}
	};
}
