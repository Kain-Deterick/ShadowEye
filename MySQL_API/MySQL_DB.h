#pragma once
#pragma comment(lib, "MySQL_API.lib")
#pragma warning(disable : 4996)
#ifndef MySQL_DB_H
#define MySQL_DB_H
#define INFO_BUFFER_SIZE 32767
//#ifdef _MSC_VER
//// Если используется Microsoft Visual C++
//#ifdef MY_DLL_BUILD
//// Если файл подключается из проекта библиотеки
//#define MySQL_DB_API __declspec (dllexport)
//#else // MY_DLL_BUILD
//// Если файл подключается из проекта клиента
//#define MySQL_DB_API __declspec (dllimport)
//#endif // MY_DLL_BUILD (else)
//#else // _MSC_VER
//// Если используется любой другой компилятор
//#define MySQL_DB_API
//#endif // _MSC_VER (else)


#include <mysqlx/xdevapi.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <ctime>
#include <format>
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <Lmcons.h>

#include "../Headers/SmartHandle.h"
#include "immutable_variables.h"


typedef std::chrono::high_resolution_clock Chrono;
using  namespace mysqlx;

class MySQL_API
{
public:
	MySQL_API(int var_number_class, Chrono::time_point time_start_app_constructor);
	~MySQL_API();

	void set_ProcessInfo_start(std::string nameProcess);
	void set_ProcessInfo_finish(std::string nameProcess, long Time_result);
private:
	void cleanup();
	std::string convertValue_to_String(Value var);
	std::string TimeNow() const;
	const TCHAR* computerID();
	std::string Data() const;
	std::wstring stringToWString(const std::string& str);
	const TCHAR* nameUser();
	std::string convertSecondsToTimeString(long total_seconds);
private:

	mysqlx::Session* sess;
	mysqlx::Schema* sch;

	int Meaning_of_connection_attempts = 0;
	Chrono::time_point time_start_app_class;
	string var_name_comp;
	int numberClass;
	bool findName = 0;
	int ID_comp;
};

#endif //DATABASEAPI_H