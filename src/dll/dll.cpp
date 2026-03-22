#include <windows.h>

DWORD ft_putchar(char c)
{
    HANDLE h;
    DWORD len;

    h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(h, &c, 1, &len, NULL);
    return (len);
}

DWORD ft_putstr(char *str)
{
    DWORD i;

    i = 0;
    while (str[i])
    {
        ft_putchar(str[i]);
        i++;
    }
    return (i);
}

DWORD WINAPI thread_func(LPVOID param)
{
    MessageBoxA(0, "Injected!", "OK", 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, thread_func, 0, 0, 0);
    }
    return 1;
}