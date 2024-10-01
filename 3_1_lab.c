#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>

// Функция для центрирования текста
void print_centered(const char* text, int color_code) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);  // Получение размера консоли
    int columns = w.ws_col;
    int len = strlen(text);
    int padding = (columns - len) / 2;
    
    printf("\033[%dm", color_code);  // Установка цвета
    for (int i = 0; i < padding; ++i) {
        printf(" ");
    }
    printf("%s\n", text);
    printf("\033[0m");  // Сброс цвета
}

// Функция для чтения и вывода содержимого файла
void read_file(int fd) {
    char buffer[1024];
    ssize_t bytes_read;

    // Чтение содержимого файла и вывод на экран
    while ((bytes_read = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytes_read] = '\0';
        print_centered(buffer, 36);  // Голубой цвет для содержимого
    }
}

int main() {
    const char* filename = "input.txt";
    int fd;

    // Проверка на наличие файла
    if (access(filename, F_OK) != 0) {
        print_centered("Файл не найден, завершение программы.", 31);  // Красный текст
        return 1;
    }

    while (1) {
        // Попытка открытия файла с блокировкой
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            print_centered("Ошибка при открытии файла.", 31);  // Красный текст
            return 1;
        }

        // Попытка наложить блокировку на файл
        if (flock(fd, LOCK_EX | LOCK_NB) == 0) {
            print_centered("Файл открыт успешно, выводим содержимое:", 32);  // Зеленый текст
            read_file(fd);

            // Снимаем блокировку и закрываем файл
            flock(fd, LOCK_UN);
            close(fd);
            break;
        } else if (errno == EWOULDBLOCK) {
            print_centered("Файл занят другим процессом, ждем освобождения...", 33);  // Желтый текст
            close(fd);
            sleep(2);  // Ждем 2 секунды перед повторной попыткой
        } else {
            print_centered("Ошибка при блокировке файла.", 31);  // Красный текст
            close(fd);
            return 1;
        }
    }
    return 0;
}