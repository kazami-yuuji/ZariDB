#include <iostream>
#include "utils\File.h"
#include "utils\String.h"
#include "zdb/Database.h"
#include "zdb/Page.h"

int main()
{
	zdb::Database database(L"test", true);
	return 0;
}