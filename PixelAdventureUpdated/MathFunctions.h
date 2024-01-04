#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class MathFunctions {
public:
	static float GetDistance(olc::vf2d EnemyPos, olc::vf2d PlayerPos);
	static float angleDifference(float angle_1, float angle_2);
	static olc::vf2d GetWorldMousePos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge);
	static float PointTo(olc::vf2d pos1, olc::vf2d pos2);
	uint32_t Lehmer32(uint32_t nLehmer);
	uint32_t NoSeedLehmer32();
	float NoSeedLehmerFloatRange(float min, float max);
	void GenerateNewGoal(olc::vf2d Home, olc::vf2d& Goal, float min, float max);
};