#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[100];

    printf("Введите имя файла для записи: ");
    scanf("%s", filename);

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    printf("Введите текст (для завершения нажмите Ctrl+Z):\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, file);
    }

    fclose(file);
    printf("Данные успешно записаны в файл '%s'.\n", filename);
    return EXIT_SUCCESS;
}
