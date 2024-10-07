#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define FILENAME "input.txt"
#define MAX_LINES 1000
#define MAX_LINE_LEN 256

// Структура для хранения текста
char text[MAX_LINES][MAX_LINE_LEN];

// Функция для вывода текста на консоль
void printText(int lines) {
    system("cls");  // Очищаем экран
    for (int i = 0; i < lines; i++) {
        printf("%s", text[i]);
    }
}

// Функция для чтения текстового файла
int readFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error: cannot open file.\n");
        return 0;
    }

    int lines = 0;
    while (fgets(text[lines], MAX_LINE_LEN, file) != NULL && lines < MAX_LINES) {
        lines++;
    }
    fclose(file);
    return lines;
}

// Функция для обработки нажатий мыши
void handleMouseClick(int x, int y, int lines) {
    if (x >= 0 && y >= 0 && y < lines) {
        int len = strlen(text[y]);
        if (x < len && text[y][x] != ' ' && text[y][x] != '\n') {
            printf("\nClick: Symbol '%c' at position (row: %d, column: %d)\n", text[y][x], y + 1, x + 1);
        }
    }
}

int main() {
    // Чтение файла
    int lines = readFile();
    if (lines == 0) return 1;

    // Выводим текст на консоль
    printText(lines);

    // Основной цикл для обработки событий
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hStdin, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    INPUT_RECORD input;
    DWORD count;

    while (1) {
        ReadConsoleInput(hStdin, &input, 1, &count);

        if (input.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouse = input.Event.MouseEvent;
            if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                int x = mouse.dwMousePosition.X;
                int y = mouse.dwMousePosition.Y;
                handleMouseClick(x, y, lines);
            }
            if (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                break;  // Выход по правой кнопке мыши
            }
        }
    }

    return 0;
}
