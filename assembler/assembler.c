#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "assembler.h"
#include "util.h"

/**
 * Parses an assembly mnemonic and maps it to an opcode.
 * 
 * @param s null terminated string with the assembly instruction
 * @param out_op output parameter; set to the corresponding Opcode of this instr if successful
 * 
 * @return 1 if the mnemonic is recognized and out_op is successfully set
 *         0 if the mnemonic us unknown or invalid
 */
static int parse_mnemonic(const char* s, Opcode* out_op);

/**
 * parses a single line of assembly source code into an instr struct
 * 
 * - strips comments and whitespace
 * - tokenizes the line
 * - parses the mnemonic and operands
 * - validates register indices
 * - populates the instr struct
 * 
 * @param line raw line read from .asm file. immutable
 * @param lineno line number in the .asm file (for error reporting)
 * @param out_instr output parameter; filled with the parsed instr struct on success
 * 
 * @return 1 if valid instr was parsed
 *         0 if the line is empty / comment (skip)
 *         -1 on parse validation error
 */
//use const to ensure we only read, not modify
static int parse_line_to_instr(const char* line, int lineno, Instr* out_instr);

/**
 * encodes a parsed instruction into a 16-bit machine word
 * 
 *   bits 15–12 : opcode
 *   bits 11–9  : DR
 *   bits 8–6   : SR1
 *   bits 5–3   : unused (000)
 *   bits 2–0   : SR2 
 * 
 * @param in   Pointer to a fully populated Instr struct
 * 
 * @return the encoded 16-bit instruction
 */
static uint16_t encode_instr(const Instr* in);

/**
 * Writes a 16-bit instruction in hex format to the output stream (emulator will read from this stream to F,D,E)
 * 
 * @param out output file stream
 * @param word 16-bit instruction word to write
 */
static void write_hex_to_stream(FILE* out, uint16_t word);


static int parse_mnemonic(const char* s, Opcode* out_op) {
    if (!s || !out_op) {
        return 0;
    }
    if (strcmp(s, "ADD") == 0) {
        *out_op = OP_ADD;
        return 1;
    }
    if (strcmp(s, "SUB") == 0) {
        *out_op = OP_SUB;
        return 1;
    }
    if (strcmp(s, "AND") == 0) {
        *out_op = OP_AND;
        return 1;
    }
    if (strcmp(s, "OR") == 0) {
        *out_op = OP_OR;
        return 1;
    }
    if (strcmp(s, "NOT") == 0) {
        *out_op = OP_NOT;
        return 1;
    }
    if (strcmp(s, "HALT") == 0) {
        *out_op = OP_HALT;
        return 1;
    }
    return 0;
}

static int parse_line_to_instr(const char* line, int lineno, Instr* out) {
    if (line == NULL || lineno < 0) {
        return -1;
    }

    char buf[256];
    strncpy(buf, line, sizeof(buf));
    buf[sizeof(buf)-1] = '\0';

    strip_comments(buf);
    strip_whitespace(buf);

    if (buf[0] == '\0') {
        return 0;
    }

    char toks[8][64];
    int numtokens = tokenize(buf, toks, 8);

    if (numtokens == 0) {
        return 0;
    }

    Opcode op;
    if (!parse_mnemonic(toks[0], &op)) {
        fprintf(stderr, "Line %d: unknown mnemonic '%s'\n", lineno, toks[0]);
        return -1;
    }
    out->op = op;

    if (numtokens == 1) {
        if (op == OP_HALT) {
            out->dr = out->sr1 = out->sr2 = 0;
            return 1;
        }
        return -1;
    }

    if (numtokens == 3) {
        if (op != OP_NOT) {
            return -1;
        }
        int dr = parse_reg(toks[1]);
        int sr1 = parse_reg(toks[2]); //use sr1 for the sr in NOT

        if (sr1 == -1 || dr == -1) {
            return -1;
        }

        out->dr = dr;
        out->sr1 = sr1;
        out->sr2 = 0;

        return 1;
    }

    if (numtokens != 4) {
        return -1;
    }

    int dr = parse_reg(toks[1]);
    int sr1 = parse_reg(toks[2]);
    int sr2 = parse_reg(toks[3]);
    if (dr == -1 || sr1 == -1 || sr2 == -1) {
        return -1;
    }
    out->dr = dr;
    out->sr1 = sr1;
    out->sr2 = sr2;

    return 1;
}

static uint16_t encode_instr(const Instr* in) {

    return (((in->op & 0xF) << 12) |
            ((in->dr & 0x7) << 9) |
            ((in->sr1 & 0x7) << 6) |
            ((0x0 << 3)) |
            ((in->sr2 & 0x7)));
}

static void write_hex_to_stream(FILE* out, uint16_t binary_word) {
    fprintf(out, "%04X\n", binary_word);
}

int assemble_file(const char* asm_path, const char* out_path) {
    if (!asm_path) {
        return 1;
    }

    FILE* in = fopen(asm_path, "r");

    if (!in) {
        perror("fopen input");
        return 1;
    }

    FILE* out = stdout;
    if (out_path) {
        out = fopen(out_path, "w");
        if (!out) {
            perror("fopen output");
            fclose(in);
            return 1;
        }
    }

    char line[256];
    int lineno = 0;

    while (fgets(line, sizeof(line), in)) {
        lineno++;

        Instr instr;
        int status = parse_line_to_instr(line, lineno, &instr);

        if (status == 0) {
            continue;
        }
        if (status < 0) {
            if (out_path) {
                fclose(out);
            }
            fclose(in);
            return 1;
        }

        uint16_t binary_instr = encode_instr(&instr);
        write_hex_to_stream(out, binary_instr);
    }
    fclose(in);
    if (out_path) {
        fclose(out);
    }
    return 0;
}

