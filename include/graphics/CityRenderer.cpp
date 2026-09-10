#include "graphics/CityRenderer.h"
#include <cstring>
#include <cstdio>

// Global renderer for GLUT callbacks
CityRenderer* g_renderer = nullptr;

// ═════════════════════════════════════════════════════════════
// Constructor
// ═════════════════════════════════════════════════════════════
CityRenderer::CityRenderer(int windowWidth, int windowHeight)
    : buildingCount(0), pathNodeCount(0),
      droneX(0.0f), droneY(0.0f),
      winW(windowWidth), winH(windowHeight) {}

// ═════════════════════════════════════════════════════════════
// addBuilding
// ═════════════════════════════════════════════════════════════
void CityRenderer::addBuilding(float x, float y,
                                float w, float h,
                                float r, float g, float b) {
    if (buildingCount >= 20) return;
    buildings[buildingCount++] = {x, y, w, h, r, g, b};
}

// ═════════════════════════════════════════════════════════════
// setPath
// ═════════════════════════════════════════════════════════════
void CityRenderer::setPath(const PathNode* nodes, int count) {
    pathNodeCount = (count > 50) ? 50 : count;
    for (int i = 0; i < pathNodeCount; i++)
        pathNodes[i] = nodes[i];
}

void CityRenderer::setDronePosition(float x, float y) {
    droneX = x;
    droneY = y;
}

// ═════════════════════════════════════════════════════════════
// loadSampleScene — mirrors the city_nodes data
// ═════════════════════════════════════════════════════════════
void CityRenderer::loadSampleScene() {
    // Buildings (x, y, width, height, R, G, B)
    addBuilding( 1.0f,  1.5f, 0.6f, 1.5f, 0.4f, 0.5f, 0.7f);
    addBuilding(-0.5f,  2.5f, 0.5f, 1.0f, 0.5f, 0.4f, 0.6f);
    addBuilding( 3.5f, -0.5f, 0.7f, 2.0f, 0.3f, 0.6f, 0.5f);
    addBuilding(-2.0f,  1.0f, 0.6f, 1.2f, 0.6f, 0.5f, 0.4f);
    addBuilding( 4.5f,  2.0f, 0.5f, 1.8f, 0.5f, 0.5f, 0.5f);
    addBuilding( 6.0f, -1.0f, 0.8f, 1.5f, 0.4f, 0.6f, 0.6f);

    // A* path: Warehouse → Junction1 → Customer_A
    PathNode path[] = {
        {0.0f, 0.0f},   // Warehouse
        {3.0f, 2.0f},   // Junction_1
        {5.0f, 3.0f}    // Customer_A
    };
    setPath(path, 3);

    // Drone starts at warehouse
    setDronePosition(0.0f, 0.0f);
}

// ═════════════════════════════════════════════════════════════
// drawGround — large grey quad as the city floor
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawGround() const {
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(-10.0f, -10.0f);
        glVertex2f( 10.0f, -10.0f);
        glVertex2f( 10.0f,  10.0f);
        glVertex2f(-10.0f,  10.0f);
    glEnd();
}

// ═════════════════════════════════════════════════════════════
// drawBuilding — single GL_QUADS building + outline
// Uses glPushMatrix / glTranslatef / glScalef (CO2)
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawBuilding(const Building& b) const {
    glPushMatrix();
        // Translation — position in world space
        glTranslatef(b.x, b.y, 0.0f);

        // Scale — size of building
        glScalef(b.width, b.height, 1.0f);

        // Filled quad (building body)
        glColor3f(b.r, b.g, b.b);
        glBegin(GL_QUADS);
            glVertex2f(-0.5f, 0.0f);
            glVertex2f( 0.5f, 0.0f);
            glVertex2f( 0.5f, 1.0f);
            glVertex2f(-0.5f, 1.0f);
        glEnd();

        // Outline (darker)
        glColor3f(b.r * 0.5f, b.g * 0.5f, b.b * 0.5f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(-0.5f, 0.0f);
            glVertex2f( 0.5f, 0.0f);
            glVertex2f( 0.5f, 1.0f);
            glVertex2f(-0.5f, 1.0f);
        glEnd();
    glPopMatrix();
}

// ═════════════════════════════════════════════════════════════
// drawPath — GL_LINES connecting waypoints
// Colour: RED = A* route
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawPath() const {
    if (pathNodeCount < 2) return;

    glColor3f(1.0f, 0.2f, 0.2f);   // red = A* path
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < pathNodeCount - 1; i++) {
        glVertex2f(pathNodes[i].x,     pathNodes[i].y);
        glVertex2f(pathNodes[i+1].x,   pathNodes[i+1].y);
    }
    glEnd();
    glLineWidth(1.0f);
}

