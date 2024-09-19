#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main() {
    // Получаем хэндлы стандартного ввода и вывода
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hStdin == INVALID_HANDLE_VALUE || hStdout == INVALID_HANDLE_VALUE) {
        printf("Error.\n");
        return 1;
    }

    // Выводим значения хэндлов с комментариями
    printf("Stdin handle: %p\n", hStdin);
    printf("Stdout handle: %p\n", hStdout);

    // Буфер для хранения 1данных
    char buffer[BUFFER_SIZE];
    DWORD bytesRead, bytesWritten;

    // Вывод приглашения для ввода
    const char *prompt = "Input your text: ";
    WriteFile(hStdout, prompt, (DWORD)strlen(prompt), &bytesWritten, NULL);

    // Чтение текста с помощью ReadFile
    if (!ReadFile(hStdin, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
        printf("Error.\n");
        return 1;
    }

    // Завершаем строку для корректного вывода
    buffer[bytesRead] = '\0';

    // Сообщаем, что введен текст
    const char *message = "Your input: ";
    WriteFile(hStdout, message, (DWORD)strlen(message), &bytesWritten, NULL);

    // Вывод введенного текста
    WriteFile(hStdout, buffer, bytesRead, &bytesWritten, NULL);

    return 0;
}