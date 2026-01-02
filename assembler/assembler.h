#pragma once //only one copy of any header. incase its included multiple times bc of overlap

typedef enum { 
    OP_ADD = 5,
    OP_SUB = 1,
    OP_HALT = 15,
    OP_OR = 2,
    OP_NOT = 3,
    OP_AND = 4
} Opcode;

typedef struct {
    Opcode op;
    int dr;
    int sr1;
    int sr2;
} Instr;



/**
 * public entry point to the assembler
 * 
 * @param asm_path path of the .asm file
 * @param out_path path we want to output to (NULL if we want terminal output)
 * 
 * @return 0 on success, 1 otherwise
 * 
 */
int assemble_file(const char* asm_path, const char* out_path);




