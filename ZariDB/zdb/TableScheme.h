#pragma once
#include "Page.h"
#include "DbColumn.h"

namespace zdb
{

	class TableScheme : public Page
	{
		TableScheme(utils::File* file, zint32 pageNumber, zint32 columnNumber, 
			const DbColumn** columns);
	};
}