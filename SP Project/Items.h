#pragma once
#include "Entity.h"
struct Items:
    public Entity
{
    using::Entity::Entity;
public:
    //Public Variables
    float time = 0;
    Vector2f velocity = { (float)((rand() % 40 - 20) / 40.0),(float)((rand() % 40 - 20) / 40.0) };
 
    float dt_v_x = velocity.x / 1.0, dt_v_y = velocity.y / 1.0;
     ~Items();
    void move();
    void intersect();
    void update();
    void pollevent();
};

