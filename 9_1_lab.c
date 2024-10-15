#include <windows.h>
#include <stdio.h>

#define SHARED_MEMORY_NAME "SharedMemoryExample"
#define EVENT_NAME "SharedMemoryEvent"
#define SHARED_MEMORY_SIZE 256

int main() {
    // Создаем событие для синхронизации
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
    if (hEvent == NULL) {
        printf("Failed to create event. Error: %d\n", GetLastError());
        return 1;
    }

    // Создаем разделяемую память
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHARED_MEMORY_SIZE, SHARED_MEMORY_NAME);
    if (hMapFile == NULL) {
        printf("Could not create file mapping object. Error: %d\n", GetLastError());
        CloseHandle(hEvent);
        return 1;
    }

    // Получаем указатель на разделяемую память
    LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEMORY_SIZE);
    if (pBuf == NULL) {
        printf("Could not map view of file. Error: %d\n", GetLastError());
        CloseHandle(hMapFile);
        CloseHandle(hEvent);
        return 1;
    }

    printf("Shared memory created at address: %p\n", pBuf);

    // Ждем 10-15 секунд
    Sleep(10000);

    // Записываем сообщение в разделяемую память
    const char* message = "Hello from the first process!";
    sprintf((char*)pBuf, "%s", message);
    printf("Message written to shared memory: %s\n", message);

    // Устанавливаем событие
    SetEvent(hEvent);

    // Выделение дополнительной памяти через VirtualAlloc
    LPVOID extraMemory = VirtualAlloc(NULL, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (extraMemory == NULL) {
        printf("Failed to allocate additional memory. Error: %d\n", GetLastError());
    } else {
        printf("Additional memory allocated at address: %p\n", extraMemory);

        // Записываем символы с пропуском 399 байт
        for (int i = 0; i < 15; i++) {
            char ch = 'a' + i;
            size_t offset = i * 400;
            if (offset < 1000) {
                *((char*)extraMemory + offset) = ch;
                printf("Character '%c' written at address: %p\n", ch, (char*)extraMemory + offset);
            }
        }
    }

    // Освобождение ресурсов
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hEvent);
    if (extraMemory) {
        VirtualFree(extraMemory, 0, MEM_RELEASE);
    }

    return 0;
}