#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define cont_timer 1

// Estructura de datos compartida
typedef struct {
    int frecuencia;
    int tiempo;
    int frecuencia_generador;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_cond_t cond2;
    int tick;
} Configuracion;

// Función del hilo Clock
void* clock_thread(void* arg) {
    Configuracion* config = (Configuracion*)arg;
    while (1) {
        pthread_mutex_lock(&config->mutex);
        while (config->tick < cont_timer ) {
            pthread_cond_wait(&config->cond, &config->mutex);
        }
        if(config->tick == config->tiempo){
            printf("Han pasado: %d segundos\n", config->tiempo / config->frecuencia);
            config->tick = 0;
        }
                

        pthread_cond_broadcast(&config->cond2);
        pthread_mutex_unlock(&config->mutex);
    }
    pthread_exit(NULL);
}

// Función del hilo Timer
void* timer_thread(void* arg) {
    Configuracion* config = (Configuracion*)arg;

    pthread_mutex_lock(&config->mutex);
    while (1) {
        config->tick++;

        pthread_cond_signal(&config->cond);
        pthread_cond_wait(&config->cond2, &config->mutex);

    }
    pthread_exit(NULL);
}

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
    pthread_cond_init(&config.cond2, NULL);

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
    //pthread_join(process_generator_hilo, NULL);

    // Limpiar recursos
    pthread_mutex_destroy(&config.mutex);
    pthread_cond_destroy(&config.cond);

    return 0;
}