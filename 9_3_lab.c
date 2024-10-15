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
    // Создаем семафор
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    // Создаем разделяемую память
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return 1;
    }
    ftruncate(shm_fd, SHM_SIZE);

    // Получаем указатель на разделяемую память
    char *shm_ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return 1;
    }

    printf("Shared memory created at address: %p\n", shm_ptr);

    // Ждем 10-15 секунд
    sleep(10);

    // Записываем сообщение в разделяемую память
    const char* message = "Hello from the first process!";
    sprintf(shm_ptr, "%s", message);
    printf("Message written to shared memory: %s\n", message);

    // Увеличиваем значение семафора, уведомляя второй процесс
    sem_post(sem);

    // Освобождение ресурсов
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);

    return 0;
}