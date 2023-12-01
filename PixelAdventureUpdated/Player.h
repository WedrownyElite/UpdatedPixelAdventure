#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

class Player {
public:

	olc::vf2d PlayerPos = { 300.0f, 300.0f };
	bool WalkingLeft = false;
	bool WalkingRight = false;
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

	olcPGEX_Animator2D animator;

	void DrawPlayer(olc::TransformedView& tv);
	void Initialize(olc::PixelGameEngine* pge);

};