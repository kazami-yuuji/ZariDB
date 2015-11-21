#pragma once
#include <Windows.h>
#include <system_error>
#include "types.h"

namespace utils
{
	enum class FileAccess : zuint32l
	{
		Read = GENERIC_READ,
		Write = GENERIC_WRITE,
		ReadWrite = GENERIC_READ | GENERIC_WRITE
	};

	enum class FileShare : zuint32l
	{
		None = NULL,
		Read = FILE_SHARE_READ,
		Write = FILE_SHARE_WRITE,
		Delete = FILE_SHARE_DELETE
	};

	enum class FileMode : zuint32l
	{
		Create = CREATE_ALWAYS,
		CreateNew = CREATE_NEW,
		Open = OPEN_EXISTING,
		OpenOrCreate = OPEN_ALWAYS,
		Truncate = TRUNCATE_EXISTING
	};

	enum class FileType : zuint32l
	{
		Sequential = FILE_FLAG_SEQUENTIAL_SCAN,
		Random = FILE_FLAG_RANDOM_ACCESS,
		None = NULL
	};

	class File
	{
	public:
		File(const zchar* const filename, FileAccess access, FileShare share,
			FileMode mode, FileType type);
		File(const File&) = delete;
		void Write(const void* data, zuint32 count) const;
		void Read(void* data, zuint32 count) const;
		void Seek(zint64 position) const;
		zint64 Tell() const;
		~File();

	private:
		HANDLE hFile;
	};

	struct FileNotFoundException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "File not found.";
		}
	};

	struct FileAlreadyExistsException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "File already exists.";
		}
	};

	struct FileSharingViolationException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "File cannot be shared.";
		}
	};

	struct FileException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "Error with file occured.";
		}
	};

	struct InvalidUserBufferException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "Error with file occured.";
		}
	};

	struct NotEnoughMemoryException : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "Not enough memory on I/O device.";
		}
	};
}