#include "Goal.h"

struct _Goal
{
    GameObject* posts[2];
    int diameter;

    //useful properties probably better to calc once
    Vec3D normalToGoal;
    int goalWidth; //considers diameter of posts
};

Goal Goal_createGoal(Vec3D p1, Vec3D p2, int d)
{
    //assign memory for goal and set up game objects
    Goal g = malloc(sizeof(struct _Goal));
    g->posts[0] = GO_createGameObject();
    GO_setPos(g->posts[0], p1);
    GO_setStationary(g->posts[0], true);
    g->posts[1] = GO_createGameObject();
    GO_setPos(g->posts[1], p2);
    GO_setStationary(g->posts[1], true);
    g->diameter = d;

    //set up bounding circles
    Circle lBC = Circle_create(p1.i, p1.j, d/2);
    GO_setBCirc(g->posts[0], lBC);
    Circle rBC = Circle_create(p2.i, p2.j, d/2);
    GO_setBCirc(g->posts[1], rBC);

    //calc properties
    Vec3D l2rVec = Vec3D_subtract(p2, p1);
    g->normalToGoal = Vec3D_getNormal(l2rVec, VECTOR_UP);
    g->goalWidth = (int)Vec3D_getMagnitude(l2rVec);

    return g;
}

//Must be called after Gameobjects destroyed!!
void Goal_destroyGoal(Goal g)
{
    if(g)free(g);
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
