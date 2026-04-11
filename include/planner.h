#ifndef PLANNER_H
#define PLANNER_H

#include <stdint.h>
#include "data.h"

// FOR THE BISON PARSING RECURSION
typedef struct TempCol {
    Column data;
    struct TempCol *next;
} TempCol;
  
typedef enum { 
    PLANNER_NODE_SELECT, 
    PLANNER_NODE_TABLE_REF, 
    PLANNER_NODE_COLUMN_LIST 
} PlannerNodeType;

typedef struct Node {
    PlannerNodeType planner_node_type;
    union {
        char* sval;        // For identifiers
        int64_t ival;      // For OIDs/Numbers
    } data;
    struct PlannerNode *left, *right; 
} PlannerNode;

// Helper function to create nodes
PlannerNode* create_planner_node(PlannerNodeType type, PlannerNode* left, PlannerNode* right);


#endif
