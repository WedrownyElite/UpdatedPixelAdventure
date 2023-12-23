#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Player {

public:
	olc::vf2d MovementDirection;
	olc::vf2d UpdatedPlayerPos;
	bool PlayerWalking = false;
	bool CanAttack = true;
	bool AttackAnim = false;
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
	std::unique_ptr<olc::Sprite> PlayerRightAttackSS;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;
	olc::Decal* WalkLeftSSDecal;
	olc::Decal* WalkRightSSDecal;
	olc::Decal* PlayerRightAttackSSDecal;

	bool AttackInput(olc::PixelGameEngine* pge, float fElapsedTime, bool PlayerAttacked, olc::vf2d MousePos);
	bool MovingCheck(olc::PixelGameEngine* pge);
	olc::vf2d PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed);
	void DrawPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime);
	void Initialize();

};