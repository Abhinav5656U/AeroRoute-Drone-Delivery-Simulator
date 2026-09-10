#include "oop/Drone.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n=== OOP Task 1 — Class Hierarchy Test ===\n\n";

    // Default constructor
    Drone d1;
    d1.displayStatus();

    // Parameterised constructor
    Drone d2(1, "AeroBot-Alpha", 80.0f, 100.0f, 5.0f);
    d2.displayStatus();

    // Copy constructor
    Drone d3(d2);
    assert(d3.getId() == d2.getId());
    std::cout << "Copy constructor: OK\n";

    // Package and load
    Package pkg(101, "Customer_A", 2.5f, 2);
    pkg.display();
    assert(d2.loadPackage(&pkg) == true);

    // Move drone (battery drain)
    d2.move(3.0f, 4.0f);   // 5 km → 10% drain
    assert(d2.getBattery() < 100.0f);

    // Deliver
    assert(d2.deliverPackage() == true);
    assert(pkg.isDelivered()   == true);

    // Charge
    d2.chargeBattery(20.0f);

    // Polymorphism — Vehicle pointer to Drone
    Vehicle* v = new Drone(2, "AeroBot-Beta", 70.0f, 80.0f, 3.0f);
    v->displayStatus();   // calls Drone::displayStatus()
    delete v;

    // SpecialDrone — multi-level inheritance
    SpecialDrone sd(3, "AeroBot-Special", 90.0f, 100.0f, 8.0f, true, true);
    Vehicle* vSpecial = &sd;
    vSpecial->displayStatus();   // calls SpecialDrone::displayStatus()

    std::cout << "\n✅ All OOP Task 1 tests passed.\n\n";
    return 0;
}