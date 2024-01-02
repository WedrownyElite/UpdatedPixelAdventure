#pragma once
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

class GlobalVars {
public:
	enum PlayerStateEnum { WALKING_LEFT, WALKING_RIGHT, IDLE_LEFT, IDLE_RIGHT, ATTACK_LEFT, ATTACK_RIGHT };
	static PlayerStateEnum PlayerState;

	enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
	static std::vector<GameStateEnum> GameState;

	static float maxDistance; //How far away the object can be to still be in range.
	static float maxAngle; //The total sweeping angle of the arch in either direction (PI/4 is 45 degrees in either direction, 90 degrees total).
	static float maxDistanceSquared;
};