#include "DbScheme.h"

zdb::DbScheme::DbScheme(utils::File* file, zint32 pageNumber) : Page(file, pageNumber, DbPageType::DbScheme)
{
	
}