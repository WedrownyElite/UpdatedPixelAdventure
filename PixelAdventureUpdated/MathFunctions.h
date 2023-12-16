#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class MathFunctions {
public:
	olc::vf2d GetPlayerPos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos);
};