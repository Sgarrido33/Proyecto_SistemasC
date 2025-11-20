/* scheduler.c - Lógica de Orbita Realista */
extern int get_sensor_temp(int minuto); // Ahora recibe el minuto
extern void check_cooling(int temp);
extern void transmit_data(int temp);

void delay() {
    for(volatile int d = 0; d < 2000000; d++);
}

void main() {
    int temp = 0;
    
    // Bucle de 0 a 100 (Orbita LEO)
    for (int minuto = 0; minuto <= 100; minuto++) {
        
        // Muestreo cada 5 minutos
        if (minuto % 5 == 0) {
            
            // 1. Sensor (Le pasamos el minuto para que simule sol/sombra)
            temp = get_sensor_temp(minuto);
            
            // 2. Enfriamiento
            check_cooling(temp);

            // 3. Transmision
            transmit_data(temp);
            
            delay();
        }
    }
    asm("li a7, 93; li a0, 0; ecall");
}
