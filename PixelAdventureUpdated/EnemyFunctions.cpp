#include "EnemyFunctions.h"

olc::vf2d EnemyFunctions::HitKnockback(olc::PixelGameEngine* pge, int IsHit, olc::vf2d EnemyPos, olc::vf2d PlayerPos) {
	if (IsHit == 1) {
		olc::vf2d dir = (PlayerPos - EnemyPos).norm();
	}
}