# Tiny ISA
A personalized 16-bit ISA with a custom assembler and CPU emulator, built in C.

## A few learning goals for this project:
1) learn how assembly code turns to machine code and is eventually executed
2) get more comfortable with C fundamentals (I/O, dynamic memory, pointers, and C conventions)
3) see how to control registers, memory, pc in an clean manner

## ISA Breakdown:
- 15–12 opcode | 11–9 DR | 8–6 SR1 | 5–3 000 | 2–0 SR2
## Supported Operations:
- ADD
- SUB (nice break from manual negation & addition)
- AND
- OR
- NOT
- HALT

## Project Structure:
- assembler/   – parses assembly and emits 16-bit hex machine code
- emulator/    – software CPU implementing fetch/decode/execute
- util/        – shared helpers (tokenization, parsing, etc.)
- main/        – driver program

## Flowchart
1. Assembly source (.asm) is parsed into an intermediate instruction representation
2. Instructions are encoded into 16-bit machine words (.hex)
3. The emulator loads the hex into memory
4. A software CPU executes instructions via a fetch/decode/execute loop

### Build
gcc main/main.c assembler/assembler.c emulator/emulator.c util/util.c \
    -Iassembler -Iemulator -Iutil -o tinyisa

### Run
./tinyisa


### Future Work:
- add immediates or load/store instructions yet
- add branching/control flow




  



