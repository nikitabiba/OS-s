#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[100];

    printf("Введите имя файла для записи: ");
    scanf("%s", filename);

    // Открываем файл для записи
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    // Чтение текста из стандартного ввода и запись в файл
    printf("Введите текст (для завершения нажмите Ctrl+D):\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, file);
    }

    // Закрываем файл
    fclose(file);
    printf("Данные успешно записаны в файл '%s'.\n", filename);
    return EXIT_SUCCESS;
}
