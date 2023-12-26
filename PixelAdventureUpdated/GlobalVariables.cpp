#include "GlobalVariables.h"

GlobalVars::PlayerStateEnum GlobalVars::PlayerState = IDLE_LEFT;
std::vector<GlobalVars::GameStateEnum> GlobalVars::GameState;
float GlobalVars::maxDistance = 1.8f;
float GlobalVars::maxAngle = M_PI / 2.5f;
float GlobalVars::maxDistanceSquared = GlobalVars::maxDistance * GlobalVars::maxDistance;
