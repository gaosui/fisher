#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>

const char minecraft[13] = "Minecraft 1.";
const int f_id = 11;

bool is_hwnd_valid = false;
HWND hwnd{};
bool run = false;

void system_pause()
{
	std::cout << "Press enter to continue...\n";
	std::cin.get();
};

BOOL CALLBACK callback(_In_ HWND handle, _In_ LPARAM p)
{
	char buffer[13];
	GetWindowTextA(handle, buffer, 13);
	for (int i = 0; i < 13; i++)
	{
		if (buffer[i] != minecraft[i]) return TRUE;
	}
	std::cout << "Minecraft window found\n";
	is_hwnd_valid = true;
	hwnd = handle;
	return FALSE;
}

void click()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (run)
		{
			SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
		}
	}
}

int main()
{
	if (RegisterHotKey(NULL, f_id, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x46))
	{
		std::cout << "Ctrl+Alt+F registered\n";
	}
	else
	{
		std::cout << "Register Ctrl+Alt+F failed\n";
		system_pause();
	}

	while (!is_hwnd_valid)
	{
		std::cout << "Searching for Minecraft window\n";
		EnumWindows(&callback, 0);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::thread t{ click };

	std::cout << "Idle\n";

	MSG msg;
	bool fishing = false;

	while (true)
	{
		if (GetMessage(&msg, NULL, 0, 0) == -1)
		{
			std::cout << "Get message error\n";
			system_pause();
			break;
		}

		if (msg.message == WM_HOTKEY && msg.wParam == f_id)
		{
			fishing = !fishing;
			run = is_hwnd_valid && fishing;

			if (run)
				std::cout << "Fishing...\n";
			else
				std::cout << "Idle\n";
		}
	}

	return 0;
}
