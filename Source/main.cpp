#include "../Headers/CheckProcess.h"

#include <tchar.h>

//Глобальные переменные 
NOTIFYICONDATA Icon = { 0 }; // Атрибут значка

// Объявление функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Стандартная обработка ошибок

// Главная функция 
int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	// Регистрация главного окна
	// Так как само окно отображаться не будет сделаем только основные свойства 
	WNDCLASSEX Wmain = { 0 };
	Wmain.cbSize = sizeof(WNDCLASSEX);
	Wmain.hInstance = instance;
	Wmain.lpszClassName = TEXT("Main");
	Wmain.lpfnWndProc = WndProc;
	RegisterClassEx(&Wmain);

	// Создание главного окна
	HWND window = CreateWindowEx(0, TEXT("Main"), NULL, 0, 0, 0, 0, 0, NULL, NULL, instance, NULL);

	// Создание значка 
	Icon.cbSize = sizeof(NOTIFYICONDATA);
	Icon.hWnd = window;
	Icon.uVersion = NOTIFYICON_VERSION;
	Icon.uCallbackMessage = WM_USER;
	Icon.hIcon = LoadIcon(NULL, IDI_SHIELD);
	Icon.uFlags = NIF_MESSAGE | NIF_ICON;
	Shell_NotifyIcon(NIM_ADD, &Icon);

	// Включение таймера
	MonitoringNewProcesses();
	SetTimer(window, 0, 5000, NULL);

	// Цикл обработки сообщений
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Удаление значка
	Shell_NotifyIcon(NIM_DELETE, &Icon);
	return 0;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		MonitoringNewProcesses();

		// Обработка сообщений от значка
	case WM_USER:
		if (lParam == WM_RBUTTONDOWN)
		{
			if (MessageBox(NULL, TEXT("Завершить работу?"), TEXT("Tray"), MB_YESNO) == IDYES)
				DestroyWindow(window);
		}
		break;
	case WM_DESTROY:
		KillTimer(window, 1);
		_endApp();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window, message, wParam, lParam);
	}
	return 0;
}