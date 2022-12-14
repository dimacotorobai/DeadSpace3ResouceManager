#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

//Function Prototypes
DWORD     GetProcessId(const wchar_t* szProcessName);
uintptr_t GetModuleBaseAddress(const wchar_t* szModuleName, DWORD dwProcessId);
uintptr_t FindDynamicAddress(HANDLE hProcess, uintptr_t uiModuleBaseAddress, const std::vector<int>& Offsets);
void      KeyUp(BYTE vk);

//Offsets
struct DS3 {
	std::vector<int> Tungsten_Offset      = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDF4 };
	std::vector<int> Semiconductor_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDF8 };
	std::vector<int> ScrapMetal_Offset    = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDFC };
	std::vector<int> SomaticGel_Offset    = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE00 };
	std::vector<int> Transducer_Offset    = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE04 };
	std::vector<int> RationSeals_Offset   = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE08 };
};

int main(int argc, char **argv) {
	//Set console name
	SetConsoleTitle(L"Dead Space™ 3 Resource Manager");

	//Get Process ID
	DWORD dwPID = GetProcessId(L"deadspace3.exe");
	if (!dwPID) return EXIT_FAILURE;

	//Get the Module Base Address
	uintptr_t uiBA = GetModuleBaseAddress(L"deadspace3.exe", dwPID);
	if (!uiBA) return EXIT_FAILURE;

	//Attach Handle to the Process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwPID);
	if (hProcess == INVALID_HANDLE_VALUE) return EXIT_FAILURE;

	//Print Process Info
	std::cout << "Process Name: deadspace3.exe\n"
	          << "PID: 0x" << std::hex << dwPID << '\n'
	          << "Base Address: 0x" << uiBA << std::dec << "\n\n";

	//Instantiate Dynamic struct
	DS3 offsets;

	//Main Loop
	std::cout << "[F1] Tungsten\n"
	          << "[F2] Semicoductor\n"
	          << "[F3] Scrap Metal\n"
	          << "[F4] Somatic Gel\n"
	          << "[F5] Transducer\n"
	          << "[F6] Ration Seals\n"
	          << "[F7] Exit\n";

	for (;;) {
		uintptr_t buffer;

		if (GetAsyncKeyState(VK_F1)) {
			KeyUp(VK_F1);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.Tungsten_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F2)) {
			KeyUp(VK_F2);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.Semiconductor_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F3)) {
			KeyUp(VK_F3);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.ScrapMetal_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F4)) {
			KeyUp(VK_F4);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.SomaticGel_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F5)) {
			KeyUp(VK_F5);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.Transducer_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F6)) {
			KeyUp(VK_F6);
			uintptr_t address = FindDynamicAddress(hProcess, uiBA, offsets.RationSeals_Offset);
			ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(buffer), 0);
			buffer += 500;
			WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), 0);
		}
		if (GetAsyncKeyState(VK_F7)) {
			break;
		}

		Sleep(50);
	}

	CloseHandle(hProcess);
	return EXIT_SUCCESS;
}

DWORD GetProcessId(const wchar_t* szProcessName) {
	//Take Snapshot of all processess
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

	//ProcessEntry and Traverse the Snapshot
	DWORD dwProcessId = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (!_wcsicmp(szProcessName, pe32.szExeFile)) {
				dwProcessId = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return dwProcessId;
}

uintptr_t GetModuleBaseAddress(const wchar_t* szModuleName, DWORD dwProcessId) {
	// Take snapshot of all modules in the process
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

	//Iterate through all modules
	uintptr_t uiBaseAddress = 0;
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);

	if (Module32First(hSnapshot, &me32)) {
		do {
			if (!_wcsicmp(szModuleName, me32.szModule)) {
				uiBaseAddress = (uintptr_t)me32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &me32));
	}

	CloseHandle(hSnapshot);
	return uiBaseAddress;
}

uintptr_t FindDynamicAddress(HANDLE hProcess, uintptr_t uiModuleBaseAddress, const std::vector<int>& Offsets) {
	uintptr_t uiDynamicAddress = uiModuleBaseAddress + Offsets[0];
	for (int i = 1; i < Offsets.size(); i++) {
		ReadProcessMemory(hProcess, (LPCVOID)uiDynamicAddress, &uiDynamicAddress, sizeof(uiDynamicAddress), 0);
		uiDynamicAddress += Offsets[i];
	}

	return uiDynamicAddress;
}

void KeyUp(BYTE vk) {
	return keybd_event(
		vk,
		MapVirtualKey(vk, MAPVK_VK_TO_VSC),
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0
	);
}