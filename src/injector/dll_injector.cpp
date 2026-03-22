#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int inject_dll(DWORD pid, char *dll_path)
{
    HANDLE process;
    LPVOID remote;
    HANDLE thread;
    LPTHREAD_START_ROUTINE load_library;

    if (!dll_path)
        return (1);

    process = OpenProcess(
        PROCESS_CREATE_THREAD |
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ,
        0,
    pid);
    if (!process)
        return (1);

    remote = VirtualAllocEx(process, 0, strlen(dll_path) + 1,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remote)
        return (CloseHandle(process), 1);

    if (!WriteProcessMemory(process, remote, dll_path,
            strlen(dll_path) + 1, NULL))
        return (VirtualFreeEx(process, remote, 0, MEM_RELEASE),
                CloseHandle(process), 1);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-function-type"

    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    load_library = (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32, "LoadLibraryA");

    #pragma GCC diagnostic pop

    thread = CreateRemoteThread(process, NULL, 0, load_library, remote, 0, NULL);
    if (!thread)
    {
        return (VirtualFreeEx(process, remote, 0, MEM_RELEASE),
                CloseHandle(process), 1);
        printf("CreateRemoteThread failed: %lu\n", GetLastError());
    }  

    DWORD exit_code;

    WaitForSingleObject(thread, INFINITE);
    GetExitCodeThread(thread, &exit_code);

    printf("LoadLibrary return: %lu\n", exit_code);
    printf("PID: %lu\n", pid);
    printf("DLL: %s\n", dll_path);
    printf("Process handle: %p\n", process);
    printf("Remote memory: %p\n", remote);
    printf("LoadLibrary addr: %p\n", load_library);

    VirtualFreeEx(process, remote, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(process);
    return (0);
}