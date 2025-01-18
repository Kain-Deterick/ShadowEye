#include "MySQL_DB.h"

MySQL_API::MySQL_API(int var_number_class, Chrono::time_point time_start_app_constructor):
	numberClass{ var_number_class }
{
	time_start_app_class = time_start_app_constructor;
	var_name_comp = computerID();

	while (Meaning_of_connection_attempts < 5)
	{
		try
		{
			sess = new Session(host, 33060, user_name, password);

			sch = new Schema(sess->getSchema("process_time", true));

			Table t_computer = sch->getTable("computer", true);
			Table t_computer_time = sch->getTable("computer_time", true);

			// Проверка БД наличие данного компьютера
			RowResult res = t_computer.select("*").execute();
			std::list<Row> rows = res.fetchAll();

			for (Row row : rows)
			{
				Value nameDB = row[1];

				if (convertValue_to_String(nameDB) == var_name_comp)
				{
					// Запоминаем индекс компьютера в таблице
					ID_comp = row[0];
					std::cout << ID_comp << std::endl;
					findName = 1;

					// Начинаем отсчёт времени работы компьютера
					t_computer_time.insert("ID_Comp", "Time_Before", "User", "Date").values(ID_comp, TimeNow(), convertValue_to_String(nameUser()), Data()).execute();
					break;
				}
			}
			// Если имени компа нету в таблице
			if (findName == 0)
			{
				t_computer.insert("Name", "Class").values(var_name_comp, var_number_class).execute();
				for (Row row : rows)
				{
					Value nameDB = row[1];

					if (convertValue_to_String(nameDB) == var_name_comp)
					{
						// Запоминаем индекс компьютера в таблице
						ID_comp = row[0];
						//std::cout << ID_comp << std::endl;
						break;
					}
				}
			}
			// Если успешно выходим из цикла
			break;
		}


		catch (const mysqlx::Error& err)
		{
			Meaning_of_connection_attempts++;
			if (Meaning_of_connection_attempts >= 5)
			{
				// Получаем строку сообщения об ошибке
				std::string errorMessage = err.what();

				// Преобразуем std::string в std::wstring
				std::wstring wErrorMessage = stringToWString(errorMessage);

				// Преобразуем std::wstring в LPCWSTR
				LPCWSTR lpwErrorMessage = wErrorMessage.c_str();

				// Отображаем сообщение об ошибке в MessageBox
				MessageBox(NULL, lpwErrorMessage, L"Error", MB_OK | MB_ICONERROR);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(60000)); // Ожидание перед повторной попыткой
		}
	}
}


MySQL_API::~MySQL_API()
{
	try
	{
		Table t_computer_time = sch->getTable("computer_time", true);
		Table t_computer = sch->getTable("computer", true);

		// Проверка БД наличие данного компьютера
		RowResult res = t_computer.select("*").execute();
		std::list<Row> rows = res.fetchAll();

		for (Row row : rows)
		{
			Value ID_Comp_select = row[0];
			if (convertValue_to_String(ID_Comp_select) == std::to_string(ID_comp))
			{
				RowResult row = t_computer_time.select("ID")
					.where("Time_After IS NULL AND ID_Comp = :ID")
					.bind("ID", ID_comp)
					.execute();

				int ID_NecessaryProcess = row.fetchOne()[0];
				std::chrono::seconds Time_Result = std::chrono::duration_cast<std::chrono::seconds>(Chrono::now() - time_start_app_class);
				long long_Time_Result = static_cast<long>(Time_Result.count());

				//std::cout << "ID_NecessaryProcess: " << ID_NecessaryProcess << std::endl;
				t_computer_time.update().set("Time_After", TimeNow())
					.set("Time_Result", convertSecondsToTimeString(long_Time_Result))
					.where("ID = :ID")
					.bind("ID", ID_NecessaryProcess)
					.execute();
				break;
			}
		}
		cleanup();
	}
	catch (const mysqlx::Error& err)
	{
		// Получаем строку сообщения об ошибке
		std::string errorMessage = err.what();

		// Преобразуем std::string в std::wstring
		std::wstring wErrorMessage = stringToWString(errorMessage);

		// Преобразуем std::wstring в LPCWSTR
		LPCWSTR lpwErrorMessage = wErrorMessage.c_str();

		// Отображаем сообщение об ошибке в MessageBox
		MessageBox(NULL, lpwErrorMessage, L"Error", MB_OK | MB_ICONERROR);
	}

}

// освобождение ресурсов
void MySQL_API::cleanup()
{
	if (sch) {
		delete sch;
		sch = nullptr;
	}
	if (sess) {
		delete sess;
		sess = nullptr;
	}
}

