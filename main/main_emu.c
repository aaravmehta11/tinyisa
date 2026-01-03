#include <stdio.h>
#include "assembler.h"
#include "emulator.h"

int main() {
    const char* asm_path = "assembler/test.asm";
    const char* hex_path = "assembler/test.hex";

    if (assemble_file(asm_path, hex_path) != 0) {
        fprintf(stderr, "assemble failed\n");
        return 1;
    }

    CPU cpu;
    init_cpu(&cpu);

    if (load_hex(hex_path, &cpu) != 0) {
        fprintf(stderr, "load failed\n");
        return 1;
    }

    cpu_run(&cpu);

    for (int i = 0; i < 8; i++) {
        printf("R%d = 0x%04X\n", i, cpu.regs[i]);
    }
    return 0;
}
