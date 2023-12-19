#include "EnemyFunctions.h"

void EnemyFunctions::HitKnockback(olc::PixelGameEngine* pge, std::vector<int> IsHit, std::vector<olc::vf2d> EnemyPos, olc::vf2d PlayerPos) {
	for (int k = 0; k < EnemyPos.size(); k++) {
		if (IsHit[k] == 1) {
			olc::vf2d dir = (PlayerPos - EnemyPos[k]).norm();
		}
	}
}