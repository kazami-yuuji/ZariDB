#include "zdb/Database.h"

int main()
{
	auto database = zdb::Database::Create(L"test");
	zdb::DbColumn col11(L"id", DbDataType::Int32);
	zdb::DbColumn col12(L"title", DbDataType::String);
	zdb::DbColumn col13(L"description", DbDataType::String);
	zdb::DbColumn col14(L"mdurl", DbDataType::String);
	zdb::DbColumn col15(L"pdf", DbDataType::String);
	zdb::DbColumn col16(L"pub", DbDataType::Int32);
	std::vector<zdb::DbColumn> columns1 = { col11, col12, col13, col14, col15, col16 };
	database->AddTable(L"articles", columns1);

	zdb::DbColumn col21(L"id", DbDataType::Int32);
	zdb::DbColumn col22(L"name", DbDataType::String);
	zdb::DbColumn col23(L"publication_type_id", DbDataType::Int32);
	zdb::DbColumn col24(L"publisher_id", DbDataType::Int32);
	std::vector<zdb::DbColumn> columns2 = { col21, col22, col23, col24 };
	database->AddTable(L"publications", columns2);
	delete database;
	return 0;
}