; all regs start at 0

ADD R1, R0, R0     ; R1 = 0
NOT R2, R1         ; R2 = 0xFFFF

SUB R3, R1, R2     ; R3 = 0 - 0xFFFF = 0x0001 (wraparound)
SUB R4, R3, R3     ; R4 = 1 - 1 = 0

HALT
