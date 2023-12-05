#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "GlobalVars.h"

class Player {

public:
	bool WalkingLeft = false;
	bool WalkingRight = false;
	olc::vf2d PlayerPos = { 300, 300 };
	//False = left, True = right
	bool Dir = false;

	//Sprites
	std::unique_ptr<olc::Sprite> PlayerRight;
	std::unique_ptr<olc::Sprite> PlayerLeft;
	std::unique_ptr<olc::Sprite> PlayerDead;
	std::unique_ptr<olc::Sprite> Shadow;
	std::unique_ptr<olc::Sprite> WalkLeftSS;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;
	olc::Decal* WalkLeftSSDecal;

	olc::vf2d PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed);
	void DrawPlayer(olc::TransformedView& tv);
	void Initialize(olc::PixelGameEngine* pge);

};