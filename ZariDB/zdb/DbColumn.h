#pragma once
#include <Windows.h>
#include "../utils/types.h"
#include "../utils/String.h"
#include "DbDataType.h"

namespace zdb
{
	struct DbColumn
	{
		zint32 nameLength;
		zchar* name;
		DbDataType dataType;

		DbColumn(const zchar* name, DbDataType dataType)
		{
			hHeap = GetProcessHeap();
			if (!hHeap)
			{
				throw utils::HeapException();
			}
			nameLength = lstrlenW(name);
			this->name = static_cast<zchar*>(HeapAlloc(hHeap,
				HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(zchar) * (nameLength + 1)));
			memcpy(this->name, name, sizeof(zchar) * nameLength);
			this->dataType = dataType;
		}

		DbColumn(const DbColumn& dbColumn)
		{
			hHeap = GetProcessHeap();
			if (!hHeap)
			{
				throw utils::HeapException();
			}
			nameLength = lstrlenW(dbColumn.name);
			this->name = static_cast<zchar*>(HeapAlloc(hHeap,
				HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, sizeof(zchar) * (nameLength + 1)));
			memcpy(this->name, dbColumn.name, sizeof(zchar) * nameLength);
			this->dataType = dbColumn.dataType;
		}

		~DbColumn()
		{
			HeapFree(hHeap, NULL, name);
		}

	private:
		HANDLE hHeap;
	};
}