/* proceso1.s - Adquisición de Temperatura */
.global get_sensor_temp

.data
current_temp: .word 45   # Inicia en 45 grados (Minimo segun PDF)
direction:    .word 1    # 1 = Subiendo, -1 = Bajando (Simulacion)

.text
# Retorna: a0 = temperatura actual
get_sensor_temp:
    la t0, current_temp
    la t1, direction
    
    lw t2, 0(t0)        # Cargar temperatura
    lw t3, 0(t1)        # Cargar dirección
    
    # Simulación lineal: temp = temp + (5 * direction)
    slli t4, t3, 2      # Multiplicar direccion por 4 (aprox 5 grados variacion)
    add t4, t4, t3      # t4 = t3 * 5
    add t2, t2, t4      # Nueva temp

    # --- Lógica de Límites (45 a 105) ---
    li t5, 105
    bge t2, t5, hit_max # Si temp >= 105, empezar a bajar
    
    li t5, 45
    ble t2, t5, hit_min # Si temp <= 45, empezar a subir
    
    j save_exit

hit_max:
    li t2, 105          # Clampear a 105
    li t3, -1           # Cambiar direccion a bajar
    sw t3, 0(t1)
    j save_exit

hit_min:
    li t2, 45           # Clampear a 45
    li t3, 1            # Cambiar direccion a subir
    sw t3, 0(t1)

save_exit:
    # --- Generador de ANOMALÍA (Opcional por ahora) ---
    # Para probar el enfriamiento luego, podrías forzar un 110 aqui a veces
    
    sw t2, 0(t0)        # Guardar nuevo estado
    mv a0, t2           # Retornar valor
    ret
