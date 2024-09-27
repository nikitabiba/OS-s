#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd1, fd2, fd3;
    char filename[100];
    char buffer[8];
    int bytesRead;

    printf("Введите имя файла для чтения: ");
    scanf("%s", filename);

    // Открываем файл для чтения
    fd1 = open(filename, O_RDONLY);
    if (fd1 == -1) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    // Дублируем дескриптор
    fd2 = dup(fd1);
    if (fd2 == -1) {
        perror("Ошибка дублирования дескриптора");
        close(fd1);
        return EXIT_FAILURE;
    }

    // Открываем файл снова, получая третий дескриптор
    fd3 = open(filename, O_RDONLY);
    if (fd3 == -1) {
        perror("Ошибка повторного открытия файла");
        close(fd1);
        close(fd2);
        return EXIT_FAILURE;
    }

    // Позиционируем дескриптор fd1 на 10-ю позицию
    lseek(fd1, 10, SEEK_SET);

    printf("Дескриптор 1: %d\n", fd1);
    printf("Дескриптор 2: %d\n", fd2);
    printf("Дескриптор 3: %d\n", fd3);

    // Чтение и вывод 7 символов с использованием каждого дескриптора
    for (int i = 0; i < 3; i++) {
        int current_fd = (i == 0) ? fd1 : (i == 1) ? fd2 : fd3;

        bytesRead = read(current_fd, buffer, 7);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Данные из дескриптора %d: %s\n", current_fd, buffer);
        } else {
            printf("Ошибка чтения из дескриптора %d\n", current_fd);
        }
    }

    // Закрываем дескрипторы
    close(fd1);
    close(fd2);
    close(fd3);
    return EXIT_SUCCESS;
}
