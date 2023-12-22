#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

#include <iostream>

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

	float KnockbackSpeed;
	float PlayerSpeed;
	bool PlayerAttacked = false;
	std::vector<Skeletons> Skeles;

	olc::vf2d MousePos = { 300, 300 };

	olc::vf2d PlayerPos = { 300, 300 };
	//Sprites
	std::unique_ptr<olc::Sprite> Grass;
	//Decals
	olc::Decal* GrassDecal;

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}

	void DebugVariables(olc::vf2d PlayerPos) {
		//MousePos (tv offset)
		std::string MousePosXString = std::to_string(MousePos.x);
		std::string MousePosYString = std::to_string(MousePos.y);

		DrawStringDecal({ 10.0f, 10.0f }, "MousePos:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 10.0f }, MousePosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 10.0f }, MousePosYString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerPos
		std::string PlayerPosXString = std::to_string(PlayerPos.x);
		std::string PlayerPosYString = std::to_string(PlayerPos.y);

		DrawStringDecal({ 10.0f, 40.0f }, "PlayerPos:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 40.0f }, PlayerPosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 40.0f }, PlayerPosYString, olc::WHITE, { 2.0f, 2.0f });

		//FPS
		std::string FPSString = std::to_string(GetFPS());

		DrawStringDecal({ 10.0f, 80.0f }, "FPS:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 80.0f, 80.0f }, FPSString, olc::WHITE, { 2.0f, 2.0f });

		//PlayerAttacked
		std::string PlayerAttackedString = std::to_string(PlayerAttacked);

		DrawStringDecal({ 10.0f, 120.0f }, "PlayerAttacked", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 250.0f, 120.0f }, PlayerAttackedString, olc::WHITE, { 2.0f, 2.0f });

		//Player dir
		olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
		std::string PlayerDirX = std::to_string(PlayerDir.x);
		std::string PlayerDirY = std::to_string(PlayerDir.y);
		DrawStringDecal({ 10.0f, 140.0f }, "PlayerDir:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 180.0f, 140.0f }, PlayerDirX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 350.0f, 140.0f }, PlayerDirY, olc::WHITE, { 2.0f, 2.0f });

		//Angle towards
		float angleTowards = MathFuncs.PointTo(PlayerPos, Skeles[0].SkelePos);
		std::string angleTowardsString = std::to_string(angleTowards);

		DrawStringDecal({ 10.0f, 160.0f }, "AngleTowards:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 220.0f, 160.0f }, angleTowardsString, olc::WHITE, { 2.0f, 2.0f });

		//AngleDiff
		float angleDiff = MathFuncs.angleDifference(PlayerDir.polar().y, angleTowards);
		std::string angleDiffString = std::to_string(angleDiff);

		DrawStringDecal({ 10.0f, 180.0f }, "AngleDiff:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 180.0f, 180.0f }, angleDiffString, olc::WHITE, { 2.0f, 2.0f });

		//Direction
		olc::vf2d dir = (PlayerPos - Skeles[0].SkelePos).norm();
		std::string dirX = std::to_string(dir.x);
		std::string dirY = std::to_string(dir.y);

		DrawStringDecal({ 10.0f, 200.0f }, "Direction:", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 170.0f, 200.0f }, dirX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 320.0f, 200.0f }, dirY, olc::WHITE, { 2.0f, 2.0f });

		//Distance
		float dist = sqrt(pow(PlayerPos.x - Skeles[0].SkelePos.x, 2) + pow(PlayerPos.y - Skeles[0].SkelePos.y, 2));
		std::string distString = std::to_string(dist);
		DrawStringDecal({ 10.0f, 220.0f }, "Less than 2.0f?", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 280.0f, 220.0f }, distString, olc::WHITE, { 2.0f, 2.0f });

		//Angle
		float Angle = abs(angleDiff);
		std::string AngleString = std::to_string(Angle);
		std::string MaxAngleString = std::to_string(GlobalVars::maxAngle);
		
		DrawStringDecal({ 10.0f, 240.0f }, "Less than", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 180.0f, 240.0f }, MaxAngleString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 340.0f, 240.0f }, AngleString, olc::WHITE, { 2.0f, 2.0f });
	}
	void DrawBGCamera() {
		// Render "tile map", by getting visible tiles
		olc::vi2d vTileTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = tv.GetBottomRightTile().min(m_vWorldSize);
		olc::vi2d vTile;

		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				int idx = vTile.y * m_vWorldSize.x + vTile.x;

				if (vWorldMap[idx] == 0)
					tv.DrawDecal(vTile, GrassDecal, { 1.0f, 1.0f });

				if (vWorldMap[idx] == 1)
					tv.DrawDecal(vTile, GrassDecal, { 1.0f, 1.0f });
			}
	}
	bool OnUserUpdate(float fElapsedTime) override {
		KnockbackSpeed = 12.0f * fElapsedTime;
		PlayerSpeed = 6.0f * fElapsedTime;

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		//Player input
		PlayerPos = P.PlayerInput(this, PlayerSpeed);
		PlayerAttacked = P.AttackInput(this, fElapsedTime, PlayerAttacked);
		//Draw background
		DrawBGCamera();
		//Enemy functions
		SkeleFunctions.SpawnSkeleton(Skeles);
		SkeleFunctions.Knockback(this, tv, Skeles, KnockbackSpeed);
		SkeleFunctions.Collision(this, Skeles, PlayerPos, PlayerSpeed);
		if (PlayerAttacked == true) {
			SkeleFunctions.IsHit(this, tv, Skeles, PlayerAttacked, PlayerPos);
		}
		
		SkeleFunctions.DrawCalculation(this, PlayerPos, PlayerSpeed, Skeles);
		//Update mouse pos (tv offset)
		MousePos = MathFuncs.GetWorldMousePos(tv, this);
		//Draw skeletons below player
		SkeleFunctions.DrawBelowPlayer(tv, this, Skeles);
		//Draw Player
		P.DrawPlayer(tv, this, fElapsedTime);
		//Draw skeletons above player
		SkeleFunctions.DrawAbovePlayer(tv, this, Skeles);
		//Draw central player rectangle
		tv.DrawRectDecal(PlayerPos, { 0.25f, 0.25f }, olc::WHITE);
		//Draw player hitbox
		tv.DrawRectDecal({ PlayerPos.x - 0.5f, PlayerPos.y - 1.0f}, { 1.0f, 2.0f }, olc::RED);
		//Draw debug variables
		if (DebugScreen == true) {
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
		Grass = std::make_unique<olc::Sprite>("./Sprites/Grass.png");
		//Decals
		GrassDecal = new olc::Decal(Grass.get());

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