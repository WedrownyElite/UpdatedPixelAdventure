#include "EnemyFunctions.h"

olc::vf2d EnemyFunctions::HitKnockback(olc::PixelGameEngine* pge, int IsHit, olc::vf2d EnemyPos, olc::vf2d PlayerPos) {
	if (IsHit == 1) {
		olc::vf2d dir = (PlayerPos - EnemyPos).norm();

		EnemyPos += dir;
	}
	return EnemyPos;
}
void EnemyFunctions::IdleActivityTimer(float fElapsedTime, float& Timer, bool& CooldownBool) {
	//Cooldown
	if (Timer > 0.0f) {
		Timer -= fElapsedTime;
	}
	//Cooldown ended
	if (Timer <= 0.0f) {
		Timer = 0.0f;
		CooldownBool = false;
	}
}