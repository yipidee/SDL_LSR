#include <math.h>
#include <stdlib.h>
#include "Goal.h"
#include "Constants.h"

struct _Goal
{
    GameObject* posts[2];
    int diameter;
    //useful properties probably better to calc once
    Vec3D normalToGoal;
    int goalWidth; //considers diameter of posts
    Vec3D LastBallPosition;
};

//static Vec3D LastBallPosition;

Goal Goal_createGoal(Vec3D p1, Vec3D p2, int d)
{
    //assign memory for goal and set up game objects
    Goal g = malloc(sizeof(struct _Goal));
    g->posts[0] = GO_createGameObject();
    GO_setOffsetPos(g->posts[0], p1);
    GO_setStationary(g->posts[0], true);
    g->posts[1] = GO_createGameObject();
    GO_setOffsetPos(g->posts[1], p2);
    GO_setStationary(g->posts[1], true);
    g->diameter = d;

    //set up bounding circles
    Circle lBC = Circle_create(p1.i, p1.j, d/2);
    GO_setBCirc(g->posts[0], lBC);
    Circle rBC = Circle_create(p2.i, p2.j, d/2);
    GO_setBCirc(g->posts[1], rBC);

    g->LastBallPosition = Vec3D_makeVector(-1, -1, -1);

    Vec3D l2rVec = Vec3D_subtract(p2, p1);
    g->normalToGoal = Vec3D_getNormal(l2rVec, VECTOR_UP);
    //Vec3D_print(g->normalToGoal);printf("\n");
    g->goalWidth = (int)Vec3D_getMagnitude(l2rVec);

    return g;
}

//Must be called after Gameobjects destroyed!!
void Goal_destroyGoal(Goal g)
{
    if(g)free(g);
}

bool Goal_scored(Goal g, GameObject* ball)
/* Goal deemed legit if it crosses the line in the correct direction in this tick */
{
    bool res = false;

    if(Vec3D_equal(g->LastBallPosition, Vec3D_makeVector(-1, -1, -1)))
    {
        g->LastBallPosition = GO_getPos(ball);
        return false;
    }

    Line goalLine, pathOfBall;

    //get positions of goal posts and ball
    goalLine.p1 = GO_getPos(g->posts[0]);
    goalLine.p2 = GO_getPos(g->posts[1]);
    pathOfBall.p1 = g->LastBallPosition;
    pathOfBall.p2 = GO_getPos(ball);

    res = Line_lineCrossesLineInNormalDirection(goalLine, pathOfBall);

    //update last position of ball
    g->LastBallPosition = GO_getPos(ball);

    return res;
}

int Goal_getWidth(Goal g)
{
    return g->goalWidth;
}

Vec3D Goal_getNormal(Goal g)
{
    return g->normalToGoal;
}

GameObject* Goal_getLPost(Goal g)
{
    return g->posts[0];
}

GameObject* Goal_getRPost(Goal g)
{
    return g->posts[1];
}
