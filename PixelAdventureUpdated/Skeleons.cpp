#include "Skeletons.h"

#include "MathFunctions.h"
#include "GlobalVariables.h"

MathFunctions MF;
Skeletons Skele;

void SkeletonFunctions::IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, std::vector<Skeletons>& Skeles, bool& PlayerAttacked, olc::vf2d PlayerPos) {
	if (PlayerAttacked == true) {
		for (int k = 0; k < Skeles.size(); k++) {
			olc::vf2d MousePos = { MF.GetWorldMousePos(tv, pge) };
			olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
			float angleTowards = MF.PointTo(PlayerPos, Skeles[k].SkelePos);
			float angleDiff = MF.angleDifference(PlayerDir.polar().y, angleTowards);
			if (
				(sqrt(pow(PlayerPos.x - Skeles[k].SkelePos.x, 2) + pow(PlayerPos.y - Skeles[k].SkelePos.y, 2)) < GlobalVars::maxDistance //Check to see if the target is in range (distance formula)
					&& abs(angleDiff) < GlobalVars::maxAngle)  //See if the target's angle is within the sweeping arc range.
				) {
				Skeles[k].SkeleHit = 1;
			}
			else {
				PlayerAttacked = false;
			}
		}
	}
}
void SkeletonFunctions::Collision(olc::PixelGameEngine* pge, std::vector<Skeletons>& Skeles, olc::vf2d PlayerPos, float PlayerSpeed) {
	for (int k = 0; k < Skeles.size(); k++) {
		olc::vf2d Skele(Skeles[k].SkelePos.x - 0.5f, Skeles[k].SkelePos.y - 1.0f);
		olc::vf2d UPlayerPos(PlayerPos.x - 0.5f, PlayerPos.y - 1.0f);
		olc::vf2d PlayerSize(0.7f, 1.2f);
		olc::vf2d SkeleSize(0.8f, 1.2f);

		//Collision
		if (Skele.x < UPlayerPos.x + PlayerSize.x
			&& Skele.x + SkeleSize.x > UPlayerPos.x
			&& Skele.y < UPlayerPos.y + PlayerSize.y
			&& Skele.y + SkeleSize.y > UPlayerPos.y) {
			//Direction of player
			olc::vf2d dir = (PlayerPos - Skeles[k].SkelePos).norm();
			Skeles[k].SkelePos += -dir * PlayerSpeed;
		}
	}
}
void SkeletonFunctions::SpawnSkeleton(std::vector<Skeletons>& Skeles) {
	for (int k = 0; (k < 1) && (k <= Skeles.size()); k++) {
		Skeletons skeleton;

		skeleton.SkelePos = { 305.0f, 305.0f };
		skeleton.SkeleHit = 0;
		Skeles.push_back(skeleton);
	}
}
void SkeletonFunctions::DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, std::vector<Skeletons> Skeles) {
	SkeleAbove.clear();
	SkeleBelow.clear();
	for (int i = 0; i < Skeles.size(); i++) {
		Collision(pge, Skeles, PlayerPos, PlayerSpeed);
		//If Skele y is greater than Player y
		if (Skeles[i].SkelePos.y < PlayerPos.y) {
			SkeleBelow.push_back(i);
		}
		if (Skeles[i].SkelePos.y >= PlayerPos.y) {
			SkeleAbove.push_back(i);
		}
	}
}
void SkeletonFunctions::DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles) {
	for (int k = 0; k < SkeleBelow.size(); k++) {
		int i = SkeleBelow[k];
		tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, {2.0f, 2.0f});
		//Draw central skele rectangle
		tv.DrawRectDecal(Skeles[i].SkelePos, { 0.25f, 0.25f }, olc::RED);
		//Draw skele hitbox
		tv.DrawRectDecal({ Skeles[i].SkelePos.x - 0.5f, Skeles[i].SkelePos.y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
	}
}
void SkeletonFunctions::DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles) {
	for (int k = 0; k < SkeleAbove.size(); k++) {
		int i = SkeleAbove[k];
		tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, {2.0f, 2.0f});
		//Draw central skele rectangle
		tv.DrawRectDecal(Skeles[i].SkelePos, { 0.25f, 0.25f }, olc::RED);
		//Draw skele hitbox
		tv.DrawRectDecal({ Skeles[i].SkelePos.x - 0.5f, Skeles[i].SkelePos.y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
	}
}
void SkeletonFunctions::Initialize() {
	//Sprites
	SkeleRight = std::make_unique<olc::Sprite>("./Sprites/SkeletonRight.png");
	SkeleRightHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonRightHurt.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	//Decals
	SkeleRightDecal = new olc::Decal(SkeleRight.get());
	SkeleRightHurtDecal = new olc::Decal(SkeleRightHurt.get());
	ShadowDecal = new olc::Decal(Shadow.get());
}