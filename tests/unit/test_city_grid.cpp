#include "core/CityGrid.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n=== PL Task 1 — Array Operations Test ===\n\n";

    CityGrid grid;
    grid.loadSampleCity();

    // DISPLAY
    grid.displayAll();

    // SEARCH
    std::cout << "--- Search 'Customer_A' ---\n";
    int id = grid.searchByName("Customer_A");
    assert(id != -1);

    // UPDATE
    std::cout << "\n--- Update Customer_A coordinates ---\n";
    grid.updateNode(id, 6.0f, 4.0f);
    grid.displayNode(id);

    // INSERT
    std::cout << "\n--- Insert new node ---\n";
    grid.insertNode("Customer_D", 9.0f, 1.0f, CUSTOMER);
    grid.displayAll();

    // DELETE
    std::cout << "--- Delete Junction_3 ---\n";
    int jid = grid.searchByName("Junction_3");
    grid.deleteNode(jid);
    grid.displayAll();

    // BOUNDARY: search deleted node
    std::cout << "--- Search deleted node ---\n";
    int result = grid.searchByName("Junction_3");
    assert(result == -1);

    std::cout << "\n✅ All PL Task 1 tests passed.\n\n";
    return 0;
}