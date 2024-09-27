#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    HANDLE hFile1, hFile2, hFile3;
    char filename[100];
    char buffer[8]; 
    DWORD bytesRead;
    LARGE_INTEGER position;

    printf("Введите имя файла для чтения: ");
    scanf("%s", filename);

    hFile1 = CreateFileA(
        filename,          
        GENERIC_READ,      
        FILE_SHARE_READ,   
        NULL,              
        OPEN_EXISTING,     
        FILE_ATTRIBUTE_NORMAL, 
        NULL);             

    if (hFile1 == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия файла (hFile1). Код ошибки: %lu\n", GetLastError());
        return EXIT_FAILURE;
    }

    if (!DuplicateHandle(
            GetCurrentProcess(),    
            hFile1,                 
            GetCurrentProcess(),    
            &hFile2,                
            0,                      
            FALSE,                  
            DUPLICATE_SAME_ACCESS)) {
        printf("Ошибка дублирования дескриптора (hFile2). Код ошибки: %lu\n", GetLastError());
        CloseHandle(hFile1);
        return EXIT_FAILURE;
    }

    hFile3 = CreateFileA(
        filename,          
        GENERIC_READ,      
        FILE_SHARE_READ,   
        NULL,              
        OPEN_EXISTING,     
        FILE_ATTRIBUTE_NORMAL, 
        NULL);             

    if (hFile3 == INVALID_HANDLE_VALUE) {
        printf("Ошибка повторного открытия файла (hFile3). Код ошибки: %lu\n", GetLastError());
        CloseHandle(hFile1);
        CloseHandle(hFile2);
        return EXIT_FAILURE;
    }

    position.QuadPart = 10;
    if (!SetFilePointerEx(hFile1, position, NULL, FILE_BEGIN)) {
        printf("Ошибка позиционирования на 10-ю позицию. Код ошибки: %lu\n", GetLastError());
        CloseHandle(hFile1);
        CloseHandle(hFile2);
        CloseHandle(hFile3);
        return EXIT_FAILURE;
    }

    printf("Дескриптор 1: %p\n", hFile1);
    printf("Дескриптор 2: %p\n", hFile2);
    printf("Дескриптор 3: %p\n", hFile3);

    for (int i = 0; i < 3; i++) {
        HANDLE current_handle = (i == 0) ? hFile1 : (i == 1) ? hFile2 : hFile3;

        if (ReadFile(current_handle, buffer, 7, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Данные из дескриптора %p: %s\n", current_handle, buffer);
        } else {
            printf("Ошибка чтения из дескриптора %p. Код ошибки: %lu\n", current_handle, GetLastError());
        }
    }

    CloseHandle(hFile1);
    CloseHandle(hFile2);
    CloseHandle(hFile3);

    return EXIT_SUCCESS;
}