// ═════════════════════════════════════════════════════════════
// drawNodes — GL_POINTS for city locations
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawNodes() const {
    glPointSize(8.0f);

    // Warehouse — yellow
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
    glEnd();

    // Customers — cyan
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
        glVertex2f( 5.0f,  3.0f);
        glVertex2f(-3.0f,  4.0f);
        glVertex2f( 7.0f, -2.0f);
    glEnd();

    // Charging — green
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
        glVertex2f(2.0f, 5.0f);
    glEnd();

    // Junctions — white
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_POINTS);
        glVertex2f( 3.0f,  2.0f);
        glVertex2f(-1.0f,  3.0f);
        glVertex2f( 4.0f, -1.0f);
    glEnd();

    glPointSize(1.0f);
}

// ═════════════════════════════════════════════════════════════
// drawDrone — small quad at drone position
// Uses glTranslatef (CO2)
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawDrone() const {
    glPushMatrix();
        glTranslatef(droneX, droneY, 0.0f);
        glColor3f(1.0f, 0.8f, 0.0f);   // orange
        float s = 0.3f;
        glBegin(GL_QUADS);
            glVertex2f(-s, -s);
            glVertex2f( s, -s);
            glVertex2f( s,  s);
            glVertex2f(-s,  s);
        glEnd();
        // Cross arms
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f(-s*1.5f, 0.0f);  glVertex2f(s*1.5f, 0.0f);
            glVertex2f(0.0f, -s*1.5f);  glVertex2f(0.0f,   s*1.5f);
        glEnd();
        glLineWidth(1.0f);
    glPopMatrix();
}

// ═════════════════════════════════════════════════════════════
// drawHUD — simple text overlay
// ═════════════════════════════════════════════════════════════
void CityRenderer::drawHUD() const {
    // Switch to screen-space orthographic for HUD text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, winH - 20);
    const char* title = "AeroRoute — Task 1 | CGL CO1 & CO2";
    for (const char* c = title; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);

    glRasterPos2i(10, winH - 38);
    const char* legend =
        "[Yellow] Warehouse  [Cyan] Customer  "
        "[Green] Charging  [Red line] A* Path";
    for (const char* c = legend; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);

    glRasterPos2i(10, 15);
    const char* keys = "ESC: Quit";
    for (const char* c = keys; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ═════════════════════════════════════════════════════════════
// render — master draw function (called each frame)
// ═════════════════════════════════════════════════════════════
void CityRenderer::render() const {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawGround();
    for (int i = 0; i < buildingCount; i++)
        drawBuilding(buildings[i]);
    drawPath();
    drawNodes();
    drawDrone();
    drawHUD();

    glutSwapBuffers();
}

// ═════════════════════════════════════════════════════════════
// GLUT Callbacks
// ═════════════════════════════════════════════════════════════
void display_callback() {
    if (g_renderer) g_renderer->render();
}

void reshape_callback(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = static_cast<float>(w) / h;
    // Show ±10 world units, adjusted for aspect ratio
    gluOrtho2D(-10.0f * aspect, 10.0f * aspect, -10.0f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard_callback(unsigned char key, int /*x*/, int /*y*/) {
    if (key == 27) exit(0);   // ESC to quit
}