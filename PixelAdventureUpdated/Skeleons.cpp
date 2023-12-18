#include "Skeletons.h"
void Skeleton::SpawnSkeleton() {
	if (SkelePos.size() < 1) {
		SkelePos.push_back({ 305.0f, 305.0f });
	}
}
void Skeleton::DrawCalculation(olc::vf2d PlayerPos) {
	SkeleAbove.clear();
	SkeleBelow.clear();
	for (int i = 0; i <= SkelePos.size(); i++) {
		//If Skele y is greater than Player y
		if (SkelePos[i].y > PlayerPos.y) {
			SkeleBelow.push_back(i);
		}
		if (SkelePos[i].y <= PlayerPos.y) {
			SkeleAbove.push_back(i);
		}
	}
}
void Skeleton::DrawBelowPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	for (int k = 0; k <= SkeleBelow.size(); k++) {
		int i = SkeleBelow[k];
		tv.DrawDecal(SkelePos[i], SkeleRightDecal, { 4.0f, 4.0f });
	}
}
void Skeleton::DrawAbovePlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	for (int k = 0; k <= SkeleAbove.size(); k++) {
		int i = SkeleAbove[k];
		tv.DrawDecal(SkelePos[i], SkeleRightDecal, { 4.0f, 4.0f });
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