#pragma once

int GetProcessId(const wchar_t *szProcessName)
{
	DWORD dwProcessId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		//ProcessEntry and Traverse the Snapshot
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		if (Process32First(hSnapshot, &pe32))
		{
			do
			{
				if (!_wcsicmp(szProcessName, pe32.szExeFile))
				{
					dwProcessId = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		}

	}

	CloseHandle(hSnapshot);
	return dwProcessId;
}

uintptr_t GetModuleBaseAddress(const wchar_t *szModuleName, DWORD dwProcessId)
{
	uintptr_t uiBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me32;
		me32.dwSize = sizeof(me32);

		if (Module32First(hSnapshot, &me32))
		{
			do
			{
				if (!_wcsicmp(szModuleName, me32.szModule))
				{
					uiBaseAddress = (uintptr_t)me32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &me32));
		}
	}

	CloseHandle(hSnapshot);
	return uiBaseAddress;
}

uintptr_t FindDynamicAddress(HANDLE hProcess, uintptr_t uiModuleBaseAddress, std::vector<int> Offsets)
{
	uintptr_t uiDynamicAddress = uiModuleBaseAddress + Offsets[0];
	for (int i = 1; i < Offsets.size(); i++)
	{
		ReadProcessMemory(hProcess, (LPCVOID)uiDynamicAddress, &uiDynamicAddress, sizeof(uiDynamicAddress), 0);
		uiDynamicAddress += Offsets[i];
	}

	return uiDynamicAddress;
}