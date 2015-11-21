#pragma once
#include "Page.h"

namespace zdb
{
	class DbScheme : public Page
	{
		explicit DbScheme(utils::File* file);
	};
}
