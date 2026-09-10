#include "oop/Drone.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

// ═════════════════════════════════════════════════════════════
// Vehicle
// ═════════════════════════════════════════════════════════════
Vehicle::Vehicle(int id, const std::string& name, float speed)
    : id(id), name(name), speed(speed), active(false) {}

// ═════════════════════════════════════════════════════════════
// Package
// ═════════════════════════════════════════════════════════════
Package::Package(int pid, const std::string& dest,
                 float weight, int priority)
    : packageId(pid), destination(dest),
      weightKg(weight), priority(priority), delivered(false) {}

void Package::display() const {
    const char* pStr[] = {"", "NORMAL", "HIGH", "EMERGENCY"};
    std::cout << "  Package #" << packageId
              << " → " << destination
              << "  Weight: " << weightKg << " kg"
              << "  Priority: " << pStr[priority]
              << "  Delivered: " << (delivered ? "YES" : "NO")
              << "\n";
}

// ═════════════════════════════════════════════════════════════
// Drone — Default constructor
// ═════════════════════════════════════════════════════════════
Drone::Drone()
    : Vehicle(0, "UnnamedDrone", 60.0f),
      batteryPct(100.0f), maxPayloadKg(5.0f),
      posX(0.0f), posY(0.0f), cargo(nullptr) {
    activate();
}

// ── Parameterised constructor ─────────────────────────────────
Drone::Drone(int id, const std::string& name,
             float speed, float battery, float maxPayload)
    : Vehicle(id, name, speed),
      batteryPct(battery), maxPayloadKg(maxPayload),
      posX(0.0f), posY(0.0f), cargo(nullptr) {
    activate();
}

// ── Copy constructor ─────────────────────────────────────────
Drone::Drone(const Drone& other)
    : Vehicle(other.id, other.name, other.speed),
      batteryPct(other.batteryPct),
      maxPayloadKg(other.maxPayloadKg),
      posX(other.posX), posY(other.posY),
      cargo(nullptr) {          // don't copy the cargo pointer
    active = other.active;
}

// ── Destructor ───────────────────────────────────────────────
Drone::~Drone() {
    // Package is owned by DeliveryManager, not Drone
    cargo = nullptr;
}

// ── displayStatus — overrides pure virtual ───────────────────
void Drone::displayStatus() const {
    std::cout << "┌─ Drone Status ──────────────────────────┐\n";
    std::cout << "│ ID      : " << id              << "\n";
    std::cout << "│ Name    : " << name            << "\n";
    std::cout << "│ Active  : " << (active ? "YES" : "NO") << "\n";
    std::cout << "│ Speed   : " << speed  << " km/h\n";
    std::cout << "│ Battery : " << batteryPct << " %\n";
    std::cout << "│ Payload : " << maxPayloadKg << " kg max\n";
    std::cout << "│ Pos     : (" << posX << ", " << posY << ")\n";
    std::cout << "│ Cargo   : "
              << (cargo ? cargo->getDestination() : "None") << "\n";
    std::cout << "└─────────────────────────────────────────┘\n";
}

// ── loadPackage ───────────────────────────────────────────────
bool Drone::loadPackage(Package* pkg) {
    if (!pkg) return false;
    if (cargo) {
        std::cout << "[Drone] Already carrying a package!\n";
        return false;
    }
    if (!canCarry(pkg->getWeight())) {
        std::cout << "[Drone] Package too heavy! Max: "
                  << maxPayloadKg << " kg\n";
        return false;
    }
    cargo = pkg;
    std::cout << "[Drone] Loaded package #" << pkg->getId()
              << " for " << pkg->getDestination() << "\n";
    return true;
}

// ── deliverPackage ────────────────────────────────────────────
bool Drone::deliverPackage() {
    if (!cargo) {
        std::cout << "[Drone] No package to deliver!\n";
        return false;
    }
    cargo->markDelivered();
    std::cout << "[Drone] Delivered package #" << cargo->getId()
              << " to " << cargo->getDestination() << "\n";
    cargo = nullptr;
    return true;
}

// ── move ──────────────────────────────────────────────────────
void Drone::move(float dx, float dy) {
    if (batteryPct <= 0) {
        std::cout << "[Drone] Battery dead! Cannot move.\n";
        return;
    }
    posX += dx;
    posY += dy;
    // Simple battery consumption model
    float dist   = std::sqrt(dx*dx + dy*dy);
    batteryPct  -= dist * 2.0f;   // 2% per km
    if (batteryPct < 0) batteryPct = 0;
    std::cout << "[Drone] Moved to (" << posX << ", " << posY
              << ")  Battery: " << batteryPct << "%\n";
}

// ── chargeBattery ─────────────────────────────────────────────
void Drone::chargeBattery(float amount) {
    batteryPct += amount;
    if (batteryPct > 100.0f) batteryPct = 100.0f;
    std::cout << "[Drone] Charged to " << batteryPct << "%\n";
}

// ── canCarry ─────────────────────────────────────────────────
bool Drone::canCarry(float weightKg) const {
    return weightKg <= maxPayloadKg;
}

// ═════════════════════════════════════════════════════════════
// SpecialDrone
// ═════════════════════════════════════════════════════════════
SpecialDrone::SpecialDrone(int id, const std::string& name,
                           float speed, float battery,
                           float maxPayload,
                           bool nightVision, bool thermal)
    : Drone(id, name, speed, battery, maxPayload),
      hasNightVision(nightVision), hasThermal(thermal) {}

void SpecialDrone::displayStatus() const {
    Drone::displayStatus();   // call parent version first
    std::cout << "│ NightVision: " << (hasNightVision ? "YES":"NO") << "\n";
    std::cout << "│ Thermal Cam: " << (hasThermal     ? "YES":"NO") << "\n";
    std::cout << "└─────────────────────────────────────────┘\n";
}