#include "Skeletons.h"

#include "MathFunctions.h"
#include "GlobalVariables.h"

MathFunctions MF;
Skeletons Skele;

void SkeletonFunctions::IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, std::vector<Skeletons>& Skeles, bool& PlayerAttacked, olc::vf2d PlayerPos) {
	//Hit check
	for (int k = 0; k < Skeles.size(); k++) {

		//Find distance
		float dis = MF.GetDistance(Skeles[k].SkelePos, PlayerPos);
		//Only run hit check if within 10 pixels
		if (dis <= 10.0f) {
			//Define needed variables
			olc::vf2d MousePos = { MF.GetWorldMousePos(tv, pge) };
			olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
			float angleTowards = MF.PointTo(PlayerPos, Skeles[k].SkelePos);
			float angleDiff = MF.angleDifference(PlayerDir.polar().y, angleTowards);
			olc::vf2d dir = (PlayerPos - Skeles[k].SkelePos).norm();

			//Check if sweep hits
			float dx = PlayerPos.x - Skeles[k].SkelePos.x;
			float dy = PlayerPos.y - Skeles[k].SkelePos.y;
			float distanceSquared = dx * dx + dy * dy;
			if (distanceSquared < GlobalVars::maxDistanceSquared && std::abs(angleDiff) < GlobalVars::maxAngle) {
				Skeles[k].SkeleHit = 1;
				PlayerAttacked = false;
				Skeles[k].RedSkele = 1;
				Skeles[k].Direction = dir;
			}
			else {
				PlayerAttacked = false;
			}
		}
		else {
			PlayerAttacked = false;
		}
	}
}
void SkeletonFunctions::Knockback(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, std::vector<Skeletons>& Skeles, float KnockbackSpeed) {
	for (int k = 0; k < Skeles.size(); k++) {
		Skeletons& skele = Skeles[k];  // Reference to the current skeleton for brevity

		if (skele.SkeleHit == 1) {
			// Red skeleton logic
			if (skele.RedSkeleTimer <= 15) {
				skele.RedSkeleTimer++;
			}
			else {
				skele.RedSkele = 0;
				skele.RedSkeleTimer = 0;
			}

			// Knockback logic
			if (skele.KnockbackDist <= 2.0f) {
				// Apply horizontal knockback
				skele.SkelePos -= skele.Direction * KnockbackSpeed;

				// Apply upward jump
				if (skele.KnockbackDist <= 0.25f) {
					skele.SkelePos.y -= 0.25f;
				}

				skele.KnockbackDist += KnockbackSpeed;
			}
			else {
				// Apply sliding when landing
				if (skele.SkelePos.y < 0.0f) {
					skele.SkelePos.y = 0.0f;
					skele.SkelePos.x -= 0.5f;
				}

				skele.KnockbackDist = 0.0f;
				skele.SkeleHit = 0;
				skele.RedSkele = 0;
				skele.RedSkeleTimer = 0;
			}
		}
	}
}

void SkeletonFunctions::Collision(olc::PixelGameEngine* pge, std::vector<Skeletons>& Skeles, olc::vf2d PlayerPos, float PlayerSpeed) {
	for (int k = 0; k < Skeles.size(); k++) {
		//Find distance
		float dis = MF.GetDistance(Skeles[k].SkelePos, PlayerPos);
		//Only run collision test if within 8 pixels
		if (dis <= 8.0f) {

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
}
void SkeletonFunctions::SpawnSkeleton(std::vector<Skeletons>& Skeles) {
	for (int k = 0; (k < 1) && (Skeles.size() < 1); k++) {
		Skeletons skeleton;

		skeleton.SkelePos = { 315.0f, 315.0f };
		skeleton.SkeleHit = 0;
		skeleton.KnockbackDist = 0.0f;
		skeleton.RedSkele = 0;
		skeleton.RedSkeleTimer = 0;
		skeleton.Direction = { 0.0f, 0.0f };
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
void SkeletonFunctions::DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles, bool DebugScreen) {
	for (int k = 0; k < SkeleBelow.size(); k++) {
		int i = SkeleBelow[k];
		tv.DrawDecal({ Skeles[i].SkelePos.x - 0.8f, Skeles[i].SkelePos.y + 0.22f}, ShadowDecal, {1.5f, 1.5f});
		//Draw red skele if hit
		if (Skeles[i].RedSkele <= 15 && Skeles[i].RedSkele != 0) {
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.2f, 2.2f });
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightHurtDecal, { 2.2f, 2.2f });
		}
		else {
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
		}
		if (DebugScreen == true) {
			//Draw central skele rectangle
			tv.DrawRectDecal(Skeles[i].SkelePos, { 0.25f, 0.25f }, olc::RED);
			//Draw skele hitbox
			tv.DrawRectDecal({ Skeles[i].SkelePos.x - 0.5f, Skeles[i].SkelePos.y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
		}
	}
}
void SkeletonFunctions::DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons> Skeles, bool DebugScreen) {
	for (int k = 0; k < SkeleAbove.size(); k++) {
		int i = SkeleAbove[k];
		tv.DrawDecal({ Skeles[i].SkelePos.x - 0.8f, Skeles[i].SkelePos.y + 0.22f }, ShadowDecal, { 1.5f, 1.5f });
		//Draw red skele if hit
		if (Skeles[i].RedSkele <= 15 && Skeles[i].RedSkele != 0) {
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.2f, 2.2f });
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightHurtDecal, { 2.2f, 2.2f });
		}
		else {
			tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
		}
		if (DebugScreen == true) {
			//Draw central skele rectangle
			tv.DrawRectDecal(Skeles[i].SkelePos, { 0.25f, 0.25f }, olc::RED);
			//Draw skele hitbox
			tv.DrawRectDecal({ Skeles[i].SkelePos.x - 0.5f, Skeles[i].SkelePos.y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
		}
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