#pragma once
#include "Page.h"

namespace zdb
{
	class DbScheme : public Page
	{
	public:
		explicit DbScheme(utils::File* file, zint32 pageNumber);
		~DbScheme() override {}
	};
}
