MAIN: sub r1, r4
LIST:   .data 6, -9

MAIN:  add *r3 ,  r3
   jsr fn1
LOOP: prn #48
macr m_macr
        cmp r3, #-6
        bne END
endmacr
        lea STR, r6
        m_macr
        inc r6
        mov *r6, L3
        sub r1, r4
        cmp r3, #-6
        bne END
        add r7, *r6
        clr K
        sub L3, L3
.entry MAIN
        jmp LOOP
END:     stop
STR:    .string "abcd"
LIST:   .data 6, -9
.data   -100
K:      .data 31
.extern L3