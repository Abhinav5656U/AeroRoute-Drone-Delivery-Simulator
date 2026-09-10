#include "core/CityGrid.h"
#include "oop/Drone.h"
#include "coa/DroneCPU.h"
#include "graphics/CityRenderer.h"
#include <iostream>

// ─────────────────────────────────────────────────────────────
// AeroRoute — Task 1 Integration Demo
// Runs all 4 subject Task 1 modules together
// ─────────────────────────────────────────────────────────────

int main(int argc, char** argv) {

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║   AeroRoute — Task 1 Demo                ║\n";
    std::cout << "║   Semester III | MIT AOE Alandi Pune     ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n\n";

    // ── PL Task 1: Arrays ────────────────────────────────────
    std::cout << "▶ [PL] Loading city map into array...\n";
    CityGrid grid;
    grid.loadSampleCity();
    grid.displayAll();
    grid.searchByName("Customer_A");

    // ── OOP Task 1: Class hierarchy ──────────────────────────
    std::cout << "\n▶ [OOP] Creating drone and package...\n";
    Drone drone(1, "AeroBot-Alpha", 80.0f, 100.0f, 5.0f);
    Package pkg(101, "Customer_A", 2.5f, 2);
    drone.loadPackage(&pkg);
    drone.displayStatus();

    // Polymorphism demo
    Vehicle* v = &drone;
    std::cout << "\n▶ [OOP] Polymorphic call via Vehicle*:\n";
    v->displayStatus();

    // ── COA Task 1: CPU Simulator ────────────────────────────
    std::cout << "\n▶ [COA] Running DroneCPU delivery program...\n";
    DroneCPU cpu;
    cpu.loadDeliveryProgram(0.0f, 0.0f, 5.0f, 3.0f, 100, 1);
    cpu.run();

    // ── CGL Task 1: OpenGL rendering ─────────────────────────
    std::cout << "\n▶ [CGL] Opening OpenGL window...\n";
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AeroRoute — Task 1 City View");

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    CityRenderer renderer(900, 700);
    renderer.loadSampleScene();
    g_renderer = &renderer;

    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);

    std::cout << "   Window open. Press ESC to close.\n\n";
    glutMainLoop();

    return 0;
}