void MySQL_API::set_ProcessInfo_start(std::string nameProcess)
{
	try
	{
		Table t_time = sch->getTable("time", true);
		Table t_computer = sch->getTable("computer", true);

		// Проверка БД наличие данного компьютера
		RowResult res = t_computer.select("*").execute();
		std::list<Row> rows = res.fetchAll();

		for (Row row : rows)
		{
			Value ID_Comp_select = row[0];
			
			if (convertValue_to_String(ID_Comp_select) == std::to_string(ID_comp))
			{
				t_time.insert("ID_Comp", "Process", "Time_Before", "User", "Date").values(ID_comp, nameProcess, TimeNow(), convertValue_to_String(nameUser()), Data()).execute();
				break;
			}
		}
	}
	catch (const mysqlx::Error& err)
	{
		// Получаем строку сообщения об ошибке
		std::string errorMessage = err.what();

		// Преобразуем std::string в std::wstring
		std::wstring wErrorMessage = stringToWString(errorMessage);

		// Преобразуем std::wstring в LPCWSTR
		LPCWSTR lpwErrorMessage = wErrorMessage.c_str();

		// Отображаем сообщение об ошибке в MessageBox
		MessageBox(NULL, lpwErrorMessage, L"Error", MB_OK | MB_ICONERROR);
	}
}

void MySQL_API::set_ProcessInfo_finish(std::string nameProcess, long Time_result)
{
	try
	{
		Table t_time = sch->getTable("time", true);
		Table t_computer = sch->getTable("computer", true);

		// Проверка БД наличие данного компьютера
		RowResult res = t_computer.select("*").execute();
		std::list<Row> rows = res.fetchAll();

		for (Row row : rows)
		{
			Value ID_Comp_select = row[0];
			if (convertValue_to_String(ID_Comp_select) == std::to_string(ID_comp))
			{
				RowResult row = t_time.select("ID_Proc")
					.where("Process LIKE :param AND Time_After IS NULL AND ID_Comp = :ID")
					.bind("param", nameProcess)
					.bind("ID", ID_comp)
					.execute();

				int ID_NecessaryProcess = row.fetchOne()[0];

				//std::cout << "ID_NecessaryProcess: " << ID_NecessaryProcess << std::endl;
				t_time.update().set("Time_After", TimeNow())
					.set("Result_time", convertSecondsToTimeString(Time_result))
					.where("ID_Proc = :ID")
					.bind("ID", ID_NecessaryProcess)
					.execute();
				break;
			}
		} 
	}

	catch (const mysqlx::Error& err)
	{
		// Получаем строку сообщения об ошибке
		std::string errorMessage = err.what();

		// Преобразуем std::string в std::wstring
		std::wstring wErrorMessage = stringToWString(errorMessage);

		// Преобразуем std::wstring в LPCWSTR
		LPCWSTR lpwErrorMessage = wErrorMessage.c_str();

		// Отображаем сообщение об ошибке в MessageBox
		MessageBox(NULL, lpwErrorMessage, L"Error", MB_OK | MB_ICONERROR);
	}
}


const TCHAR* MySQL_API::computerID()
{
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD bufCharCount = INFO_BUFFER_SIZE;

	if (!GetComputerName(infoBuf, &bufCharCount))
	{
		return TEXT("GetComputerName");
	}
	return infoBuf;
}


const TCHAR* MySQL_API::nameUser()
{
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	if (GetUserName((TCHAR*)name, &size))
		return name;
}


std::string MySQL_API::convertSecondsToTimeString(long total_seconds)
{
	int hours = total_seconds / 3600;          // Вычисляем количество часов
	int minutes = (total_seconds % 3600) / 60; // Вычисляем количество минут
	int seconds = total_seconds % 60;          // Оставшиеся секунды

	// Используем строковый поток для форматирования строки
	std::ostringstream timeStream;
	timeStream << std::setfill('0') << std::setw(2) << hours << ":"
		<< std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << seconds;

	// Возвращаем строку с временем в формате HH:MM:SS
	return timeStream.str();
}


std::string MySQL_API::convertValue_to_String(Value var)
{
	std::stringstream myStreamString;
	myStreamString << var;
	string myString = myStreamString.str();
	return myString;
}


std::string MySQL_API::TimeNow() const
{
	auto start = std::chrono::system_clock::now();

	std::time_t start_time = std::chrono::system_clock::to_time_t(start);

	char* start_time_ch = std::ctime(&start_time);
	
	std::string start_time_res = "";

	for (size_t i = 0; i != strlen(start_time_ch); i++)
	{
		if (i >= 11 && i <= 18)
		{
			start_time_res += start_time_ch[i];
		}
	}
	//std::cout << start_time_res << std::endl;
	return start_time_res;
}


std::string MySQL_API::Data() const
{
	// Получаем текущее время
	auto now = std::chrono::system_clock::now();

	// Преобразуем в time_t
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	// Преобразуем в структуру tm
	std::tm* now_tm = std::localtime(&now_time_t);

	std::ostringstream oss;
	oss << std::put_time(now_tm, "%Y-%m-%d");

	// Получаем строку
	std::string date_str = oss.str();

	return date_str;
}

// Функция для конвертации std::string в std::wstring
std::wstring MySQL_API::stringToWString(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}



  