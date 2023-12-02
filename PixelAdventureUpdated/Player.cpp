#include "Player.h"
#include "olcPGEX_Animator2D.h"

olcPGEX_Animator2D animator;
void Player::DrawPlayer(olc::TransformedView& tv) {
	if (Player::Dir == false) {
		tv.DrawDecal(GlobalVars::PlayerPos, PlayerLeftDecal, { 4.0f, 4.0f });
	}
	if (Player::Dir == true) {
		tv.DrawDecal(GlobalVars::PlayerPos, PlayerRightDecal, { 4.0f, 4.0f });
	}
	else {
		tv.DrawDecal(GlobalVars::PlayerPos, PlayerLeftDecal, { 4.0f, 4.0f });
	}
}
void Player::Initialize(olc::PixelGameEngine* pge) {
	//Sprites
	PlayerRight = std::make_unique<olc::Sprite>("./Sprites/CharacterRightFacing.png");
	PlayerLeft = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing.png");
	PlayerDead = std::make_unique<olc::Sprite>("./Sprites/CharacterDeathPose.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	WalkLeftSS = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing-Sheet.png");
	//Decals
	PlayerRightDecal = new olc::Decal(PlayerRight.get());
	PlayerLeftDecal = new olc::Decal(PlayerLeft.get());
	PlayerDeadDecal = new olc::Decal(PlayerDead.get());
	ShadowDecal = new olc::Decal(Shadow.get());
	WalkLeftSSDecal = new olc::Decal(WalkLeftSS.get());

	//Animations
	animator.AddAnimation("Walk_Left", 0.6f, 6, WalkLeftSSDecal, { 0, 0 }, { 32, 32 }, { 0, 0 }, { 0, 0 }, true, false);
}