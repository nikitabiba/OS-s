#include <windows.h>
#include <stdio.h>

#define SHARED_MEMORY_NAME "SharedMemoryExample"
#define EVENT_NAME "SharedMemoryEvent"
#define SHARED_MEMORY_SIZE 256

int main() {
    // Открываем существующее событие
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (hEvent == NULL) {
        printf("Failed to open event. Error: %d\n", GetLastError());
        return 1;
    }

    // Открываем разделяемую память
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEMORY_NAME);
    if (hMapFile == NULL) {
        printf("Could not open file mapping object. Error: %d\n", GetLastError());
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

    printf("Shared memory opened at address: %p\n", pBuf);

    // Ожидание записи в разделяемую память
    printf("Waiting for message...\n");

    // Ожидание сигнала события
    WaitForSingleObject(hEvent, INFINITE);

    // Чтение сообщения из разделяемой памяти
    printf("Message received from shared memory: %s\n", (char*)pBuf);

    // Освобождение ресурсов
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(hEvent);

    return 0;
}
