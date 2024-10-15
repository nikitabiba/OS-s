#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"
#define SHM_SIZE 256

int main() {
    // Открываем существующий семафор
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    // Открываем разделяемую память
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        sem_close(sem);
        return 1;
    }

    // Получаем указатель на разделяемую память
    char *shm_ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        sem_close(sem);
        return 1;
    }

    printf("Shared memory opened at address: %p\n", shm_ptr);

    // Ожидание сообщения в разделяемой памяти
    printf("Waiting for message...\n");

    // Ожидание увеличения семафора
    sem_wait(sem);

    // Чтение сообщения из разделяемой памяти
    printf("Message received from shared memory: %s\n", shm_ptr);

    // Освобождение ресурсов
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);

    return 0;
}