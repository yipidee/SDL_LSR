#include <math.h>
#include "Goal.h"
#include "Constants.h"

struct _Goal
{
    GameObject* posts[2];
    int diameter;
    Rect net;
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
    //Vec3D_print(g->normalToGoal);printf("\n");
    g->goalWidth = (int)Vec3D_getMagnitude(l2rVec);

    Rect goalArea;
    if(Vec3D_equal(g->normalToGoal, VECTOR_S))
    {
        goalArea.x = GO_getPos(g->posts[0]).i;
        goalArea.y = GO_getPos(g->posts[0]).j - 50;
        goalArea.w = g->goalWidth;
        goalArea.h = 50;
    }else
    {
        goalArea.x = GO_getPos(g->posts[1]).i;
        goalArea.y = GO_getPos(g->posts[1]).j;
        goalArea.w = g->goalWidth;
        goalArea.h = 50;
    }
    g->net = goalArea;

    return g;
}

//Must be called after Gameobjects destroyed!!
void Goal_destroyGoal(Goal g)
{
    if(g)free(g);
}

bool Goal_scored(Goal g, GameObject* ball)
/* Goal deemed legit if in net and has a j component of velocity
   in opposite direction of goal normal
*/
{
    bool res = false;

    if(Rect_containsCircle(g->net, ball->BCirc) && (GO_getVel(ball).j != 0))
    {
        if((fabs(g->normalToGoal.j) + fabs(GO_getVel(ball).j)) != fabs(g->normalToGoal.j + GO_getVel(ball).j))res = true;
    }

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
