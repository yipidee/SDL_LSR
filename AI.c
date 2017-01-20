#include <stdlib.h>
#include <stdbool.h>
#include "AI.h"

typedef Input (*LeafNodeFunc)(GameState*, int);
typedef bool (*BranchNodeFunc)(GameState*, int);

typedef enum NodeType
{
    BranchNode,
    LeafNode
}NodeType;

union funcType
{
    BranchNodeFunc b;
    LeafNodeFunc l;
};

struct decisionTreeNode
{
    int id;
    NodeType type;
    union funcType func;
    Node yes, no;
};

Node makeDT();
Node AI_makeDTNode(int id);
void AI_makeLeafNode(Node n, LeafNodeFunc func);

Node AI_init()
{
    return makeDT();
}

// function that traverses decision tree and return AI input
Input AI_getUserInput(GameState* gs, int id, Node start)
{
    //dummy function returning null input for the sake of testing

    if(start->type == BranchNode)
    {
        Node next = start->func.b(gs, id) ? start->yes : start->no;
        return (AI_getUserInput(gs, id, next));
    }else
    {
        return start->func.l(gs, id);
    }
}


/*** for testing, delete when complete!!!!!*/
Input dummyFunc(GameState* gs, int id)
{
    Input i;
    i.direction = Vec3D_scalarMult(VECTOR_S, 0.1);
    i.control = VECTOR_ZERO;
    i.shot = VECTOR_ZERO;
    return i;
}

Node makeDT()
{
    Node n = AI_makeDTNode(0);
    AI_makeLeafNode(n, &dummyFunc);
    return n;
}

/********************************************/

Node AI_makeDTNode(int id)
{
    Node n = malloc(sizeof(struct decisionTreeNode));
    n->id = id;
    n->yes = NULL;
    n->no = NULL;
    return n;
}

void AI_makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no)
{
    n->type = BranchNode;
    n->func.b = func;
    n->yes = yes;
    n->no = no;
}

void AI_makeLeafNode(Node n, LeafNodeFunc func)
{
    n->type = LeafNode;
    n->func.l = func;
}

