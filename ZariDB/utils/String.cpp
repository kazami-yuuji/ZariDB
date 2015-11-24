#include "String.h"

utils::String::String()
{
	hHeap = GetProcessHeap();
	if (!hHeap)
	{
		throw HeapException();
	}
	string = nullptr;
	length = 0;
}

utils::String::String(const zchar* str) : String()
{
	length = lstrlenW(str);
	string = static_cast<zchar*>(HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, 
		sizeof(zchar) * (length + 1)));
	memcpy(string, str, sizeof(zchar) * length);
}

utils::String::String(const String& str)
{
	hHeap = GetProcessHeap();
	length = str.length;
	string = static_cast<zchar*>(HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
		sizeof(zchar) * (length + 1)));
	memcpy(string, str.string, sizeof(zchar) * length);
}

utils::String::~String()
{
	if (string != nullptr)
		HeapFree(hHeap, NULL, string);
}

const zchar* utils::String::ToString() const
{
	return string;
}

utils::String& utils::String::operator=(const zchar* str)
{
	HeapFree(hHeap, NULL, string);
	length = lstrlenW(str);
	string = static_cast<zchar*>(HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
		sizeof(zchar) * (length + 1)));
	return *this;
}

utils::String& utils::String::operator+=(const zchar* str)
{
	auto strlen = lstrlenW(str);
	auto newlen = length + strlen;
	auto newstringBuffer = static_cast<zchar*>(HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
		sizeof(zchar) * (newlen + 1)));
	memcpy(newstringBuffer, string, sizeof(zchar) * length);
	memcpy(newstringBuffer + length, str, sizeof(zchar) * strlen);
	length = newlen;
	HeapFree(hHeap, NULL, string);
	string = newstringBuffer;
	return *this;
}

bool utils::String::operator==(const zchar* str)
{
	return lstrcmpW(this->string, str) == 0;
}

bool utils::String::Consists(const String& str)
{
	auto strlenCurrent = lstrlenW(string);
	auto strlenParameter = lstrlenW(str.string);
	for (auto i = 0; i < strlenCurrent; i++)
	{
		if (string[i] == str.string[0])
		{
			auto confirmed = true;
			for (auto j = 1; j < strlenParameter; j++)
			{
				if (i + j >= strlenCurrent)
					return false;
				if (string[i + j] != str.string[j])
				{
					confirmed = false;
					break;
				}
			}
			if (confirmed)
			{
				return true;
			}
		}
	}
	return false;
}

bool utils::String::StartsWith(const String& str)
{
	auto strlenCurrent = lstrlenW(string);
	auto strlenParameter = lstrlenW(str.string);
	for (auto i = 0; i < strlenCurrent; i++)
	{
		if (i >= strlenParameter)
			return true;
		if (string[i] != str.string[i])
			return false;
	}
	return false;
}

bool utils::String::operator==(const String& str)
{
	return lstrcmpW(this->string, str.string) == 0;
}

utils::String& utils::operator+(const String& string, const zchar* str)
{
	auto strlen = lstrlenW(str);
	auto newlen = string.length + strlen;
	auto newstringBuffer = static_cast<zchar*>(HeapAlloc(string.hHeap, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
		sizeof(zchar) * (newlen + 1)));
	auto newstring = new String();
	memcpy(newstringBuffer, string.string, sizeof(zchar) * string.length);
	memcpy(newstringBuffer + string.length, str, sizeof(zchar) * strlen);
	newstring->length = newlen;
	newstring->string = newstringBuffer;
	return *newstring;
}