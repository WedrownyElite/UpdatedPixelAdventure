#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "GlobalVariables.h"

class Player {

public:
	olc::vf2d MovementDirection;
	olc::vf2d UpdatedPlayerPos;
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
	std::unique_ptr<olc::Sprite> PlayerRightAttackSS;
	std::unique_ptr<olc::Sprite> PlayerLeftAttackSS;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;
	olc::Decal* WalkLeftSSDecal;
	olc::Decal* WalkRightSSDecal;
	olc::Decal* PlayerRightAttackSSDecal;
	olc::Decal* PlayerLeftAttackSSDecal;

	void AttackInput(olc::PixelGameEngine* pge, float fElapsedTime, bool& PlayerAttacked, olc::vf2d MousePos, bool& AttackAnim);
	bool AttackAnimcheck();
	bool MovingCheck(olc::PixelGameEngine* pge);
	olc::vf2d PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed, bool& PlayerWalking);
	void DrawPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime, bool& PlayerWalking, bool& AttackAnim);
	void Initialize();

};