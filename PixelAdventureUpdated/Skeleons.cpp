#include "Skeletons.h"

#include "MathFunctions.h"
#include "GlobalVariables.h"

MathFunctions MF;

bool Skeleton::IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, bool PlayerAttacked, olc::vf2d PlayerPos) {
	if (PlayerAttacked == true) {
		for (int k = 0; k < SkelePos.size(); k++) {
			olc::vf2d MousePos = { MF.GetWorldMousePos(tv, pge) };
			olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
			float angleTowards = MF.PointTo(PlayerPos, SkelePos[k]);
			float angleDiff = MF.angleDifference(PlayerDir.polar().y, angleTowards);
			if (
				(sqrt(pow(PlayerPos.x - SkelePos[k].x, 2) + pow(PlayerPos.y - SkelePos[k].y, 2)) < GlobalVars::maxDistance //Check to see if the target is in range (distance formula)
					&& abs(angleDiff) < GlobalVars::maxAngle)  //See if the target's angle is within the sweeping arc range.
				) {
				SkeleHit[k] = 1;
			}
			else {
				PlayerAttacked = false;
			}
		}
	}
	return PlayerAttacked;
}
olc::vf2d Skeleton::Collision(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed) {
	for (int k = 0; k < SkelePos.size(); k++) {
		olc::vf2d Skele(SkelePos[k].x - 0.5f, SkelePos[k].y - 1.0f);
		olc::vf2d UPlayerPos(PlayerPos.x - 0.5f, PlayerPos.y - 1.0f);
		olc::vf2d PlayerSize(0.7f, 1.2f);
		olc::vf2d SkeleSize(0.8f, 1.2f);

		//Collision
		if (Skele.x < UPlayerPos.x + PlayerSize.x
			&& Skele.x + SkeleSize.x > UPlayerPos.x
			&& Skele.y < UPlayerPos.y + PlayerSize.y
			&& Skele.y + SkeleSize.y > UPlayerPos.y) {
			//Direction of player
			olc::vf2d dir = (PlayerPos - SkelePos[k]).norm();
			SkelePos[k] += -dir * PlayerSpeed;
		}
		return SkelePos[k];
	}
}
void Skeleton::SpawnSkeleton() {
	if (SkelePos.size() < 1) {
		SkeleHit.push_back(0);
		SkelePos.push_back({ 305.0f, 305.0f });
	}
}
void Skeleton::DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed) {
	SkeleAbove.clear();
	SkeleBelow.clear();
	for (int i = 0; i < SkelePos.size(); i++) {
		SkelePos[i] = Collision(pge, PlayerPos, PlayerSpeed);
		//If Skele y is greater than Player y
		if (SkelePos[i].y < PlayerPos.y) {
			SkeleBelow.push_back(i);
		}
		if (SkelePos[i].y >= PlayerPos.y) {
			SkeleAbove.push_back(i);
		}
	}
}
void Skeleton::DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	for (int k = 0; k < SkeleBelow.size(); k++) {
		int i = SkeleBelow[k];
		tv.DrawDecal({ SkelePos[i].x - 1.0f, SkelePos[i].y - 1.0f }, SkeleRightDecal, {2.0f, 2.0f});
		//Draw central skele rectangle
		tv.DrawRectDecal(SkelePos[i], { 0.25f, 0.25f }, olc::RED);
		//Draw skele hitbox
		tv.DrawRectDecal({ SkelePos[i].x - 0.5f, SkelePos[i].y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
	}
}
void Skeleton::DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	for (int k = 0; k < SkeleAbove.size(); k++) {
		int i = SkeleAbove[k];
		tv.DrawDecal({ SkelePos[i].x - 1.0f, SkelePos[i].y - 1.0f }, SkeleRightDecal, {2.0f, 2.0f});
		//Draw central skele rectangle
		tv.DrawRectDecal(SkelePos[i], { 0.25f, 0.25f }, olc::RED);
		//Draw skele hitbox
		tv.DrawRectDecal({ SkelePos[i].x - 0.5f, SkelePos[i].y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
	}
}
void Skeleton::Initialize() {
	//Sprites
	SkeleRight = std::make_unique<olc::Sprite>("./Sprites/SkeletonRight.png");
	SkeleRightHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonRightHurt.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	//Decals
	SkeleRightDecal = new olc::Decal(SkeleRight.get());
	SkeleRightHurtDecal = new olc::Decal(SkeleRightHurt.get());
	ShadowDecal = new olc::Decal(Shadow.get());
}