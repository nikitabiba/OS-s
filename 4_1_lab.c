#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Функция для перемещения курсора в центр строки с учетом длины текста
void move_to_center(const char *text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int consoleWidth, textLength, xPosition, yPosition;

    // Получаем информацию о консоли
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;  // Ширина консоли
    textLength = strlen(text);  // Длина текста

    // Вычисляем координаты для центрирования текста
    xPosition = (consoleWidth - textLength) / 2;
    yPosition = csbi.dwCursorPosition.Y;  // Текущая строка

    // Устанавливаем позицию курсора
    COORD pos = { (SHORT)xPosition, (SHORT)yPosition };
    SetConsoleCursorPosition(hConsole, pos);
}

// Функция для вывода цветного текста по центру
void print_colored_centered(const char *text, WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    move_to_center(text);  // Перемещаем курсор в центр для этого текста
    SetConsoleTextAttribute(hConsole, color);  // Установка цвета
    printf("%s\n", text);
    SetConsoleTextAttribute(hConsole, 7);  // Сброс цвета к белому
}

// Главная функция программы
int main(int argc, char *argv[]) {
    const char *filename = "input.txt";
    const WORD green = 2;   // Зеленый цвет
    const WORD red = 4;     // Красный цвет
    const WORD yellow = 6;  // Желтый цвет

    // Открытие файла
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,  // Разрешаем другим процессам читать и писать в файл
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        print_colored_centered("Cannot open file!", red);
        return 1;
    }

    // Попытка установить блокировку
    print_colored_centered("Trying to lock file...", yellow);

    OVERLAPPED ov = { 0 };
    if (!LockFileEx(
            hFile,
            LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
            0,
            MAXDWORD,
            MAXDWORD,
            &ov)) {
        print_colored_centered("Lock is already set, waiting...", red);

        // Ожидание до тех пор, пока блокировка не станет доступной
        if (!LockFileEx(
                hFile,
                LOCKFILE_EXCLUSIVE_LOCK,
                0,
                MAXDWORD,
                MAXDWORD,
                &ov)) {
            print_colored_centered("Cannot lock file!", red);
            CloseHandle(hFile);
            return 1;
        }
    }

    // Блокировка установлена
    print_colored_centered("Lock is successfully set!", green);

    // Чтение данных из файла и вывод на экран
    char buffer[256];
    DWORD bytesRead;
    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';  // Завершение строки
        print_colored_centered(buffer, green);
    }

    // Задержка на 7-8 секунд
    Sleep(7000 + (rand() % 2000));  // Sleep принимает миллисекунды

    // Снятие блокировки
    UnlockFile(hFile, 0, 0, MAXDWORD, MAXDWORD);
    print_colored_centered("Lock is down!", green);

    CloseHandle(hFile);
    return 0;
}
