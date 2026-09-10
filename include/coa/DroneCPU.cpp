#include "coa/DroneCPU.h"
#include <cmath>
#include <cstdio>

// ═════════════════════════════════════════════════════════════
// Registers
// ═════════════════════════════════════════════════════════════
Registers::Registers() {
    reset();
}

void Registers::reset() {
    R0 = R1 = R2 = R3 = 0;
    PC = IR = SP = ACC = 0;
}

void Registers::display() const {
    printf("\n  ┌── Registers ───────────────────────────┐\n");
    printf("  │  R0  (PosX * 100) = %8d            │\n", R0);
    printf("  │  R1  (PosY * 100) = %8d            │\n", R1);
    printf("  │  R2  (Battery %%)  = %8d            │\n", R2);
    printf("  │  R3  (DestID)     = %8d            │\n", R3);
    printf("  │  PC  (Prog Cntr)  = %8d            │\n", PC);
    printf("  │  IR  (Last Opcode)= %8X (hex)      │\n", IR);
    printf("  │  SP  (Stack Ptr)  = %8d            │\n", SP);
    printf("  │  ACC (ALU Result) = %8d            │\n", ACC);
    printf("  └────────────────────────────────────────┘\n\n");
}

// ═════════════════════════════════════════════════════════════
// ALU
// ═════════════════════════════════════════════════════════════
int32_t ALU::ADD(int32_t a, int32_t b) {
    int32_t result = a + b;
    zeroFlag     = (result == 0);
    negativeFlag = (result  < 0);
    overflowFlag = ((b > 0 && a > INT32_MAX - b) ||
                    (b < 0 && a < INT32_MIN - b));
    std::cout << "  [ALU] ADD " << a << " + " << b
              << " = " << result << "\n";
    return result;
}

int32_t ALU::SUB(int32_t a, int32_t b) {
    int32_t result = a - b;
    zeroFlag     = (result == 0);
    negativeFlag = (result  < 0);
    overflowFlag = false;
    std::cout << "  [ALU] SUB " << a << " - " << b
              << " = " << result << "\n";
    return result;
}

// MUL via successive addition (as per COA syllabus)
int32_t ALU::MUL(int32_t a, int32_t b) {
    int32_t result  = 0;
    int32_t absB    = (b < 0) ? -b : b;
    for (int32_t i = 0; i < absB; i++) {
        result = ADD(result, a);
    }
    if (b < 0) result = -result;
    std::cout << "  [ALU] MUL " << a << " * " << b
              << " = " << result << " (successive addition)\n";
    return result;
}

int32_t ALU::DIV(int32_t a, int32_t b) {
    if (b == 0) {
        std::cout << "  [ALU] DIV ERROR: divide by zero\n";
        overflowFlag = true;
        return 0;
    }
    int32_t result = a / b;
    zeroFlag     = (result == 0);
    negativeFlag = (result  < 0);
    std::cout << "  [ALU] DIV " << a << " / " << b
              << " = " << result << "\n";
    return result;
}

bool ALU::CMP(int32_t a, int32_t b) {
    int32_t diff = SUB(a, b);
    std::cout << "  [ALU] CMP: " << (diff == 0 ? "EQUAL":"NOT EQUAL") << "\n";
    return (diff == 0);
}

void ALU::displayFlags() const {
    printf("  [ALU Flags]  Zero=%d  Negative=%d  Overflow=%d\n",
           zeroFlag, negativeFlag, overflowFlag);
}

// ═════════════════════════════════════════════════════════════
// DroneCPU
// ═════════════════════════════════════════════════════════════
DroneCPU::DroneCPU()
    : iMemCount(0), halted(false), cycleCount(0) {}

void DroneCPU::loadInstruction(const Instruction& instr) {
    if (iMemCount >= IMEM_SIZE) {
        std::cout << "[CPU] Instruction memory full!\n";
        return;
    }
    iMem[iMemCount++] = instr;
}

