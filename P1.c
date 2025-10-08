#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define NUM_THREADS 5
#define MAX_IDS 1000

int ids[MAX_IDS];
int total_ids = 0;
int current_index = 0;
HANDLE lock; 


void fake_api(int id, char *response) {
    double valor = (rand() % 10000) / 100.0;
    sprintf(response, "{\"id\":%d,\"status\":\"ok\",\"valor\":%.2f}", id, valor);
}


DWORD WINAPI thread_func(LPVOID param) {
    int thread_id = *(int*)param;
    FILE *log = fopen("logs.txt", "a");
    if (!log) return 0;

    while (1) {
        WaitForSingleObject(lock, INFINITE); 
        if (current_index >= total_ids) {
            ReleaseMutex(lock);
            break;
        }
        int id = ids[current_index++];
        ReleaseMutex(lock);

        char resposta[128];
        fake_api(id, resposta);

        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char data[64];
        strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(log, "Thread %d, ID %d, Resposta %s, Data %s\n",
                thread_id, id, resposta, data);
        fflush(log);

        Sleep(10); 
    }

    fclose(log);
    return 0;
}

int main() {
    srand(time(NULL));
    lock = CreateMutex(NULL, FALSE, NULL);

    FILE *f = fopen("lista_ids.txt", "r");
    if (!f) {
        printf("Arquivo lista_ids.txt não encontrado.\n");
        return 1;
    }

    
    while (fscanf(f, "%d", &ids[total_ids]) != EOF && total_ids < MAX_IDS)
        total_ids++;
    fclose(f);

    // Cria as threads
    HANDLE threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, thread_func, &thread_ids[i], 0, NULL);
    }

    // Espera todas as threads terminarem
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
    CloseHandle(lock);

    printf("P1 finalizado com sucesso.\n");
    return 0;
}