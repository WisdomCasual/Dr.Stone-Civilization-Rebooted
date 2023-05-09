#pragma once
#include "Entity.h"
struct Items:
    public Entity
{
    using::Entity::Entity;
public:
    //Public Variables
    int item_ID = 0;
    bool taken = 0;
    float time = 0;
    Vector2f velocity = { (rand() % 40 - 20),(rand() % 40 - 20) };
    
     ~Items();
    void move();
    bool intersect();
    void update();
    void pollevent();
};