// ── Fetch → Decode → Execute (one instruction) ───────────────
void DroneCPU::fetchDecodeExecute(const Instruction& instr) {
    printf("\n  ── Cycle %d ─────────────────────────────────\n",
           cycleCount + 1);

    // FETCH
    reg.IR = static_cast<int32_t>(instr.opcode);
    printf("  [FETCH]   Opcode=0x%02X  Label='%s'\n",
           reg.IR, instr.label.c_str());

    // DECODE
    printf("  [DECODE]  Op1=%d  Op2=%d\n",
           instr.operand1, instr.operand2);

    // EXECUTE
    printf("  [EXECUTE] ");
    switch (instr.opcode) {

        case OP_LOAD:
            // operand1 = register index (0-3), operand2 = value
            switch (instr.operand1) {
                case 0: reg.R0 = instr.operand2; break;
                case 1: reg.R1 = instr.operand2; break;
                case 2: reg.R2 = instr.operand2; break;
                case 3: reg.R3 = instr.operand2; break;
            }
            printf("LOAD R%d = %d\n", instr.operand1, instr.operand2);
            break;

        case OP_MOVE:
            // operand1=deltaX*100, operand2=deltaY*100
            reg.ACC = alu.ADD(reg.R0, instr.operand1);
            reg.R0  = reg.ACC;
            reg.ACC = alu.ADD(reg.R1, instr.operand2);
            reg.R1  = reg.ACC;
            printf("MOVE drone to (%d, %d)\n", reg.R0, reg.R1);
            break;

        case OP_CALC: {
            // Calculate Manhattan distance (operand1=dX, operand2=dY)
            int32_t dx  = (instr.operand1 < 0) ?
                           -instr.operand1 : instr.operand1;
            int32_t dy  = (instr.operand2 < 0) ?
                           -instr.operand2 : instr.operand2;
            reg.ACC     = alu.ADD(dx, dy);
            printf("CALC distance = %d (units * 100)\n", reg.ACC);
            break;
        }

        case OP_CHARGE:
            // operand1 = battery units to add
            reg.ACC = alu.ADD(reg.R2, instr.operand1);
            reg.R2  = (reg.ACC > 100) ? 100 : reg.ACC;
            printf("CHARGE battery → %d%%\n", reg.R2);
            break;

        case OP_CMP:
            // Compare R3 (current dest) with operand1 (target)
            alu.CMP(reg.R3, instr.operand1);
            printf("CMP R3 vs %d\n", instr.operand1);
            break;

        case OP_DELIVER:
            printf("DELIVER package at (%d, %d)\n", reg.R0, reg.R1);
            break;

        case OP_JMP:
            reg.PC = instr.operand1;
            printf("JMP to instruction %d\n", instr.operand1);
            break;

        case OP_HALT:
            halted = true;
            printf("HALT — simulation complete\n");
            break;

        default:
            printf("UNKNOWN OPCODE 0x%02X\n", instr.opcode);
            break;
    }

    if (instr.opcode != OP_JMP)
        reg.PC++;
    cycleCount++;
}

// ── step — run one instruction at current PC ─────────────────
void DroneCPU::step() {
    if (halted || reg.PC >= iMemCount) {
        std::cout << "[CPU] Halted or no more instructions.\n";
        return;
    }
    fetchDecodeExecute(iMem[reg.PC]);
}

// ── run — run all instructions until HALT ────────────────────
void DroneCPU::run() {
    std::cout << "\n[DroneCPU] Starting execution...\n";
    while (!halted && reg.PC < iMemCount) {
        step();
    }
    std::cout << "\n[DroneCPU] Execution complete. "
              << "Cycles: " << cycleCount << "\n";
    displayState();
}

// ── loadDeliveryProgram — generate instructions for a delivery
void DroneCPU::loadDeliveryProgram(float startX, float startY,
                                   float destX,  float destY,
                                   int   battery, int destId) {
    // Scale floats to int (×100) so ALU works with integers
    int32_t sx = static_cast<int32_t>(startX * 100);
    int32_t sy = static_cast<int32_t>(startY * 100);
    int32_t dx = static_cast<int32_t>((destX - startX) * 100);
    int32_t dy = static_cast<int32_t>((destY - startY) * 100);

    loadInstruction({OP_LOAD,   0, sx,     "LOAD R0=StartX"});
    loadInstruction({OP_LOAD,   1, sy,     "LOAD R1=StartY"});
    loadInstruction({OP_LOAD,   2, battery,"LOAD R2=Battery"});
    loadInstruction({OP_LOAD,   3, destId, "LOAD R3=DestID"});
    loadInstruction({OP_CALC,   dx, dy,    "CALC distance"});
    loadInstruction({OP_MOVE,   dx, dy,    "MOVE to dest"});
    loadInstruction({OP_CMP,    destId, 0, "CMP arrived?"});
    loadInstruction({OP_DELIVER,0, 0,      "DELIVER package"});
    loadInstruction({OP_HALT,   0, 0,      "HALT"});
}

// ── displayState ──────────────────────────────────────────────
void DroneCPU::displayState() const {
    std::cout << "\n════════ DroneCPU State ════════\n";
    reg.display();
    alu.displayFlags();
    printf("  Halted: %s  |  Cycles: %d\n",
           halted ? "YES" : "NO", cycleCount);
    std::cout << "════════════════════════════════\n\n";
}