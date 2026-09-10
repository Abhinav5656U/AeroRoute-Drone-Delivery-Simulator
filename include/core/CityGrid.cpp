#include "core/CityGrid.h"
#include <cstring>
#include <cstdio>

// ─────────────────────────────────────────────────────────────
// Constructor — zero-initialise the array
// ─────────────────────────────────────────────────────────────
CityGrid::CityGrid() : count(0), nextId(0) {
    for (int i = 0; i < MAX_NODES; i++) {
        nodes[i].isActive = false;
        nodes[i].id       = -1;
    }
}

// ─────────────────────────────────────────────────────────────
// Private helper — scan active nodes for matching ID
// ─────────────────────────────────────────────────────────────
int CityGrid::findIndexById(int id) const {
    for (int i = 0; i < MAX_NODES; i++) {
        if (nodes[i].isActive && nodes[i].id == id)
            return i;
    }
    return -1;
}

// ─────────────────────────────────────────────────────────────
// INSERT — add a new city node into the array
// ─────────────────────────────────────────────────────────────
bool CityGrid::insertNode(const char* name, float x, float y, NodeType type) {
    if (isFull()) {
        std::cout << "[CityGrid] ERROR: Array is full. Cannot insert.\n";
        return false;
    }

    // Find first inactive slot
    for (int i = 0; i < MAX_NODES; i++) {
        if (!nodes[i].isActive) {
            nodes[i].id       = nextId++;
            strncpy(nodes[i].name, name, MAX_NAME_LEN - 1);
            nodes[i].name[MAX_NAME_LEN - 1] = '\0';
            nodes[i].x        = x;
            nodes[i].y        = y;
            nodes[i].type     = type;
            nodes[i].isActive = true;
            count++;
            std::cout << "[CityGrid] Inserted: ID=" << nodes[i].id
                      << " Name=" << nodes[i].name << "\n";
            return true;
        }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────
// DELETE — logical delete (mark isActive = false)
// ─────────────────────────────────────────────────────────────
bool CityGrid::deleteNode(int id) {
    int idx = findIndexById(id);
    if (idx == -1) {
        std::cout << "[CityGrid] ERROR: Node ID=" << id << " not found.\n";
        return false;
    }
    nodes[idx].isActive = false;
    count--;
    std::cout << "[CityGrid] Deleted: ID=" << id
              << " Name=" << nodes[idx].name << "\n";
    return true;
}

// ─────────────────────────────────────────────────────────────
// UPDATE — change coordinates of an existing node
// ─────────────────────────────────────────────────────────────
bool CityGrid::updateNode(int id, float newX, float newY) {
    int idx = findIndexById(id);
    if (idx == -1) {
        std::cout << "[CityGrid] ERROR: Node ID=" << id << " not found.\n";
        return false;
    }
    nodes[idx].x = newX;
    nodes[idx].y = newY;
    std::cout << "[CityGrid] Updated: ID=" << id
              << " New coords=(" << newX << ", " << newY << ")\n";
    return true;
}

// ─────────────────────────────────────────────────────────────
// SEARCH — linear search by name, returns ID
// ─────────────────────────────────────────────────────────────
int CityGrid::searchByName(const char* name) const {
    for (int i = 0; i < MAX_NODES; i++) {
        if (nodes[i].isActive &&
            strncmp(nodes[i].name, name, MAX_NAME_LEN) == 0) {
            std::cout << "[CityGrid] Found: ID=" << nodes[i].id
                      << " at (" << nodes[i].x << ", "
                      << nodes[i].y << ")\n";
            return nodes[i].id;
        }
    }
    std::cout << "[CityGrid] Not found: " << name << "\n";
    return -1;
}

// ─────────────────────────────────────────────────────────────
// DISPLAY — print a single node
// ─────────────────────────────────────────────────────────────
void CityGrid::displayNode(int id) const {
    int idx = findIndexById(id);
    if (idx == -1) {
        std::cout << "[CityGrid] Node ID=" << id << " not found.\n";
        return;
    }
    const CityNode& n = nodes[idx];
    const char* types[] = {"WAREHOUSE","CUSTOMER","CHARGING","JUNCTION"};
    printf("  [%02d] %-20s  (%.1f, %.1f)  Type: %s\n",
           n.id, n.name, n.x, n.y, types[n.type]);
}

// ─────────────────────────────────────────────────────────────
// DISPLAY ALL — iterate array and print every active node
// ─────────────────────────────────────────────────────────────
void CityGrid::displayAll() const {
    std::cout << "\n════════════════════════════════════════\n";
    std::cout << "  AeroRoute City Map  (" << count << " nodes)\n";
    std::cout << "════════════════════════════════════════\n";
    const char* types[] = {"WAREHOUSE","CUSTOMER","CHARGING","JUNCTION"};
    for (int i = 0; i < MAX_NODES; i++) {
        if (nodes[i].isActive) {
            printf("  [%02d] %-20s  (%.1f, %.1f)  Type: %s\n",
                   nodes[i].id,
                   nodes[i].name,
                   nodes[i].x,
                   nodes[i].y,
                   types[nodes[i].type]);
        }
    }
    std::cout << "════════════════════════════════════════\n\n";
}

// ─────────────────────────────────────────────────────────────
// GET NODE — const pointer for read-only access
// ─────────────────────────────────────────────────────────────
const CityNode* CityGrid::getNode(int id) const {
    int idx = findIndexById(id);
    if (idx == -1) return nullptr;
    return &nodes[idx];
}

// ─────────────────────────────────────────────────────────────
// LOAD SAMPLE CITY — pre-populate for demo
// ─────────────────────────────────────────────────────────────
void CityGrid::loadSampleCity() {
    insertNode("Warehouse_Central",  0.0f,  0.0f, WAREHOUSE);
    insertNode("Customer_A",         5.0f,  3.0f, CUSTOMER);
    insertNode("Customer_B",        -3.0f,  4.0f, CUSTOMER);
    insertNode("Customer_C",         7.0f, -2.0f, CUSTOMER);
    insertNode("Charging_North",     2.0f,  5.0f, CHARGING);
    insertNode("Junction_1",         3.0f,  2.0f, JUNCTION);
    insertNode("Junction_2",        -1.0f,  3.0f, JUNCTION);
    insertNode("Junction_3",         4.0f, -1.0f, JUNCTION);
    std::cout << "[CityGrid] Sample city loaded.\n";
}