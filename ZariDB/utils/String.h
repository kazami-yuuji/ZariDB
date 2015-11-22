#pragma once
#include <Windows.h>
#include <exception>
#include "types.h"

namespace utils
{
	class String
	{
	public:
		String();
		explicit String(const zchar* str);
		String(const String& str);
		~String();

		const zchar* ToString() const;

		String& operator=(const zchar* str);
		String& operator+=(const zchar* str);
		friend String& operator+(const String &string, const zchar* str);

	private:
		zchar* string;
		zint32 length;
		HANDLE hHeap;
	};

	struct HeapException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "File not found.";
		}
	};
}