#include "GlobalVariables.h"

std::vector<GlobalVars::GameStateEnum> GlobalVars::GameState;
float GlobalVars::maxDistance = 2.5f;
float GlobalVars::maxAngle = M_PI / 2.5f;
float GlobalVars::maxDistanceSquared = GlobalVars::maxDistance * GlobalVars::maxDistance;
