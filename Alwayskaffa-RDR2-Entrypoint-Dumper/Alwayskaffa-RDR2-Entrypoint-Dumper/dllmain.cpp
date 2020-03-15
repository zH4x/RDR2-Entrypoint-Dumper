// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <vector>
#include <Psapi.h>
#include <iosfwd>
#include <fstream>
#include "dllmain.hpp"
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		std::ofstream file;
		file.open("RDR2_HASH_TO_ENTRYPOINTS_FULL.txt");
		if (!file.is_open() || !file.good()) {
			MessageBoxA(0, "Failed to open file!", "ERROR", MB_OK);
			return 0;
		}
		static auto base_address = (uintptr_t)(GetModuleHandleA(0));
		static auto native_base = memory::find_signature(0, "\x0F\xB6\xC1\x48\x8D\x15\x00\x00\x00\x00\x4C\x8B\xC9", "xxxxxx????xxx");
		static auto get_native_address = reinterpret_cast<uintptr_t(*)(uint64_t)>(native_base); // 0F B6 C1 48 8D 15 ? ? ? ? 4C 8B C9
		file << "static std::map<uintptr_t, DWORD> nativehash_to_address_table = {" << std::endl;
		for (auto native : native_dump_list_full)
			file << "{" << std::hex << "0x" << native.hash << ", " << std::hex << "0x" << (get_native_address(native.hash) - base_address) << " }, " << std::endl;

		file << "};" << std::endl;
		file.close();
		MessageBoxA(0, "Dumped entrypoints to game folder, Press OK to exit.", "SUCCESS", MB_OK);
		ExitProcess(ERROR_SUCCESS);
	}
	return 1;
}


