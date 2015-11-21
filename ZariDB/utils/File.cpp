#include "File.h"

utils::File::File(const zchar* const filename, FileAccess access, FileShare share,
	FileMode mode, FileType type)
{
	hFile = CreateFileW(filename, static_cast<DWORD>(access), static_cast<DWORD>(share), nullptr,
		static_cast<DWORD>(mode), static_cast<DWORD>(type), nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringW(L"Something is wrong:\n");
		auto error = GetLastError();
		switch (error)
		{
		case ERROR_FILE_EXISTS:
			OutputDebugStringW(L"File already exists.\n");
			throw FileAlreadyExistsException();
		case ERROR_FILE_NOT_FOUND:
			OutputDebugStringW(L"File not found.\n");
			throw FileNotFoundException();
		case ERROR_SHARING_VIOLATION:
			OutputDebugStringW(L"File cannot be shared.\n");
			throw FileSharingViolationException();
		default:
			OutputDebugStringW(L"Reason is not defined.\n");
			throw FileException();
		}
	}
}

void utils::File::Write(const void * data, zuint32 count) const
{
	zuint32l bytesWritten = 0;
	if (!WriteFile(hFile, data, count, &bytesWritten, nullptr))
	{
		throw FileException();
	}
}

void utils::File::Read(void* data, zuint32 count) const
{
	zuint32l bytesRead;
	if (!ReadFile(hFile, data, count, &bytesRead, nullptr))
	{
		throw FileException();
	}
}

void utils::File::Seek(zint64 position) const
{
	zint32 low = position & 0xFFFFFFFF;
	zint32l high = position >> 32;
	auto result = SetFilePointer(hFile, low, &high, FILE_BEGIN);
	if (result == INVALID_SET_FILE_POINTER)
	{
		throw FileException();
	}
}

zint64 utils::File::Tell() const
{
	auto low = 0;
	zint32l high = 0;
	low = SetFilePointer(hFile, 0, &high, FILE_CURRENT);
	auto result = (static_cast<zint64>(high) << 32) | low;
	return result;
}

utils::File::~File()
{
	if (!hFile)
		CloseHandle(hFile);
}

