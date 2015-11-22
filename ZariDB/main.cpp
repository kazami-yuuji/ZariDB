#include <iostream>
#include "utils\File.h"
#include "utils\String.h"
#include "zdb/Database.h"
#include "zdb/Page.h"

int main()
{
	auto database = zdb::Database::Create(L"test");
	zdb::DbColumn col1(L"lalka1", DbDataType::Int32);
	zdb::DbColumn col2(L"lalka2", DbDataType::Int64);
	zdb::DbColumn col3(L"lalka3", DbDataType::Int8);
	std::vector<zdb::DbColumn> columns = { col1, col2, col3 };
	database->AddTable(L"testTable", columns);
	delete database;
	return 0;
}