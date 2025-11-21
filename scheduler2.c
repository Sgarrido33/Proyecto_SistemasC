/* scheduler.c - Versión Corregida y Automatizada */

extern int get_sensor_temp();
extern void print_msg(char* str, int len);
extern void print_int(int num);
extern void check_cooling(int temp, int* state_addr);
extern void transmit_data(int temp);
extern int get_time();
extern int get_current_pc();

// --- NUEVA FUNCIÓN AUXILIAR ---
// Calcula el largo del texto automáticamente para no equivocarnos con los números
int my_strlen(char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Estructura para el Bloque de Control de Proceso (PCB)
typedef struct {
    int id;
    int priority; 
    int status;   
} Process;

void main() {
    int current_temp = 0;
    int cooling_state = 0; 
    int prev_cooling_state = 0; 
    int sim_time = 0; 

    // Métricas
    int total_simulation_cycles = 0;
    int total_syscalls = 0;
    int anomaly_count = 0; 
    
    // Tiempos
    int cycle_snapshot_start = 0;
    int cycle_snapshot_end = 0;

    // Strings guardados en variables para usar strlen fácilmente
    char* msg_inicio = "\n ESCENARIO 2 \n";
    print_msg(msg_inicio, my_strlen(msg_inicio));

    Process p1 = {1, 2, 0}; 
    Process p2 = {2, 0, 0}; 
    Process p3 = {3, 1, 0}; 

    while(1) {
        sim_time += 5;
        cycle_snapshot_start = get_time();

        // 1. ADQUISICIÓN (P1)
        current_temp = get_sensor_temp();
        total_syscalls++; 
        
        if (current_temp == 0) break; 

        // Prints informativos
        char* str_min = "Min: ";
        print_msg(str_min, my_strlen(str_min));
        print_int(sim_time);

        int tiempo_relativo = sim_time % 100;
        if (tiempo_relativo <= 42) {
            char* str_luz = " [Luz] | ";
            print_msg(str_luz, my_strlen(str_luz));
        } else {
            char* str_osc = " [Osc] | ";
            print_msg(str_osc, my_strlen(str_osc));
        }
        total_syscalls += 2;

        // 2. CHEQUEO DE PRIORIDAD Y RIESGO
        if (current_temp > 90 && cooling_state == 0) {
            // Definimos mensajes largos
            char* msg_warn = " [!] WARNING: Riesgo de perdida de data";
            char* msg_action = "\n          -> Transmitiendo primero: ";
            
            print_msg(msg_warn, my_strlen(msg_warn));
            print_msg(msg_action, my_strlen(msg_action));
            
            anomaly_count++; 
        }

        // Ejecutar P3 (Transmisión) PRIMERO
        transmit_data(current_temp);
        total_syscalls++;

        // 3. CONTROL (P2) DESPUÉS
        prev_cooling_state = cooling_state; 
        check_cooling(current_temp, &cooling_state);
        
        if (prev_cooling_state != cooling_state) {
            total_syscalls++;
        }

        // Anomalía Crítica
        if (current_temp > 105) {
            int anomaly_pc = get_current_pc();
            char* msg_anom = " | ANOMALIA PC: ";
            print_msg(msg_anom, my_strlen(msg_anom));
            print_int(anomaly_pc);
            anomaly_count++;
        }

        cycle_snapshot_end = get_time();
        int iter_cycles = cycle_snapshot_end - cycle_snapshot_start;
        total_simulation_cycles += iter_cycles;

        char* msg_exe = " | T_exe: ";
        print_msg(msg_exe, my_strlen(msg_exe));
        print_int(iter_cycles);
        print_msg("\n", 1); 
        total_syscalls += 2;

        for(volatile int d = 0; d < 2000000; d++);
    }
    
    // Cálculo memoria
    int mem_occupation = 0;
    mem_occupation += sizeof(p1) * 3;      
    mem_occupation += sizeof(current_temp);  
    mem_occupation += sizeof(cooling_state); 
    mem_occupation += sizeof(sim_time);      
    mem_occupation += sizeof(total_simulation_cycles); 

    // --- REPORTE FINAL ---
    char* msg_res = "\n--- RESULTADOS ESCENARIO 2 ---\n";
    print_msg(msg_res, my_strlen(msg_res));
    
    char* msg_ciclos = "Ciclos Totales: ";
    print_msg(msg_ciclos, my_strlen(msg_ciclos));
    print_int(total_simulation_cycles);
    print_msg("\n", 1);

    char* msg_sys = "Syscalls: ";
    print_msg(msg_sys, my_strlen(msg_sys));
    print_int(total_syscalls);
    print_msg("\n", 1);

    char* msg_int = "Interrupts (Anomalias): ";
    print_msg(msg_int, my_strlen(msg_int));
    print_int(anomaly_count);
    print_msg("\n", 1);

    char* msg_mem = "Mem. Occupation (Bytes): ";
    print_msg(msg_mem, my_strlen(msg_mem));
    print_int(mem_occupation);
    print_msg("\n", 1);

    char* msg_line = "------------------------------\n";
    print_msg(msg_line, my_strlen(msg_line));
    
    asm("li a7, 93; li a0, 0; ecall");
}