/* proceso3.s - Version Blindada */
.global transmit_data

.text
# Entrada: a0 = temperatura
transmit_data:
    # Prologo
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)

    mv s0, a0           # Guardamos temperatura en s0

    # 1. Imprimir la temperatura (Numero)
    mv a0, s0
    call print_int

    # 2. Imprimir Salto de Linea (Manual en Stack)
    li t0, 10           # Codigo ASCII del salto de linea (\n)
    sb t0, 0(sp)        # Lo guardamos en el tope del stack
    
    li a7, 64           # Syscall write
    li a0, 1            # STDOUT
    mv a1, sp           # Imprimir lo que esta en sp (el \n)
    li a2, 1            # Longitud 1 byte
    ecall

    # Epilogo
    lw s0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret

# --- Rutina de Impresion de Enteros ---
print_int:
    addi sp, sp, -32    
    sw ra, 28(sp)       
    
    addi t1, sp, 26     # Puntero al buffer
    li t2, 10           # Divisor
    mv t3, a0           # Numero

convert_loop:
    rem t4, t3, t2      
    div t3, t3, t2      
    addi t4, t4, 48     # ASCII
    addi t1, t1, -1     
    sb t4, 0(t1)        
    bnez t3, convert_loop 
    
    # Imprimir numero
    li a7, 64
    li a0, 1
    mv a1, t1           
    addi t5, sp, 26
    sub a2, t5, t1
    ecall

    lw ra, 28(sp)
    addi sp, sp, 32
    ret
