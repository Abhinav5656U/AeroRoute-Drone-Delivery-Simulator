#ifndef CITY_GRID_H
#define CITY_GRID_H

// AeroRoute — Task 2 (PL / CO2): Singly Linked List
// City nodes stored using a dynamically allocated singly linked list.
// Demonstrates: insert, delete, update, search, display
// Team Member: Abhinav Kumar (202501090028)

#define MAX_NAME_LEN  30

// Node types in the city
enum NodeType {
    WAREHOUSE,
    CUSTOMER,
    CHARGING,
    JUNCTION
};

// A single city location — now a singly linked-list node
struct CityNode {
    int       id;
    char      name[MAX_NAME_LEN];
    float     x, y;          // 2D coordinates on the city map
    NodeType  type;
    CityNode* next;          // pointer to the next node in the list
};

// CityGrid — singly linked-list based city map

class CityGrid {
private:
    CityNode* head;      // pointer to the first node in the list
    int       count;     // current number of nodes
    int       nextId;    // auto-increment ID

    // Helper: find node pointer by ID (nullptr if not found)
    CityNode* findNodeById(int id) const;

public:
    CityGrid();
    ~CityGrid();   // destructor — traverses list and frees every node

    // Core linked-list operations required by Task 2
    bool insertNode(const char* name, float x, float y, NodeType type);
    bool deleteNode(int id);
    bool updateNode(int id, float newX, float newY);
    int  searchByName(const char* name) const;  // returns id, -1 if not found
    void displayAll()  const;
    void displayNode(int id) const;

    // Utility
    int  getCount() const { return count; }
    const CityNode* getNode(int id) const;

    // Load default city for demo
    void loadSampleCity();
};

#endif // CITY_GRID_H
