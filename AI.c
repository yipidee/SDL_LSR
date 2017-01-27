#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "AI.h"

typedef struct decisionTreeNode* Node;

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

struct BFuncEntry BFuncList[] = {
            {"randomTrueFalse", &returnRandBool},
            {"END", NULL}
            };

struct LFuncEntry LFuncList[] = {
            {"runToBall", &runToBall},
            {"stop", &stop},
            {"END", NULL}
            };

size_t NodeSize;

void _makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no);
void _makeLeafNode(Node n, LeafNodeFunc func);
BranchNodeFunc lookupBFunc(char* funcName);
LeafNodeFunc lookupLFunc(char* funcName);

Node AI_init()
{
    NodeSize = sizeof(struct decisionTreeNode);
    srand((int)time(NULL));
    return AI_parseDecisionTree("Assets/decision_trees/_default_.dt");
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

void AI_makeBranchNode(DecisionTree dt, int id, char* func, int yes, int no)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    Node n = dt + id*NodeSize;
    BranchNodeFunc f = lookupBFunc(func);
    Node yesn = dt + yes*NodeSize;
    Node non = dt + no*NodeSize;
    _makeBranchNode(n, f, yesn, non);
}
void _makeBranchNode(Node n, BranchNodeFunc func, Node yes, Node no)
{
    n->type = BranchNode;
    n->node.b.func = func;
    n->node.b.yes = yes;
    n->node.b.no = no;
}

void AI_makeLeafNode(DecisionTree dt, int id, char* func)
{
    if(!func || !dt) {printf("Null pointer for func name or tree when making Decision tree");exit(-1);}
    Node n = dt + id*NodeSize;
    LeafNodeFunc f = lookupLFunc(func);
    _makeLeafNode(n, f);
}

void _makeLeafNode(Node n, LeafNodeFunc func)
{
    n->type = LeafNode;
    n->node.l.func = func;
}

void AI_freeDecisionTree(DecisionTree dt)
{
    free(dt);
}

BranchNodeFunc lookupBFunc(char* funcName)
{
    if(funcName!=NULL)
    {
        int i = 0;
        struct BFuncEntry b = BFuncList[i];
        while(b.func)
        {
            if(!strcmp(funcName, b.name)) return b.func;
            ++i;
            b = BFuncList[i];
        }
    }
    return NULL;
}

LeafNodeFunc lookupLFunc(char* funcName)
{
    if(funcName!=NULL)
    {
        int i = 0;
        struct LFuncEntry l = LFuncList[i];
        while(l.func)
        {
            if(!strcmp(funcName, l.name)) return l.func;
            ++i;
            l = LFuncList[i];
        }
    }
    return NULL;
}
