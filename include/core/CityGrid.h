#ifndef CITY_GRID_H
#define CITY_GRID_H

// AeroRoute — Task 1 (PL / CO1): Arrays
// City nodes stored in a fixed-size array.
// Demonstrates: insert, delete, update, search, display
// Team Member: Abhinav Kumar (202501090028)


#define MAX_NODES     50
#define MAX_NAME_LEN  30

// Node types in the city
enum NodeType {
    WAREHOUSE,
    CUSTOMER,
    CHARGING,
    JUNCTION
};

// A single city location (node)
struct CityNode {
    int    id;
    char   name[MAX_NAME_LEN];
    float  x, y;          // 2D coordinates on the city map
    NodeType type;
    bool   isActive;      // false = logically deleted
};

// CityGrid — array-based city map

class CityGrid {
private:
    CityNode nodes[MAX_NODES];   // fixed-size array (CO1)
    int      count;              // current number of nodes
    int      nextId;             // auto-increment ID

    // Helper: find index by ID (-1 if not found)
    int findIndexById(int id) const;

public:
    CityGrid();

    // Core array operations required by Task 1
    bool insertNode(const char* name, float x, float y, NodeType type);
    bool deleteNode(int id);
    bool updateNode(int id, float newX, float newY);
    int  searchByName(const char* name) const;  // returns id, -1 if not found
    void displayAll()  const;
    void displayNode(int id) const;

    // Utility
    int  getCount()    const { return count; }
    bool isFull()      const { return count >= MAX_NODES; }
    const CityNode* getNode(int id) const;

    // Load default city for demo
    void loadSampleCity();
};

#endif // CITY_GRID_H

