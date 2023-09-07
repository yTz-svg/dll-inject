#include <iostream>
#include <windows.h>

BOOL InjectDLL(DWORD dwProcessId, LPCSTR lpszDLLPath) {
    HANDLE  hProcess, hThread;
    LPVOID  lpBaseAddr, lpFuncAddr;
    DWORD   dwMemSize, dwExitCode;
    BOOL    bSuccess = FALSE;
    HMODULE hKernel32;

    if ((hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId))) {
        dwMemSize = lstrlen(lpszDLLPath) + 1;

        if ((lpBaseAddr = VirtualAllocEx(hProcess, NULL, dwMemSize, MEM_COMMIT, PAGE_READWRITE))) {
            if (WriteProcessMemory(hProcess, lpBaseAddr, lpszDLLPath, dwMemSize, NULL)) {
                hKernel32 = GetModuleHandle("kernel32.dll");
                if (hKernel32 != NULL) {
                    lpFuncAddr = GetProcAddress(hKernel32, "LoadLibraryA");
                    if (lpFuncAddr != NULL) {
                        if ((hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpFuncAddr, lpBaseAddr, 0, NULL))) {
                            WaitForSingleObject(hThread, INFINITE);
                            if (GetExitCodeThread(hThread, &dwExitCode)) {
                                bSuccess = (dwExitCode != 0) ? TRUE : FALSE;
                            }
                            CloseHandle(hThread);
                        }
                    }
                }
            }
            VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
        }
        CloseHandle(hProcess);
    }
    return bSuccess;
}

int main() {
    DWORD dwProcessId = 12345; // Substitua pelo ID do processo alvo
    LPCSTR lpszDLLPath = "C:\\Caminho\\para\\sua.dll"; // Substitua pelo caminho da sua DLL

    BOOL bResult = InjectDLL(dwProcessId, lpszDLLPath);

    if (bResult) {
        std::cout << "DLL injetada com sucesso!" << std::endl;
    } else {
        std::cerr << "Falha ao injetar a DLL." << std::endl;
    }

    return 0;
}
