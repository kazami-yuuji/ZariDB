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
		bool operator==(const zchar* str);
		bool operator==(const String& str);
		friend String& operator+(const String &string, const zchar* str);

		bool Consists(const String& str);
		bool StartsWith(const String& str);

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