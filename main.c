#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Estructura de datos compartida
typedef struct {
    int frecuencia;
    int tiempo;
    int frecuencia_generador;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int tick;
} Configuracion;

// Función del hilo Clock
void* clock_thread(void* arg) {
    Configuracion* config = (Configuracion*)arg;
    while (1) {
        pthread_mutex_lock(&config->mutex);
        config->tick++;
        pthread_cond_signal(&config->cond);
        pthread_mutex_unlock(&config->mutex);
    }
    pthread_exit(NULL);
}
// Función del hilo Timer
void* timer_thread(void* arg) {
    Configuracion* config = (Configuracion*)arg;
    while (1) {
        pthread_mutex_lock(&config->mutex);
        while (config->tick != config->tiempo ) {
            pthread_cond_wait(&config->cond, &config->mutex);
        }
        config->tick=0;
        printf("Han pasado: %d segundos\n", config->tiempo);
        pthread_mutex_unlock(&config->mutex);
    }
    pthread_exit(NULL);
}
// Función del hilo Process Generator
/*void* process_generator_thread(void* arg) {
    Configuracion* config = (Configuracion*)arg;
    while (1) {
        pthread_mutex_lock(&config->mutex);
        printf("Generando proceso en tick: %d\n", config->tick);
        pthread_mutex_unlock(&config->mutex);
        sleep(1000000 / config->frecuencia_generador); // Simula la generación de procesos
    }
    pthread_exit(NULL);
}*/

int main(int argc, char *argv[]) {

        if (argc < 4) {
        printf("Parametros incorrectos: (Frecuencia hz) (Tiempo del temporizador) (Frecuencia generador de procesos)\n");
        return 1;
    }

    // Inicializar configuración
    Configuracion config;
    config.frecuencia = atoi(argv[1]);
    config.tiempo = (atoi(argv[2])*config.frecuencia);
    config.frecuencia_generador = atoi(argv[3]);
    config.tick = 0;
    pthread_mutex_init(&config.mutex, NULL);
    pthread_cond_init(&config.cond, NULL);

    // Crear hilos
    pthread_t clock_hilo, timer_hilo, process_generator_hilo;

    // Lanzar hilos
    if (pthread_create(&clock_hilo, NULL, clock_thread, (void*)&config) != 0) {
        perror("Error al crear el hilo Clock");
        return 1;
    }
    if (pthread_create(&timer_hilo, NULL, timer_thread, (void*)&config) != 0) {
        perror("Error al crear el hilo Timer");
        return 1;
    }
    /*if (pthread_create(&process_generator_hilo, NULL, process_generator_thread, (void*)&config) != 0) {
        perror("Error al crear el hilo Process Generator");
        return 1;
    }*/

    // Esperar a que los hilos terminen (en este caso, nunca terminan)
    pthread_join(clock_hilo, NULL);
    pthread_join(timer_hilo, NULL);
    pthread_join(process_generator_hilo, NULL);

    // Limpiar recursos
    pthread_mutex_destroy(&config.mutex);
    pthread_cond_destroy(&config.cond);

    return 0;
}