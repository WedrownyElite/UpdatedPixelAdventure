#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#include "MathFunctions.h"

class Skeleton {
private:
	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;
public:

};
