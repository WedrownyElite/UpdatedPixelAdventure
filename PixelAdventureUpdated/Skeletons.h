#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Skeletons {
public:
	olc::vf2d SkelePos;
	int SkeleHit;
	int RedSkele;
	int RedSkeleTimer;
	float KnockbackDist;
};
class SkeletonFunctions {
private:
	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;

	std::vector<int> SkeleBelow;
	std::vector<int> SkeleAbove;
public:
	void IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, std::vector<Skeletons>& Skeles, bool& PlayerAttacked, olc::vf2d PlayeroPos);
	void Knockback(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, std::vector<Skeletons>& Skeles);
	void Collision(olc::PixelGameEngine* pge, std::vector<Skeletons>& Skeles, olc::vf2d PlayerPos, float PlayerSpeed);
	void SpawnSkeleton(std::vector<Skeletons>& Skeles);
	void DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, std::vector<Skeletons> Skeles);
	void DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles);
	void DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles);
	void Initialize();
};
