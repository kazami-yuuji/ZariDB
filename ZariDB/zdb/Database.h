#pragma once
#include "../utils/types.h"

namespace zdb
{
	class Database
	{
	public:
		Database();
		~Database();

		const zchar* const DEFAULT_PATH = L"zdb";

	};
}