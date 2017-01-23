#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "AI.h"

typedef struct decisionTreeNode* Node;
typedef Input (*LeafNodeFunc)(GameState*, int);
typedef bool (*BranchNodeFunc)(GameState*, int);

typedef enum NodeType
{
    BranchNode,
    LeafNode
}NodeType;

typedef struct bNode
{
    BranchNodeFunc func;
    Node yes, no;
}bNode;

typedef struct lNode
{
    LeafNodeFunc func;
}lNode;

union Nodes
{
    bNode b;
    lNode l;
};

struct decisionTreeNode
{
    int id;
    NodeType type;
    union Nodes node;
};

Node makeDT();
Node AI_makeDTNode(int id);
void AI_makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no);
void AI_makeLeafNode(Node n, LeafNodeFunc func);
Input runToBall(GameState* gs, int i);
bool returnRandBool(GameState* gs, int i);

Node AI_init()
{
    srand((int)time(NULL));
    return makeDT();
}

// function that traverses decision tree and return AI input
Input AI_getUserInput(GameState* gs, int id, Node start)
{
    //dummy function returning null input for the sake of testing

    if(start->type == BranchNode)
    {
        Node next = start->node.b.func(gs, id) ? start->node.b.yes : start->node.b.no;
        if(next == NULL) return INPUT_NULL;
        return (AI_getUserInput(gs, id, next));
    }else
    {
        return start->node.l.func(gs, id);
    }
}


/*** for testing, delete when complete!!!!!*/
Input stop(GameState* gs, int id)
{
    Input i = INPUT_NULL;
    i.shot = VECTOR_N;
    return i;
}

Node makeDT()
{
    Node n0 = AI_makeDTNode(0);
    Node n1 = AI_makeDTNode(1);
    Node n2 = AI_makeDTNode(2);
    AI_makeBranchNode(n0, &returnRandBool, n1, n2);
    AI_makeLeafNode(n1, &stop);
    AI_makeLeafNode(n2, &runToBall);
    return n0;
}

bool returnRandBool(GameState* gs, int i)
{
    double r = rand();
    r /= RAND_MAX;
    bool res = (r > 0.5);
    return res;
}

Input runToBall(GameState* gs, int i)
{
    Input in = INPUT_NULL;
    Vec3D toBall = Vec3D_subtract(GO_getPos(gs->ball), Player_getPos(gs->players[1]));
    toBall = Vec3D_scalarMult(Vec3D_normalise(toBall), 0.1);
    in.direction = toBall;
    return in;
}

/********************************************/

Node AI_makeDTNode(int id)
{
    Node n = malloc(sizeof(struct decisionTreeNode));
    n->id = id;
    return n;
}

void AI_makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no)
{
    n->type = BranchNode;
    n->node.b.func = func;
    n->node.b.yes = yes;
    n->node.b.no = no;
}

void AI_makeLeafNode(Node n, LeafNodeFunc func)
{
    n->type = LeafNode;
    n->node.l.func = func;
}

