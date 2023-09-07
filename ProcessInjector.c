#include "injector.h"
#include <stdio.h>

int main() {
    DWORD dwProcessId = 12345; // Substitua pelo ID do processo alvo
    LPCSTR lpszDLLPath = "C:\\Caminho\\para\\sua.dll"; // Substitua pelo caminho da sua DLL

    BOOL bResult = InjectDLL(dwProcessId, lpszDLLPath);

    if (bResult) {
        printf("DLL injetada com sucesso!\n");
    } else {
        fprintf(stderr, "Falha ao injetar a DLL.\n");
    }

    return 0;
}
