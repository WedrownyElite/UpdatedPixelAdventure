#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

#include <iostream>
#include <random>

#include "Player.h"
#include "Skeletons.h"
#include "MathFunctions.h"
#include "GlobalVariables.h"

MathFunctions MathFuncs;
SkeletonFunctions SkeleFunctions;
Player P;

class Pixel_Adventure : public olc::PixelGameEngine {
public:
	bool DebugScreen = false;
	//Camera variables
	// Transformed view object to make world offsetting simple
	olc::TileTransformedView tv;
	// Conveninet constants to define tile map world
	olc::vi2d m_vWorldSize = { 1000, 1000 };
	olc::vi2d m_vTileSize = { 32, 32 };
	// The camera!
	olc::utils::Camera2D camera;
	// The world map, stored as a 1D array
	std::vector<uint8_t> vWorldMap;

	int rand_num;

	float KnockbackSpeed;
	float PlayerSpeed;
	bool PlayerAttacked = false;
	bool PlayerWalking = false;
	bool AttackAnim = false;

	int o = 0;
	std::vector<Skeletons> Skeles;

	olc::vf2d MousePos = { 300, 300 };

	olc::vf2d PlayerPos = { 300, 300 };
	//Sprites
	std::unique_ptr<olc::Sprite> Grass1;
	std::unique_ptr<olc::Sprite> Grass2;
	std::unique_ptr<olc::Sprite> Grass3;
	std::unique_ptr<olc::Sprite> Grass4;
	//Decals
	olc::Decal* Grass1Decal;
	olc::Decal* Grass2Decal;
	olc::Decal* Grass3Decal;
	olc::Decal* Grass4Decal;

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}
	void DebugVariables(olc::vf2d PlayerPos) {
		//FPS
		std::string FPSString = std::to_string(GetFPS());

		DrawStringDecal({ 10.0f, 10.0f }, "FPS:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 80.0f, 10.0f }, FPSString, olc::WHITE, { 2.0f, 2.0f });

		//MousePos (tv offset)
		std::string MousePosXString = std::to_string(MousePos.x);
		std::string MousePosYString = std::to_string(MousePos.y);

		DrawStringDecal({ 10.0f, 30.0f }, "MousePos:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 30.0f }, MousePosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 30.0f }, MousePosYString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerPos
		std::string PlayerPosXString = std::to_string(PlayerPos.x);
		std::string PlayerPosYString = std::to_string(PlayerPos.y);

		DrawStringDecal({ 10.0f, 50.0f }, "PlayerPos:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 50.0f }, PlayerPosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 50.0f }, PlayerPosYString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerSpeed
		std::string PlayerSpeedString = std::to_string(PlayerSpeed);

		DrawStringDecal({ 10.0f, 70.0f }, "PlayerSpeed:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 220.0f, 70.0f }, PlayerSpeedString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerAttacked
		std::string PlayerAttackedString = std::to_string(PlayerAttacked);

		DrawStringDecal({ 10.0f, 90.0f }, "PlayerAttacked", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 250.0f, 90.0f }, PlayerAttackedString, olc::WHITE, { 2.0f, 2.0f });

		//Player dir
		olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
		std::string PlayerDirX = std::to_string(PlayerDir.x);
		std::string PlayerDirY = std::to_string(PlayerDir.y);
		DrawStringDecal({ 10.0f, 110.0f }, "PlayerDir:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 180.0f, 110.0f }, PlayerDirX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 110.0f }, PlayerDirY, olc::WHITE, { 2.0f, 2.0f });

		//Angle towards
		float angleTowards = MathFuncs.PointTo(PlayerPos, Skeles[0].SkelePos);
		std::string angleTowardsString = std::to_string(angleTowards);

		DrawStringDecal({ 10.0f, 130.0f }, "AngleTowards:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 220.0f, 130.0f }, angleTowardsString, olc::WHITE, { 2.0f, 2.0f });

		//AngleDiff
		float angleDiff = MathFuncs.angleDifference(PlayerDir.polar().y, angleTowards);
		std::string angleDiffString = std::to_string(angleDiff);

		DrawStringDecal({ 10.0f, 150.0f }, "AngleDiff:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 180.0f, 150.0f }, angleDiffString, olc::WHITE, { 2.0f, 2.0f });

		//Direction
		olc::vf2d dir = (PlayerPos - Skeles[0].SkelePos).norm();
		std::string dirX = std::to_string(dir.x);
		std::string dirY = std::to_string(dir.y);

		DrawStringDecal({ 10.0f, 170.0f }, "Direction:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 170.0f }, dirX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 320.0f, 170.0f }, dirY, olc::WHITE, { 2.0f, 2.0f });

		//Distance
		float dist = sqrt(pow(PlayerPos.x - Skeles[0].SkelePos.x, 2) + pow(PlayerPos.y - Skeles[0].SkelePos.y, 2));
		std::string distString = std::to_string(dist);
		DrawStringDecal({ 10.0f, 190.0f }, "Less than 1.8f?", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 280.0f, 190.0f }, distString, olc::WHITE, { 2.0f, 2.0f });

		//Angle
		float Angle = abs(angleDiff);
		std::string AngleString = std::to_string(Angle);
		std::string MaxAngleString = std::to_string(GlobalVars::maxAngle);
		
		DrawStringDecal({ 10.0f, 210.0f }, "Less than", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 160.0f, 210.0f }, MaxAngleString, olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 285.0f, 210.0f }, "f?", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 340.0f, 210.0f }, AngleString, olc::WHITE, { 2.0f, 2.0f });

		//RandoNum

		std::string RandNumString = std::to_string(rand_num);

		DrawStringDecal({ 10.0f, 250.0f }, RandNumString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerState
		std::string PlayerStateString = std::to_string(GlobalVars::PlayerState);
		std::string PlayerStateStringUpdated;
		if (PlayerStateString == "0") {
			PlayerStateStringUpdated = "Walking left";
		}
		if (PlayerStateString == "1") {
			PlayerStateStringUpdated = "Walking right";
		}
		if (PlayerStateString == "2") {
			PlayerStateStringUpdated = "Idle left";
		}
		if (PlayerStateString == "3") {
			PlayerStateStringUpdated = "Idle right";
		}
		if (PlayerStateString == "4") {
			PlayerStateStringUpdated = "Attack left";
		}
		if (PlayerStateString == "5") {
			PlayerStateStringUpdated = "Attack right";
		}
		DrawStringDecal({ 10.0f, 230.0f }, "PlayerState:", olc::BLUE, { 2.0f, 2.0f });
		DrawStringDecal({ 210.0f, 230.0f }, PlayerStateStringUpdated, olc::WHITE, { 2.0f, 2.0f });
	}
	void DrawBGCamera() {
		// Render "tile map", by getting visible tiles
		olc::vi2d vTileTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = tv.GetBottomRightTile().min(m_vWorldSize);
		olc::vi2d vTile;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(0, 256);
		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				int nSeed = vTile.y << 16 | vTile.x;
				//mt.seed(nSeed);
				uint64_t nLehmer = nSeed;
				uint32_t rand_num = MathFuncs.Lehmer32(nLehmer) % 256;
				//int idx = vTile.y * m_vWorldSize.x + vTile.x;
				if (rand_num <= 120) {
					tv.DrawDecal(vTile, Grass1Decal, { 1.0f, 1.0f });
					//tv.DrawDecal(vTile, Grass2Decal, { 1.0f, 1.0f });
				}

				if (rand_num > 120 && rand_num <= 253) {
					//tv.DrawDecal(vTile, Grass1Decal, { 1.0f, 1.0f });
					tv.DrawDecal(vTile, Grass2Decal, { 1.0f, 1.0f });
				}
				if (rand_num == 254) {
					tv.DrawDecal(vTile, Grass4Decal, { 1.0f, 1.0f });
				}
				if (rand_num == 255) {
					tv.DrawDecal(vTile, Grass3Decal, { 1.0f, 1.0f });
				}
			}
	}
	bool OnUserUpdate(float fElapsedTime) override {
		KnockbackSpeed = 12.0f * fElapsedTime;
		//Attacking
		if (P.animator.GetAnim("Attack_Left")->bIsPlaying || P.animator.GetAnim("Attack_Right")->bIsPlaying) {
			PlayerSpeed = 3.0f * fElapsedTime;
		}
		//Sprinting
		if (GetKey(olc::Key::SHIFT).bHeld) {
			PlayerSpeed = 10.0f * fElapsedTime;
		}
		//Walking
		else {
			PlayerSpeed = 6.0f * fElapsedTime;
		}

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		//Update mouse pos (tv offset)
		MousePos = MathFuncs.GetWorldMousePos(tv, this);

		//Player input
		PlayerPos = P.PlayerInput(this, PlayerSpeed, PlayerWalking);
		P.AttackInput(this, fElapsedTime, PlayerAttacked, MousePos, AttackAnim);
		//Draw background
		DrawBGCamera();
		//Enemy functions
		SkeleFunctions.Functions(tv, this, Skeles, KnockbackSpeed, PlayerSpeed, fElapsedTime, PlayerPos, PlayerAttacked);
		//Draw skeletons below player
		SkeleFunctions.DrawBelowPlayer(tv, this, Skeles, DebugScreen);
		//Draw Player
		P.DrawPlayer(tv, this, fElapsedTime, PlayerWalking, AttackAnim);
		//Draw skeletons above player
		SkeleFunctions.DrawAbovePlayer(tv, this, Skeles, DebugScreen);
		//Draw debug variables
		if (DebugScreen == true) {
			//Draw central player rectangle
			tv.DrawRectDecal(PlayerPos, { 0.25f, 0.25f }, olc::WHITE);
			//Draw player hitbox
			tv.DrawRectDecal({ PlayerPos.x - 0.5f, PlayerPos.y - 1.0f }, { 1.0f, 2.0f }, olc::RED);

			DebugVariables(PlayerPos);
		}

		//General inputs
		//Escape (EXIT)
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			return false;
		}
		//Toggle DebugScreen
		if (GetKey(olc::Key::F1).bPressed) DebugScreen = !DebugScreen;

		return true;
	}
private:
	bool OnUserCreate() override {
		//Sprites
		Grass1 = std::make_unique<olc::Sprite>("./Sprites/Grass1.png");
		Grass2 = std::make_unique<olc::Sprite>("./Sprites/Grass2.png");
		Grass3 = std::make_unique<olc::Sprite>("./Sprites/Grass3.png");
		Grass4 = std::make_unique<olc::Sprite>("./Sprites/Grass4.png");
		//Decals
		Grass1Decal = new olc::Decal(Grass1.get());
		Grass2Decal = new olc::Decal(Grass2.get());
		Grass3Decal = new olc::Decal(Grass3.get());
		Grass4Decal = new olc::Decal(Grass4.get());

		// Construct transform view
		tv = olc::TileTransformedView(GetScreenSize(), m_vTileSize);

		// Construct Camera
		camera = olc::utils::Camera2D(GetScreenSize() / m_vTileSize, PlayerPos);

		// Configure Camera
		camera.SetTarget(PlayerPos);
		camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
		camera.SetWorldBoundary({ 0.0f, 0.0f }, m_vWorldSize);
		camera.EnableWorldBoundary(true);

		// Create "tile map" world with just two tiles
		vWorldMap.resize(m_vWorldSize.x * m_vWorldSize.y);
		for (int i = 0; i < vWorldMap.size(); i++)
			vWorldMap[i] = ((rand() % 20) == 1) ? 1 : 0;

		//Initialize needed files
		SkeleFunctions.Initialize();
		P.Initialize();
		return true;
	}
};

int main() {
	Pixel_Adventure demo;
	if (demo.Construct(1024, 576, 1, 1, true, true))
		demo.Start();
}