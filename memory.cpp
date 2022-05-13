#include "memory.h"
#include "tlhelp32.h"

using namespace std;

process_id_t g_process_id = 0;
address_t g_module_address = 0;

process_id_t process::get_process_id(const string process_name) {
    wstring wide_process_name(process_name.begin(), process_name.end());

    auto* snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W pe { pe.dwSize = sizeof(PROCESSENTRY32W) };

    if (!Process32FirstW(snapshot, &pe)) {
        CloseHandle(snapshot);
        return 0;
    }

    do {
        if (!wcscmp(pe.szExeFile, wide_process_name.c_str())) {
            CloseHandle(snapshot);
            return pe.th32ProcessID;
        }
    } while (Process32NextW(snapshot, &pe));

    CloseHandle(snapshot);
    return 0;
}

/*
 * ***************************
 * Add Your Own Implementation
 * ***************************
 */