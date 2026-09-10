#ifndef CITY_RENDERER_H
#define CITY_RENDERER_H

// ─────────────────────────────────────────────────────────────
// AeroRoute — Task 1 (CGL / CO1, CO2)
// OpenGL primitives + basic geometric transformations
// Demonstrates:
//   GL_QUADS   → buildings & ground
//   GL_LINES   → flight path
//   GL_POINTS  → city nodes
//   glTranslatef / glScalef → positioning objects
// Team Member: Vedant Bankar (202501070088)
// ─────────────────────────────────────────────────────────────

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// Simple 2D point for city layout
struct Point2D {
    float x, y;
};

// A building definition
struct Building {
    float x, y;          // base center
    float width, height; // dimensions
    float r, g, b;       // colour
};

// A path node for GL_LINES
struct PathNode {
    float x, y;
};

// ─────────────────────────────────────────────────────────────
// CityRenderer — draws the 2D city scene
// ─────────────────────────────────────────────────────────────
class CityRenderer {
private:
    // City data
    Building  buildings[20];
    int       buildingCount;

    PathNode  pathNodes[50];
    int       pathNodeCount;

    // Drone position (for the marker)
    float     droneX, droneY;

    // Window dimensions
    int       winW, winH;

    // Internal draw helpers
    void drawBuilding(const Building& b)    const;
    void drawGround()                       const;
    void drawPath()                         const;
    void drawDrone()                        const;
    void drawNodes()                        const;
    void drawHUD()                          const;

public:
    CityRenderer(int windowWidth, int windowHeight);

    // Call once to add buildings
    void addBuilding(float x, float y,
                     float w, float h,
                     float r, float g, float b);

    // Call once to set flight path
    void setPath(const PathNode* nodes, int count);

    // Update drone marker position
    void setDronePosition(float x, float y);

    // Load the sample AeroRoute city
    void loadSampleScene();

    // Master render call — called from display callback
    void render() const;
};

// ── GLUT callback functions (defined in CityRenderer.cpp) ────
void display_callback();
void reshape_callback(int w, int h);
void keyboard_callback(unsigned char key, int x, int y);

// Global renderer pointer (used by GLUT callbacks)
extern CityRenderer* g_renderer;