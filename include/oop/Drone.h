#ifndef DRONE_H
#define DRONE_H

#include <string>
#include <iostream>

// ─────────────────────────────────────────────────────────────
// AeroRoute — Task 1 (OOP / CO1)
// Demonstrates: Classes, Encapsulation, Constructors,
//               Inheritance, Polymorphism, Virtual functions
// Team Member: Akshada Pote (202501100099)
// ─────────────────────────────────────────────────────────────

// ── Abstract base: any vehicle in the simulation ─────────────
class Vehicle {
protected:
    int         id;
    std::string name;
    float       speed;      // km/h
    bool        active;

public:
    Vehicle(int id, const std::string& name, float speed);
    virtual ~Vehicle() {}

    // Pure virtual — every vehicle must implement its own status
    virtual void displayStatus() const = 0;

    // Common getters (encapsulation)
    int         getId()    const { return id;     }
    std::string getName()  const { return name;   }
    float       getSpeed() const { return speed;  }
    bool        isActive() const { return active; }

    void activate()   { active = true;  }
    void deactivate() { active = false; }
};

// ── Package — what the drone carries ─────────────────────────
class Package {
private:
    int         packageId;
    std::string destination;
    float       weightKg;
    int         priority;     // 1=NORMAL 2=HIGH 3=EMERGENCY
    bool        delivered;

public:
    Package(int pid, const std::string& dest,
            float weight, int priority);

    // Getters
    int         getId()          const { return packageId;   }
    std::string getDestination() const { return destination; }
    float       getWeight()      const { return weightKg;    }
    int         getPriority()    const { return priority;    }
    bool        isDelivered()    const { return delivered;   }

    void markDelivered() { delivered = true; }
    void display()       const;
};

// ── Drone — inherits Vehicle ──────────────────────────────────
class Drone : public Vehicle {
private:
    float    batteryPct;    // 0.0 to 100.0
    float    maxPayloadKg;
    float    posX, posY;   // current position
    Package* cargo;        // currently carried package (nullptr if empty)

public:
    // Default constructor
    Drone();

    // Parameterised constructor
    Drone(int id, const std::string& name,
          float speed, float battery, float maxPayload);

    // Copy constructor
    Drone(const Drone& other);

    // Destructor
    ~Drone();

    // Overrides pure virtual
    void displayStatus() const override;

    // Drone-specific behaviour
    bool loadPackage(Package* pkg);
    bool deliverPackage();
    void move(float dx, float dy);
    void chargeBattery(float amount);
    bool canCarry(float weightKg) const;

    // Getters
    float    getBattery()    const { return batteryPct;   }
    float    getMaxPayload() const { return maxPayloadKg; }
    float    getPosX()       const { return posX;         }
    float    getPosY()       const { return posY;         }
    Package* getCargo()      const { return cargo;        }
};

// ── SpecialDrone — inherits Drone (multi-level inheritance) ──
class SpecialDrone : public Drone {
private:
    bool hasNightVision;
    bool hasThermal;

public:
    SpecialDrone(int id, const std::string& name,
                 float speed, float battery, float maxPayload,
                 bool nightVision, bool thermal);

    // Overrides displayStatus() again — polymorphism
    void displayStatus() const override;

    bool getNightVision() const { return hasNightVision; }
    bool getThermal()     const { return hasThermal;     }
};

#endif // DRONE_H

