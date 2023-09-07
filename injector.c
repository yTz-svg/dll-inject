#include "injector.h"

BOOL InjectDLL(DWORD dwProcessId, LPCSTR lpszDLLPath) {
    HANDLE hProcess, hThread;
    LPVOID lpBaseAddr, lpFuncAddr;
    DWORD dwMemSize, dwExitCode;
    BOOL bSuccess = FALSE;
    HMODULE hKernel32;

    hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);
    
    if (hProcess == NULL) {
        fprintf(stderr, "Erro ao abrir o processo alvo. Código de erro: %lu\n", GetLastError());
        return FALSE;
    }

    dwMemSize = lstrlen(lpszDLLPath) + 1;

    lpBaseAddr = VirtualAllocEx(hProcess, NULL, dwMemSize, MEM_COMMIT, PAGE_READWRITE);

    if (lpBaseAddr == NULL) {
        fprintf(stderr, "Erro ao alocar memória no processo alvo. Código de erro: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }

    if (!WriteProcessMemory(hProcess, lpBaseAddr, lpszDLLPath, dwMemSize, NULL)) {
        fprintf(stderr, "Erro ao escrever na memória do processo alvo. Código de erro: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    hKernel32 = GetModuleHandle("kernel32.dll");

    if (hKernel32 == NULL) {
        fprintf(stderr, "Erro ao carregar a biblioteca kernel32.dll no processo alvo. Código de erro: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    lpFuncAddr = GetProcAddress(hKernel32, "LoadLibraryA");

    if (lpFuncAddr == NULL) {
        fprintf(stderr, "Erro ao obter o endereço da função LoadLibraryA no kernel32.dll. Código de erro: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpFuncAddr, lpBaseAddr, 0, NULL);

    if (hThread == NULL) {
        fprintf(stderr, "Erro ao criar um thread remoto no processo alvo. Código de erro: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);

    if (!GetExitCodeThread(hThread, &dwExitCode)) {
        fprintf(stderr, "Erro ao obter o código de saída do thread remoto. Código de erro: %lu\n", GetLastError());
        bSuccess = FALSE;
    } else {
        bSuccess = (dwExitCode != 0) ? TRUE : FALSE;
    }

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return bSuccess;
}
