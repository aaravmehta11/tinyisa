#include "emulator.h"
#include <stdio.h>

int main() {
    CPU cpu;
    init_cpu(&cpu);

    if (load_hex("assembler/test.hex", &cpu) != 0) {
        return 1;
    }

    cpu_run(&cpu);


    for (int i = 0; i < 8; i++) {
        printf("R%d: 0x%04X\n", i, cpu.regs[i]);
    }

}