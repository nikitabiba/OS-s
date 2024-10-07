#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Функция для центрирования текста
void print_centered(const char* text, int color_code) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Получаем размер консоли
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    } else {
        columns = 80; // Если не удалось получить размер, используем значение по умолчанию
    }

    int len = strlen(text);
    int padding = (columns - len) / 2;

    // Установка цвета
    SetConsoleTextAttribute(hConsole, color_code);

    for (int i = 0; i < padding; ++i) {
        printf(" ");
    }
    printf("%s\n", text);

    // Сброс цвета
    SetConsoleTextAttribute(hConsole, 7);
}

// Функция для чтения и вывода содержимого файла
void read_file(HANDLE hFile) {
    char buffer[1024];
    DWORD bytes_read;

    // Чтение содержимого файла и вывод на экран
    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) {
        buffer[bytes_read] = '\0';
        print_centered(buffer, 11);  // Голубой цвет для содержимого
    }
}

int main() {
    const char* filename = "input.txt";
    HANDLE hFile;
    DWORD dwWaitResult;
    OVERLAPPED overlapped = { 0 };

    // Попытка открытия файла
    hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,  // Позволяет другим процессам читать и писать в файл
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        print_centered("File not found, end of program.", 12);  // Красный текст
        return 1;
    }

    while (1) {
        // Попытка наложить блокировку на файл
        dwWaitResult = LockFileEx(
            hFile,
            LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
            0,
            MAXDWORD,
            MAXDWORD,
            &overlapped
        );

        if (dwWaitResult) {
            print_centered("File opened successfully, printing text:", 10);  // Зеленый текст
            read_file(hFile);

            // Снимаем блокировку
            UnlockFile(hFile, 0, 0, MAXDWORD, MAXDWORD);
            CloseHandle(hFile);
            break;
        } else if (GetLastError() == ERROR_LOCK_VIOLATION) {
            print_centered("File locked, waiting for unlock...", 14);  // Желтый текст
            Sleep(2000);  // Ждем 2 секунды перед повторной попыткой
        } else {
            print_centered("Error in locking file.", 12);  // Красный текст
            CloseHandle(hFile);
            return 1;
        }
    }
    return 0;
}