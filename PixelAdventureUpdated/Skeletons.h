#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Skeleton {
private:
	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;

	std::vector<olc::vf2d> SkelePos;
	std::vector<int> SkeleBelow;
	std::vector<int> SkeleAbove;
	std::vector<int> SkeleHit;
public:
	bool IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, bool PlayerAttacked, olc::vf2d PlayeroPos);
	olc::vf2d Collision(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed);
	void SpawnSkeleton();
	void DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed);
	void DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge);
	void DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge);
	void Initialize();
};
