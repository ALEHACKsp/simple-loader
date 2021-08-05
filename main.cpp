#include <iostream>
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS

#define vmp_protect_start(s) VMProtectBeginUltra(s)
#define vmp_detect_debuggers(a) VMProtectIsDebuggerPresent(a)
#define vmp_end VMProtectEnd

#include "auth.hpp"
#include "color_win.hpp"
#include "vmp_api.h"
#include "encrypt_chars.hpp"
#include "injection.hpp"
#include "dll_library.h"
#include <locale>
#include <random>

injec* inject;
char inj;

using namespace colorwin;

DWORD get_pid(char* ProcName)
{
	PROCESSENTRY32 lppe;
	long PID = 0, Result = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnap)
	{
		lppe.dwSize = sizeof(PROCESSENTRY32);
		Result = Process32First(hSnap, &lppe);
		while (Result)
		{
			if (strcmp(lppe.szExeFile, ProcName) == NULL)
			{
				PID = lppe.th32ProcessID;
				break;
			}
			Result = Process32Next(hSnap, &lppe);
		}
		CloseHandle(hSnap);
	}
	return PID;
}

void randomizetitle()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> distr(0, 51);
	std::string name = "";
	char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	for (int i = 0; i < 15; ++i)
	{
		name = name + alphabet[distr(mt)];
		SetConsoleTitleA(name.c_str());
	}

}


static void printf_sucess_lic() {
	std::cout << "" << std::endl;
	std::cout << "  " << color(blue) << "EN:" << color(grey) << "" << strenc(" you have license, your days ")
		<< color(blue) << auth::get_days_user() <<  color(grey) << strenc(" for use cheat.") << std::endl;

	std::cout << "  " << color(blue) << "RU:" << color(grey) << "" << strenc(" âû èìååòå ëèöåíçèþ, âàøè äíè ")
		<< color(blue) << auth::get_days_user() << color(grey) << strenc(" äëÿ èñïîëüçîâàíèÿ ïðîäóêòà.") << std::endl;

	Sleep(2555);

	std::cout << "" << std::endl;
	std::cout << "  " << color(blue) << "EN:" << color(grey) << "" << strenc(" write ")
		<< color(blue) << "/inject" << color(grey) << strenc(" for inject cheat.") << std::endl;

	std::cout << "  " << color(blue) << "RU:" << color(grey) << "" << strenc(" íàïèøèòå ")
		<< color(blue) << "/inject" << color(grey) << strenc(" äëÿ âíåäðåíèÿ ïðîäóêòà.") << std::endl;


	std::cin >> inj;
	
	if (inj == L'/inject') {
		DWORD pid = 0;
		Sleep(1000);

		pid = get_pid((char*)"csgo.exe");
		typedef unsigned char       BYTE;


		inject->manualmap(pid, (PBYTE)kernel32dll); // BYTE name[] { ... }; 

		Sleep(2222);
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "  " << color(red) << "EN:" << color(grey) << strenc(" unknown command") << std::endl;
		std::cout << "  " << color(red) << "RU:" << color(grey) << strenc(" íåèçâåñòíàÿ êîìàíäà") << std::endl;
		Sleep(1200);
	}

}

BOOL is_running_admin_mode()
{
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:
	// Centralized cleanup for all allocated resources.
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	// Throw the error if something failed in the function.
	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	return fIsRunAsAdmin;
}

static void check_options() {
	if (is_running_admin_mode() == TRUE) {

	}
	else {
		Sleep(1500);

		std::cout << "" << std::endl;
		std::cout << "  " << color(red) << "EN:" << color(grey) << "" << strenc(" run as an ") << color(red)
			<< "admin" << std::endl;

		std::cout << "  " << color(red) << "RU:" << color(grey) << "" << strenc(" çàïóñòèòå îò èìåíè ") << color(red)
			<< "àäìèíà" << std::endl;

		Sleep(2500);
		exit(1);
	}
}

static void printf_error_lic() {
	std::cout << "" << std::endl;
	std::cout << "  " << color(red) << "EN:" << color(grey) << "" << strenc(" you dont have ") << color(red) << "license"  << color(grey)
		 << strenc(" to use cheat.") << std::endl;

	std::cout << "  " << color(red) << "RU:" << color(grey) << "" << strenc(" âû íå èìååòå ") << color(red) << "ëèöåíçèè" << color(grey)
		<< strenc(" äëÿ èñïîëüçîâàíèÿ ïðîäóêòà.") << std::endl;
	Sleep(2555);

	exit(1);
}

static void set_locale_translation() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

static void attach_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

int main() {
	vmp_protect_start("main");
	vmp_detect_debuggers(true);
	randomizetitle();
	set_locale_translation();
	attach_console();
	check_options();
	if (auth::check_actuality_license() == true) { printf_sucess_lic(); }
	else { printf_error_lic(); }
	return 0;
	vmp_end();
}
