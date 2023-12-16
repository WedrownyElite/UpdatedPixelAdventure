#include "MathFunctions.h"

olc::vf2d MathFunctions::GetPlayerPos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos) {
	return PlayerPos / 32 + tv.GetWorldOffset();
}