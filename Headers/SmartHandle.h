#pragma once
#ifndef SMARTHANDLE_H
#define SMARTHANDLE_H
#include <Windows.h>
#pragma comment(lib, "psapi.lib")
#include <TlHelp32.h>
class SmartHandle
{
public:
	SmartHandle(HANDLE handle) : _handle(handle) {}
	~SmartHandle()
	{
		if (_handle)
		{
			CloseHandle(_handle);
		}
	}

	operator bool()
	{
		return _handle != NULL;
	}

	operator HANDLE()
	{
		return _handle;
	}

	HANDLE handle()
	{
		return _handle;
	}
private:
	HANDLE _handle = NULL;
};

#endif // !