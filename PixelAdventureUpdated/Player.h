#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Player {

public:
	olc::vf2d UpdatedPlayerPos;
	bool PlayerWalking = false;
	bool CanAttack = true;
	float AttackCooldown = 0.0f;
	olc::vf2d PlayerPos = { 300, 300 };
	//False = left, True = right
	bool Dir = false;

	//Sprites
	std::unique_ptr<olc::Sprite> PlayerRight;
	std::unique_ptr<olc::Sprite> PlayerLeft;
	std::unique_ptr<olc::Sprite> PlayerDead;
	std::unique_ptr<olc::Sprite> Shadow;
	std::unique_ptr<olc::Sprite> WalkLeftSS;
	std::unique_ptr<olc::Sprite> WalkRightSS;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;
	olc::Decal* WalkLeftSSDecal;
	olc::Decal* WalkRightSSDecal;

	bool AttackInput(olc::PixelGameEngine* pge, float fElapsedTime, bool PlayerAttacked);
	bool MovingCheck(olc::PixelGameEngine* pge);
	olc::vf2d PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed);
	void DrawPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime);
	void Initialize(olc::PixelGameEngine* pge);

};