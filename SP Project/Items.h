#pragma once
#include "Entity.h"
struct Items:
    public Entity
{
    using::Entity::Entity;
public:
    //Public Variables
    float time = 0;
    Vector2f velocity = { (float)((rand() % 40 - 20)) * 3,(float)((rand() % 40 - 20) * 3) };
 
    float dt_v_x = velocity.x, dt_v_y = velocity.y;
     ~Items();
    void move();
    void intersect();
    void update();
    void pollevent();
};

