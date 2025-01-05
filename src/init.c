#include "init.h"
#include "process_queue.h"

void init_processmap() {
    int i;

    for (i = 0; i < MAX_PROCESS; i++) process_map[i] = 0; /*Init process_map*/
}

pcb_t init_pcb() {
    pcb_t null_process;
    null_process.pid = -1;
    null_process.status = NULL_S;
    null_process.load_quantum = -1;
    null_process.quantum = null_process.load_quantum;
    null_process.live_time = -1;
    null_process.priority = -1;
    return null_process;
}

void init_machine(args_t* args) {
    int cpu_id, core_id, thread_id;

    args->machine.num_cpus = num_cpus;
    args->machine.cpus = (cpu_t*)malloc(args->machine.num_cpus * sizeof(cpu_t)); /*CPUs*/
    printf("\n");
    for (cpu_id = 0; cpu_id < args->machine.num_cpus; cpu_id++) {
        cpu_t* cpu = &(args->machine.cpus[cpu_id]);
        cpu->id = cpu_id;
        cpu->num_cores = num_cores;
        printf("CPU %d:\n", cpu->id);

        cpu->cores = (core_t*)malloc(cpu->num_cores * sizeof(core_t)); /*Cores per CPU*/

        for (core_id = 0; core_id < cpu->num_cores; core_id++) {
            core_t* core = &(cpu->cores[core_id]);
            core->id = core_id;
            core->num_threads = num_threads;
            inic_queue(&core->queue);
            core->num_proc_queue = 0;
            printf(" -C%d -> ", core->id);

            core->threads = (thread_t*)malloc(core->num_threads * sizeof(thread_t)); /*Threads per core*/

            for (thread_id = 0; thread_id < core->num_threads; thread_id++) {
                thread_t* thread = &(core->threads[thread_id]);
                thread->cpu_id = cpu_id;
                thread->core_id = core_id;
                thread->id = thread_id;
                thread->pcb = init_pcb();
                printf("T%d ", thread->id);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int init_config(args_t* args) {

    printf("Establece la cantidad de CPUs: ");
    scanf("%d", &num_cpus);
    printf("Establece los cores por CPU: ");
    scanf("%d", &num_cores);
    printf("Establece el número de hilos por núcleo: ");
    scanf("%d", &num_threads);
    printf("Establece la frecuencia del scheduler: ");
    scanf("%d", &args->freq_schl);
    printf("Establece la frecuencia del procces_generator: ");
    scanf("%d", &args->freq_pgen);
    printf("Maquina configurada correctamente\n");   
    printf("Iniciando simulador kernel\n");
    init_machine(args);
    init_processmap();
    return (0);
}