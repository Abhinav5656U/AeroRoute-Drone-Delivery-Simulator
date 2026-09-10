#ifndef DRONE_CPU_H
#define DRONE_CPU_H

#include <string>
#include <iostream>
#include <cstdint>

// ─────────────────────────────────────────────────────────────
// AeroRoute — Task 1 (COA / CO1)
// Simplified Drone CPU Simulator
// Maps drone operations → processor components
//
// Drone Operation    →  CPU Equivalent
// ─────────────────────────────────────
// Drone position X   →  Register R0
// Drone position Y   →  Register R1
// Battery level      →  Register R2
// Destination ID     →  Register R3
// Current instruction→  IR (Instruction Register)
// Next instruction   →  PC (Program Counter)
// Distance calc      →  ALU (ADD / SUB / MUL)
//
// Team Member: Ishan Kanchan (202501100088)
// ─────────────────────────────────────────────────────────────

// ── Registers ────────────────────────────────────────────────
struct Registers {
    int32_t R0;   // Drone X position (scaled: actual * 100)
    int32_t R1;   // Drone Y position (scaled: actual * 100)
    int32_t R2;   // Battery percentage (0–100)
    int32_t R3;   // Destination node ID
    int32_t PC;   // Program Counter
    int32_t IR;   // Instruction Register (last fetched opcode)
    int32_t SP;   // Stack Pointer (for return trip)
    int32_t ACC;  // Accumulator (ALU result)

    Registers();
    void display() const;
    void reset();
};

// ── ALU — Arithmetic Logic Unit ──────────────────────────────
class ALU {
public:
    // Returns result; sets flags
    int32_t ADD(int32_t a, int32_t b);
    int32_t SUB(int32_t a, int32_t b);
    int32_t MUL(int32_t a, int32_t b);  // via successive addition
    int32_t DIV(int32_t a, int32_t b);
    bool    CMP(int32_t a, int32_t b);  // true if a == b

    // Flags
    bool zeroFlag;
    bool negativeFlag;
    bool overflowFlag;

    ALU() : zeroFlag(false), negativeFlag(false), overflowFlag(false) {}
    void displayFlags() const;
};

// ── Custom Instruction Set (8 opcodes) ───────────────────────
enum Opcode {
    OP_LOAD   = 0x01,   // Load immediate value into register
    OP_MOVE   = 0x02,   // Move drone (update R0, R1)
    OP_CALC   = 0x03,   // Calculate distance (ALU ADD)
    OP_CHARGE = 0x04,   // Update battery register (R2)
    OP_CMP    = 0x05,   // Compare R3 with destination
    OP_JMP    = 0x06,   // Jump to instruction address
    OP_DELIVER= 0x07,   // Mark delivery complete
    OP_HALT   = 0xFF    // Stop execution
};

// An instruction word
struct Instruction {
    Opcode  opcode;
    int32_t operand1;
    int32_t operand2;
    std::string label;   // human-readable for display
};

// ── DroneCPU — top-level CPU class ───────────────────────────
class DroneCPU {
private:
    Registers   reg;
    ALU         alu;

    static const int IMEM_SIZE = 32;
    Instruction iMem[IMEM_SIZE];  // instruction memory (array)
    int         iMemCount;

    bool halted;
    int  cycleCount;

    void fetchDecodeExecute(const Instruction& instr);

public:
    DroneCPU();

    // Load a program into instruction memory
    void loadInstruction(const Instruction& instr);

    // Run one cycle (fetch → decode → execute)
    void step();

    // Run all loaded instructions
    void run();

    // Convenience: load a standard delivery program
    void loadDeliveryProgram(float startX, float startY,
                             float destX,  float destY,
                             int   battery, int destId);

    // Display full CPU state
    void displayState() const;

    bool isHalted() const { return halted; }
    int  getCycles() const { return cycleCount; }
};

#endif // DRONE_CPU_H

