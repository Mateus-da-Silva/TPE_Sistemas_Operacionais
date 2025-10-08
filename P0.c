#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Cria o processo P1
    if (!CreateProcess(
            NULL,
            "p1.exe",
            NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Erro ao criar processo P1.\n");
        return 1;
    }

    
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    printf("\nP1 finalizado com código %ld\n", exitCode);
    printf("Processo de enriquecimento concluído com sucesso!\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}