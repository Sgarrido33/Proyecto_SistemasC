/* proceso2.s - Versión Blindada (Strings en Stack) */
.global check_cooling

.data
state:   .word 0   # 0=OFF, 1=ON
# Ya no usamos .string porque fallaba

.text
check_cooling:
    # Prologo
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)     # Guardamos s0
    
    mv s0, a0        # s0 = temperatura
    
    # Cargar estado
    la t0, state
    lw t1, 0(t0)
    
    # Logica de control
    bnez t1, check_off_logic  # Si esta ON (1), ver si apagamos
    
    # Logica ON: Si Temp > 90
    li t2, 90
    bgt s0, t2, action_turn_on
    j end

check_off_logic:
    # Logica OFF: Si Temp < 60
    li t2, 60
    blt s0, t2, action_turn_off
    j end

action_turn_on:
    li t1, 1
    la t0, state
    sw t1, 0(t0)
    
    # --- IMPRIMIR "[ON] " DESDE EL STACK ---
    # Construimos la cadena manualmente: ' ' ']' 'N' 'O' '['
    # ASCII: [=91, O=79, N=78, ]=93, space=32
    
    li t3, 32        # Espacio final
    sb t3, 4(sp)
    li t3, 93        # ']'
    sb t3, 3(sp)
    li t3, 78        # 'N'
    sb t3, 2(sp)
    li t3, 79        # 'O'
    sb t3, 1(sp)
    li t3, 91        # '['
    sb t3, 0(sp)
    
    li a7, 64        # Syscall write
    li a0, 1         # STDOUT
    mv a1, sp        # Dirección del stack
    li a2, 5         # Longitud 5 chars
    ecall
    j end

action_turn_off:
    li t1, 0
    la t0, state
    sw t1, 0(t0)
    
    # --- IMPRIMIR "[OF] " DESDE EL STACK ---
    # Abreviamos a [OF] para mantenerlo simple
    
    li t3, 32        # Espacio
    sb t3, 4(sp)
    li t3, 93        # ']'
    sb t3, 3(sp)
    li t3, 70        # 'F'
    sb t3, 2(sp)
    li t3, 79        # 'O'
    sb t3, 1(sp)
    li t3, 91        # '['
    sb t3, 0(sp)
    
    li a7, 64
    li a0, 1
    mv a1, sp
    li a2, 5
    ecall

end:
    lw s0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret
