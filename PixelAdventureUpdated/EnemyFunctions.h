#pragma once
#include "olcPixelGameEngine.h"
#include <vector>

class EnemyFunctions {
public:
	olc::vf2d HitKnockback(olc::PixelGameEngine* pge, int IsHit, olc::vf2d EnemyPos, olc::vf2d PlayerPos);
	void IdleActivityTimer(float fElapsedTime,  float& Timer, bool& ActiveBool);
};
