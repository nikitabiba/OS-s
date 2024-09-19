#include <stdio.h>
#include <unistd.h>  // Для функции isatty()

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];

    // Проверяем, является ли стандартный ввод консолью
    if (isatty(fileno(stdin))) {
        // Ввод с консоли, выводим приглашение на стандартный вывод
        fprintf(stderr, "Input your text: ");
    }

    // Чтение данных из стандартного ввода
    ssize_t bytesRead = read(fileno(stdin), buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
        perror("Error");
        return 1;
    }

    // Завершаем строку для корректного вывода
    buffer[bytesRead] = '\0';

    // Проверяем, является ли стандартный вывод консолью
    if (isatty(fileno(stdout))) {
        // Стандартный вывод — это консоль, выводим результат на стандартный вывод
        printf("Your input: %s", buffer);
    } else {
        // Стандартный вывод перенаправлен, выводим приглашение в консоль принудительно через stderr
        fprintf(stderr, "Your input: %s", buffer);
    }

    return 0;
}
