#include "pch.h"
#include "process.h"

struct DS3
{
	uintptr_t Tungsten;
	uintptr_t Semiconductor;
	uintptr_t ScrapMetal;
	uintptr_t SomaticGel;
	uintptr_t Transducer;
	uintptr_t RationSeals;

	std::vector<int> Tungsten_Offset = {0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDF4};
	std::vector<int> Semiconductor_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDF8 };
	std::vector<int> ScrapMetal_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xDFC };
	std::vector<int> SomaticGel_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE00 };
	std::vector<int> Transducer_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE04 };
	std::vector<int> RationSeals_Offset = { 0xDD3B8C, 0x8, 0xE4, 0x74, 0x8, 0x98, 0x80, 0xE08 };
	DS3(HANDLE hProcess, uintptr_t uiModuleBaseAddress)
	{
		Tungsten = FindDynamicAddress(hProcess, uiModuleBaseAddress, Tungsten_Offset);
		Semiconductor = FindDynamicAddress(hProcess, uiModuleBaseAddress, Semiconductor_Offset);
		ScrapMetal = FindDynamicAddress(hProcess, uiModuleBaseAddress, ScrapMetal_Offset);
		SomaticGel = FindDynamicAddress(hProcess, uiModuleBaseAddress, SomaticGel_Offset);
		Transducer = FindDynamicAddress(hProcess, uiModuleBaseAddress, Transducer_Offset);
		RationSeals = FindDynamicAddress(hProcess, uiModuleBaseAddress, RationSeals_Offset);
	}

	//Delete Copy constructor and assignment operator
	DS3(const DS3&) = delete;
	DS3& operator=(const DS3&) = delete;
};

int main(int argc, char **argv)
{
	//Get Process ID
	DWORD dwPID = GetProcessId(L"deadspace3.exe");
	if (dwPID)
	{
		//Get Module base address
		uintptr_t uiBA = GetModuleBaseAddress(L"deadspace3.exe", dwPID);
		if (uiBA)
		{
			std::cout << std::hex << "PID: " << dwPID << std::endl;
			std::cout << "Base Address: " << uiBA << std::dec << std::endl;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwPID);
			if (hProcess != INVALID_HANDLE_VALUE)
			{
				std::cout << "Success" << std::endl;
				
				//Instantiate Dynamic struct
				DS3 addresses = DS3(hProcess, uiBA);

				//Main Loop
				std::cout << "Press [F1] for Tungsten" << std::endl;
				std::cout << "Press [F2] for Tungsten" << std::endl;
				std::cout << "Press [F3] for Tungsten" << std::endl;
				std::cout << "Press [F4] for Tungsten" << std::endl;
				std::cout << "Press [F5] for Tungsten" << std::endl;
				std::cout << "Press [F6] for Tungsten" << std::endl;
				std::cout << "Press [F7] to exit" << std::endl;

				uintptr_t buffer;
				for (;;)
				{
					if (GetAsyncKeyState(VK_F1))
					{
						//Read Current then add then write back
						ReadProcessMemory(hProcess, (LPCVOID)addresses.Tungsten, &buffer, sizeof(buffer), 0);
						buffer += 500;
						WriteProcessMemory(hProcess, (LPVOID)addresses.Tungsten, &buffer, sizeof(buffer), 0);

					}
					if (GetAsyncKeyState(VK_F2))
					{

					}
					if (GetAsyncKeyState(VK_F3))
					{

					}
					if (GetAsyncKeyState(VK_F4))
					{

					}
					if (GetAsyncKeyState(VK_F5))
					{

					}
					if (GetAsyncKeyState(VK_F6))
					{

					}
					if (GetAsyncKeyState(VK_F7))
					{
						break;
					}


				}

			}
			CloseHandle(hProcess);
		}

	}

	std::cin.get();
	return 0;
}