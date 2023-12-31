#include "Skeletons.h"

#include "MathFunctions.h"
#include "GlobalVariables.h"
#include "EnemyFunctions.h"

EnemyFunctions EF;
MathFunctions MF;
Skeletons Skele;

void SkeletonFunctions::IsHit(olc::PixelGameEngine* pge, olc::TileTransformedView& tv, Skeletons& skele, bool& PlayerAttacked, olc::vf2d PlayerPos) {
	//Find distance
	float dis = MF.GetDistance(skele.SkelePos, PlayerPos);
	//Only run hit check if within 10 pixels
	if (dis <= 10.0f) {
		//Define needed variables
		olc::vf2d MousePos = { MF.GetWorldMousePos(tv, pge) };
		olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
		float angleTowards = MF.PointTo(PlayerPos, skele.SkelePos);
		float angleDiff = MF.angleDifference(PlayerDir.polar().y, angleTowards);
		olc::vf2d dir = (PlayerPos - skele.SkelePos).norm();

		//Check if sweep hits
		float dx = PlayerPos.x - skele.SkelePos.x;
		float dy = PlayerPos.y - skele.SkelePos.y;
		float distanceSquared = dx * dx + dy * dy;
		if (distanceSquared < GlobalVars::maxDistanceSquared && std::abs(angleDiff) < GlobalVars::maxAngle) {
			skele.SkeleHit = 1;
			PlayerAttacked = false;
			skele.RedSkele = 1;
			skele.Direction = dir;
		}
		else {
			PlayerAttacked = false;
		}
	}
	else {
		PlayerAttacked = false;
	}
}
void SkeletonFunctions::Knockback(Skeletons& skele, float KnockbackSpeed) {
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
		skele.KnockbackDist = 0.0f;
		skele.SkeleHit = 0;
		skele.RedSkele = 0;
		skele.RedSkeleTimer = 0;
	}
}
void SkeletonFunctions::Collision(Skeletons& skele, olc::vf2d PlayerPos, float PlayerSpeed) {
	olc::vf2d Skele(skele.SkelePos.x - 0.5f, skele.SkelePos.y - 1.0f);
	olc::vf2d UPlayerPos(PlayerPos.x - 0.5f, PlayerPos.y - 1.0f);
	olc::vf2d PlayerSize(0.7f, 1.2f);
	olc::vf2d SkeleSize(0.8f, 1.2f);

	//Collision
	if (Skele.x < UPlayerPos.x + PlayerSize.x
		&& Skele.x + SkeleSize.x > UPlayerPos.x
		&& Skele.y < UPlayerPos.y + PlayerSize.y
		&& Skele.y + SkeleSize.y > UPlayerPos.y) {
		//Direction of player
		olc::vf2d dir = (PlayerPos - skele.SkelePos).norm();
		skele.SkelePos += -dir * PlayerSpeed;
	}
}
void SkeletonFunctions::SpawnSkeleton(std::vector<Skeletons>& Skeles) {
	for (int k = 0; (k < 1) && (Skeles.size() < 1); k++) {
		Skeletons skeleton;

		skeleton.SkelePos = { 315.0f, 315.0f };
		//Knockback variables
		skeleton.SkeleHit = 0;
		skeleton.KnockbackDist = 0.0f;
		skeleton.RedSkele = 0;
		skeleton.RedSkeleTimer = 0;
		skeleton.Direction = { 0.0f, 0.0f };

		skeleton.SkeletonState = Skeletons::IDLE_LEFT;
		//"bot" variables
		skeleton.CurrentlyActive = false;
		skeleton.ActivityCooldownBool = true;
		skeleton.ActivityCooldown = 2.0f;
		skeleton.HomePos = skeleton.SkelePos;
		skeleton.WanderingGoal = skeleton.HomePos;
		skeleton.GoalDist = 0.0f;
		skeleton.WanderingDir = { 0.0f, 0.0f };
		Skeles.push_back(skeleton);
	}
}
void SkeletonFunctions::DrawCalculation(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, std::vector<Skeletons> Skeles) {
	SkeleAbove.clear();
	SkeleBelow.clear();
	for (int i = 0; i < Skeles.size(); i++) {
		Skeletons& skele = Skeles[i];
		Collision(skele, PlayerPos, PlayerSpeed);
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
			if (Skeles[i].SkeletonState == Skeletons::IDLE_RIGHT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.2f, 2.2f });
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightHurtDecal, { 2.2f, 2.2f });
			}
			if (Skeles[i].SkeletonState == Skeletons::IDLE_LEFT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftDecal, { 2.2f, 2.2f });
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftHurtDecal, { 2.2f, 2.2f });
			}
		}
		else {
			if (Skeles[i].SkeletonState == Skeletons::IDLE_LEFT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftDecal, { 2.0f, 2.0f });
			}
			if (Skeles[i].SkeletonState == Skeletons::IDLE_RIGHT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
			}

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
			if (Skeles[i].SkeletonState == Skeletons::IDLE_RIGHT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.2f, 2.2f });
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightHurtDecal, { 2.2f, 2.2f });
			}
			if (Skeles[i].SkeletonState == Skeletons::IDLE_LEFT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftDecal, { 2.2f, 2.2f });
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftHurtDecal, { 2.2f, 2.2f });
			}
		}
		else {
			if (Skeles[i].SkeletonState == Skeletons::IDLE_LEFT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.05f, Skeles[i].SkelePos.y - 1.0f }, SkeleLeftDecal, { 2.0f, 2.0f });
			}
			if (Skeles[i].SkeletonState == Skeletons::IDLE_RIGHT) {
				tv.DrawDecal({ Skeles[i].SkelePos.x - 1.0f, Skeles[i].SkelePos.y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
			}
		}
		if (DebugScreen == true) {
			//Draw central skele rectangle
			tv.DrawRectDecal(Skeles[i].SkelePos, { 0.25f, 0.25f }, olc::RED);
			//Draw skele hitbox
			tv.DrawRectDecal({ Skeles[i].SkelePos.x - 0.5f, Skeles[i].SkelePos.y - 1.0f }, { 1.2f, 2.0f }, olc::RED);
		}
	}
}
void SkeletonFunctions::Wander(Skeletons& skele, float fElapsedTime) {
	//If no activity and no cooldown, give new activity
	if (!skele.CurrentlyActive && !skele.ActivityCooldownBool) {
		//Generate new goal
		MF.GenerateNewGoal(skele.HomePos, skele.WanderingGoal, 1.5f, 2.7f);

		//Calculate wandering direction
		skele.WanderingDir = (skele.WanderingGoal - skele.SkelePos).norm();

		//Switch state depending on dir facing
		if (skele.WanderingDir.x < 0.0f && skele.WanderingDir.y <= 1.0f) {
			skele.SkeletonState = Skeletons::IDLE_LEFT;
		}
		else {
			skele.SkeletonState = Skeletons::IDLE_RIGHT;
		}

		skele.GoalDist = MF.GetDistance(skele.SkelePos, skele.WanderingGoal);
		skele.CurrentlyActive = true;
	}
	//Currently active logic
	if (skele.CurrentlyActive) {
		if (skele.GoalDist > 0.0f) {
			skele.SkelePos += skele.WanderingDir * IdleSpeed;
			skele.GoalDist -= IdleSpeed;
		}
		if (skele.GoalDist <= 0.0f) {
			float cooldown = MF.NoSeedLehmerFloatRange(2, 5);

 			skele.CurrentlyActive = false;
			skele.GoalDist = 0.0f;
			skele.ActivityCooldownBool = true;
			skele.ActivityCooldown = cooldown;
			skele.WanderingDir = { 0.0f, 0.0f };
		}
	}
	//Idle timer
	if (skele.ActivityCooldownBool == true) {
		EF.IdleActivityTimer(fElapsedTime, skele.ActivityCooldown, skele.ActivityCooldownBool);
	}
}
void SkeletonFunctions::Functions(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, std::vector<Skeletons>& Skeles, float KnockbackSpeed, float PlayerSpeed, float fElapsedTime, olc::vf2d PlayerPos, bool& PlayerAttacked) {
	IdleSpeed = 4.25f * fElapsedTime;
	SpawnSkeleton(Skeles);
	DrawCalculation(pge, PlayerPos, PlayerSpeed, Skeles);
	for (int i = 0; i < Skeles.size(); i++) {
		Skeletons& skele = Skeles[i];
		//Find distance
		float dis = MF.GetDistance(skele.SkelePos, PlayerPos);
		//Hit check
		if (PlayerAttacked) {
			IsHit(pge, tv, skele, PlayerAttacked, PlayerPos);
		}
		//Knockback
		if (skele.SkeleHit == 1) {
			Knockback(skele, KnockbackSpeed);
		}
		//Collision
		//Only run collision test if within 8 pixels
		if (dis <= 8.0f) {
			Collision(skele, PlayerPos, PlayerSpeed);
		}
		Wander(skele, fElapsedTime);
	}
}
void SkeletonFunctions::Initialize() {
	//Sprites
	SkeleRight = std::make_unique<olc::Sprite>("./Sprites/SkeletonRight.png");
	SkeleLeft = std::make_unique<olc::Sprite>("./Sprites/SkeletonLeft.png");
	SkeleRightHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonRightHurt.png");
	SkeleLeftHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonLeftHurt.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	//Decals
	SkeleRightDecal = new olc::Decal(SkeleRight.get());
	SkeleLeftDecal = new olc::Decal(SkeleLeft.get());
	SkeleRightHurtDecal = new olc::Decal(SkeleRightHurt.get());
	SkeleLeftHurtDecal = new olc::Decal(SkeleLeftHurt.get());
	ShadowDecal = new olc::Decal(Shadow.get());
}