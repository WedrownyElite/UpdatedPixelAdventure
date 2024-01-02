#include "MathFunctions.h"

float MathFunctions::GetDistance(olc::vf2d EnemyPos, olc::vf2d PlayerPos) {
	float dx = EnemyPos.x - PlayerPos.x;
	float dy = EnemyPos.y - PlayerPos.y;

	float dis = (dx * dx + dy * dy);

	return dis;
}
float MathFunctions::angleDifference(float angle_1, float angle_2) {
	angle_1 = fmod(angle_1, 2 * 3.14159f);
	angle_2 = fmod(angle_2, 2 * 3.14159f);
	float angle_diff = angle_1 - angle_2;

	if (angle_diff > 3.14159f)
		angle_diff -= 2 * 3.14159f;
	else if (angle_diff < -3.14159f)
		angle_diff += 2 * 3.14159f;

	return -angle_diff;
}
olc::vf2d MathFunctions::GetWorldMousePos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	return pge->GetMousePos() / 32 + tv.GetWorldOffset();
}
float MathFunctions::PointTo(olc::vf2d pos1, olc::vf2d pos2) {
	return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
}
uint32_t MathFunctions::Lehmer32(uint32_t nLehmer) {
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t  m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c0;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}