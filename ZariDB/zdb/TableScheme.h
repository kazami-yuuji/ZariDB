#pragma once
#include <vector>
#include <exception>
#include "../utils/types.h"
#include "../utils/File.h"
#include "DbColumn.h"
#include "Page.h"

namespace zdb
{

	class TableScheme : public Page
	{
	public:
		TableScheme(utils::File* file, zint32 pageNumber, 
			const std::vector<DbColumn>& columns, Database* db);
		TableScheme(utils::File* file, zint32 pageNumber, Database* db);
		TableScheme(const TableScheme& tableScheme);

	private:
		TableScheme(utils::File* file, zint32 pageNumber,
			const std::vector<DbColumn>& columns, Database* db, 
			std::vector<DbColumn>::const_iterator& columnIterator);
		TableScheme(utils::File* file, zint32 pageNumber, Database* db,
			std::vector<DbColumn>& columns);
		std::vector<DbColumn> columns;
		TableScheme* nextScheme;
	};

	class ColumnsNumberInvalidException : public std::exception
	{
	public:
		char const* what() const override {
			return "Columns number should be greater than zero.";
		}
	};

	struct TooLongColumnNameException : std::exception
	{
		char const* what() const override {
			return "Name of column is too long (4080 max).";
		}
	};
}