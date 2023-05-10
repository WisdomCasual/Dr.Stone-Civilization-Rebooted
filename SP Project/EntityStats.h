#pragma once
#include"State.h"
struct base_stats {
	short health;
	short* item_drops;
	short drops_no = 0;
	short damage = 0;
	short speed = 0;
};

base_stats object_stats[30];