#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "GlobalVariables.h"

class Skeletons {
public:
	olc::vf2d SkelePos;
	int SkeleHit;
	int RedSkele;
	int RedSkeleTimer;
	float KnockbackDist;
	olc::vf2d Direction;
	enum SkeletonStateEnum { IDLE_LEFT, IDLE_RIGHT, IDLE_WALK_LEFT, IDLE_WALK_RIGHT, WALK_LEFT, WALK_RIGHT, ATTACK_LEFT, ATTACK_RIGHT };
	SkeletonStateEnum SkeletonState;
	bool CurrentlyActive;
	bool ActivityCooldownBool;
	olc::vf2d HomePos;
	olc::vf2d WanderingGoal;
	float GoalDist;
	olc::vf2d WanderingDir;

	float ActivityCooldown;
};
class SkeletonFunctions {
private:
	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleLeft;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> SkeleLeftHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleLeftDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* SkeleLeftHurtDecal;
	olc::Decal* ShadowDecal;

	std::vector<int> SkeleBelow;
	std::vector<int> SkeleAbove;

	float IdleSpeed;
public:

	void IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, Skeletons& skele, bool& PlayerAttacked, olc::vf2d PlayerPos);
	void Knockback(Skeletons& skele, float KnockbackSpeed);
	void Collision(Skeletons& skele, olc::vf2d PlayerPos, float PlayerSpeed);
	void SpawnSkeleton(std::vector<Skeletons>& Skeles);
	void DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, std::vector<Skeletons> Skeles);
	void DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles, bool DebugScreen);
	void DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles, bool DebugScreen);
	void Wander(Skeletons& skele, float fElapsedTime);
	void Functions(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons>& Skeles, float KnockbackSpeed, float PlayerSpeed, float fElapsedTime, olc::vf2d PlayerPos, bool& PlayerAttacked);
	void Initialize();
};
