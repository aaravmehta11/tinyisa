#pragma once
#include <stdint.h>

typedef struct {
    uint16_t mem[65536];
    int regs[8];
    int running;
    int pc;
} CPU;


/**
 * Initializes a CPU
 * 
 * Zeroes memory and registers
 * Sets running = 1 and pc = 0
 * 
 * @param cpu to be initialized
 */
void init_cpu(CPU* cpu);

/**
 * Loads .hex file (one 16-bit hex word per line) to be read from
 * 
 * @param cpu that will run this program
 * @param hex_path file path of the .hex file to be loaded from disk and opened
 * 
 * @returns 0 on success, nonzero on failure
 * 
 */
int load_hex(const char* hex_path, CPU* cpu);

/**
 * performs one cycle of fetch/decode/execute and updates the state of the cpu
 * - reads from mem[pc]
 * - increments pc
 * - updates state of the cpu
 * 
 * @param CPU cpu whose state will be updated
 * 
 * @returns 0 on success, nonzero on failure (illegal opcode or runtime error)
 */
int cpu_step(CPU* cpu);

/**
 * Runs the CPU until HALT is executed or a runtime error occurs
 * 
 * @param CPU cpu to run process on
 */
void cpu_run(CPU* cpu);

