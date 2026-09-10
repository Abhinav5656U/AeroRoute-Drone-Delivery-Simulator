# AeroRoute: Autonomous Drone Delivery Simulator

**Institution:** MIT Academy of Engineering, Pune  
**Course:** Integrated Project (Week 1 / Task 1)  

AeroRoute is a C++ based autonomous drone delivery simulator that integrates fundamental computer science concepts across four core subjects. This repository contains the Phase 1 implementation, establishing the foundational data structures, object-oriented architecture, hardware-level instruction simulation, and graphical rendering.

## 🚀 Week 1 Objectives & Module Breakdown

### 1. Programming Laboratory (PL) - Core Data Structures
*   **Component:** `CityGrid`
*   **Implementation:** Developed a fixed-size array-based data structure to manage up to 50 city nodes (warehouses, customers, and charging stations). 
*   **Features:** Core CRUD operations including node insertion, logical deletion, updates, and searching by node name with minimal memory overhead.

### 2. Object-Oriented Programming (OOP) - Class Architecture
*   **Component:** `Drone` & `Vehicle` Hierarchy
*   **Implementation:** Designed a robust class hierarchy demonstrating abstraction and runtime polymorphism.
*   **Features:** An abstract `Vehicle` base class enforces a pure virtual `displayStatus()` method. The `Drone` subclass encapsulates payload and battery data, while a `SpecialDrone` class demonstrates multi-level inheritance.

### 3. Computer Organization & Architecture (COA) - CPU Simulation
*   **Component:** `DroneCPU`
*   **Implementation:** Built a software-simulated CPU mimicking the drone's internal processor to handle navigation instructions.
*   **Features:** Includes an 8-register architecture (R0-R3, PC, IR, ACC, SP), an Arithmetic Logic Unit (ALU) utilizing successive addition for multiplication, and a custom 8-instruction set processed through a visible Fetch-Decode-Execute pipeline.

### 4. Computer Graphics Lab (CGL) - Visual Rendering
*   **Component:** `CityRenderer`
*   **Implementation:** Developed a 2D/3D visualization module using OpenGL and FreeGLUT.
*   **Features:** Utilizes standard primitives (`GL_QUADS`, `GL_LINES`, `GL_POINTS`) and geometric matrix transformations (`glTranslatef`, `glScalef`, `glPushMatrix`, `glPopMatrix`) to render isolated coordinate systems for the city map and drone flight paths.

## 📂 Project Structure

```text
AeroRoute/
├── include/                 # Header files (.h)
│   ├── coa/                 # DroneCPU.h
│   ├── core/                # CityGrid.h
│   ├── graphics/            # CityRenderer.h
│   └── oop/                 # Drone.h
├── src/                     # Source files (.cpp)
│   ├── coa/                 # DroneCPU.cpp
│   ├── core/                # CityGrid.cpp, MapLoader.cpp
│   ├── graphics/            # CityRenderer.cpp
│   ├── integration/         # main.cpp (Entry point)
│   └── oop/                 # Drone.cpp
└── tests/
    └── unit/                # Individual module test files
