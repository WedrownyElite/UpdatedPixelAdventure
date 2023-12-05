#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

#include <iostream>

#include "Player.h"
#include "GlobalVars.h"
Player P;
GlobalVars GV;

class Pixel_Adventure : public olc::PixelGameEngine {
public:

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

	float PlayerSpeed;

	olc::vf2d MousePos = { 300, 300 };

	olc::vf2d PlayerPos = { 300, 300 };
	//Sprites
	std::unique_ptr<olc::Sprite> Grass;
	//Decals
	olc::Decal* GrassDecal;

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}

	olc::vf2d MousePosFunc() {
		MousePos = GetMousePos() / 32 + tv.GetWorldOffset();
		return MousePos;
	}
	void DebugVariables(olc::vf2d PlayerPos) {
		std::string MousePosXString = std::to_string(MousePos.x);
		std::string MousePosYString = std::to_string(MousePos.y);

		DrawStringDecal({ 10.0f, 10.0f }, MousePosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 250.0f, 10.0f }, MousePosYString, olc::WHITE, { 2.0f, 2.0f });

		std::string PlayerPosXString = std::to_string(PlayerPos.x);
		std::string PlayerPosYString = std::to_string(PlayerPos.y);

		DrawStringDecal({ 10.0f, 80.0f }, PlayerPosXString, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 250.0f, 80.0f }, PlayerPosYString, olc::WHITE, { 2.0f, 2.0f });
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
		PlayerSpeed = 6.0f * fElapsedTime;

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		//Player input
		PlayerPos = P.PlayerInput(this, PlayerSpeed);
		//Draw background
		DrawBGCamera();
		//Update mouse pos (tv offset)
		MousePos = MousePosFunc();
		//Draw Player
		P.DrawPlayer(tv);
		//Draw debug variables
		DebugVariables(PlayerPos);
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

		P.Initialize(this);
		return true;
	}
};

int main() {
	Pixel_Adventure demo;
	if (demo.Construct(1024, 576, 1, 1, true, true))
		demo.Start();
}