#include "coa/DroneCPU.h"
#include <cassert>

int main() {
    std::cout << "\n=== COA Task 1 — DroneCPU Simulation Test ===\n\n";

    DroneCPU cpu;

    // Load a delivery: Warehouse(0,0) → Customer_A(5,3)
    // Battery = 85%, DestID = 1
    cpu.loadDeliveryProgram(0.0f, 0.0f, 5.0f, 3.0f, 85, 1);

    // Show initial state
    cpu.displayState();

    // Run all instructions
    cpu.run();

    // Verify CPU halted
    assert(cpu.isHalted() == true);
    std::cout << "\n✅ COA Task 1 tests passed.\n\n";
    return 0;
}