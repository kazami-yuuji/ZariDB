#pragma once
#include "Page.h"
#include "DbColumn.h"
#include <vector>
#include <exception>

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
		std::vector<DbColumn> columns;
	};

	class ColumnsNumberInvalidException : public std::exception
	{
	public:
		char const* what() const override {
			return "Columns number should be greater than zero.";
		}
	};
}