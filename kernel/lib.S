.global delay
.global out_word
.global in_word
.global memset
.global memcpy
.global memcpy8
.global memmove
.global memcmp
.global get_el
.global get_ttbr0_el1
.global get_ttbr1_el1

get_el:
    mrs x0, currentel
    lsr x0, x0, #2
    ret

delay:
    subs x0, x0, #1
    bne delay
    ret

out_word:
    str w1, [x0]
    ret

in_word:
    ldr w0, [x0]
    ret

memset:
    cmp x2, #0
    beq memset_end

set:
    strb w1, [x0], #1
    subs x2, x2, #1
    bne set

memset_end:
    ret

memcmp:
    mov x3, x0
    mov x0, #0

compare:
    cmp x2, #0
    beq memcmp_end

    ldrb w4, [x3], #1
    ldrb w5, [x1], #1
    sub x2, x2, #1
    cmp w4, w5
    beq compare

    mov x0, #1

memcmp_end:
    ret

memmove:
memcpy:
    cmp x2, #0
    beq memcpy_end

    mov x4, #1

    cmp x1, x0
    bhs copy
    add x3, x1, x2
    cmp x3, x0
    bls copy

overlap:
    sub x3, x2, #1
    add x0, x0, x3
    add x1, x1, x3
    neg x4, x4

copy:
    ldrb w3, [x1]
    strb w3, [x0]
    add x0, x0, x4
    add x1, x1, x4

    subs x2, x2, #1
    bne copy

memcpy_end:
    ret

memcpy8:
    cmp x2, #0
    beq memcpy_end8

    mov x4, #8          

    cmp x1, x0
    bhs copy8
    add x3, x1, x2
    cmp x3, x0
    bls copy8

overlap8:
    sub x3, x2, #8      
    add x0, x0, x3
    add x1, x1, x3
    neg x4, x4

copy8:
    ldr x3, [x1]        
    str x3, [x0]        
    add x0, x0, x4      
    add x1, x1, x4      

    subs x2, x2, #8     
    bne copy8

memcpy_end8:
    ret


get_ttbr0_el1:
    mrs x0, ttbr0_el1
    ret

get_ttbr1_el1:
    mrs x0, TTBR1_EL1
    ret
