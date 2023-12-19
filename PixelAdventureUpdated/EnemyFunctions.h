#pragma once
#include "olcPixelGameEngine.h"
#include <vector>

class EnemyFunctions {
public:
	void HitKnockback(olc::PixelGameEngine* pge, std::vector<int> IsHit, std::vector<olc::vf2d> EnemyPos, olc::vf2d PlayerPos);
};
