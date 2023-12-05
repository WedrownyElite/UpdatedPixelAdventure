#include "Player.h"
#include "olcPGEX_Animator2D.h"

olcPGEX_Animator2D animator;

olc::vf2d Player::PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed) {
	if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld) {
		PlayerPos.y -= PlayerSpeed;
	}
	if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld) {
		PlayerPos.y += PlayerSpeed;
	}
	if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld) {
		PlayerPos.x -= PlayerSpeed;
		Player::Dir = false;
		WalkingLeft = true;
		WalkingRight = false;
	}
	if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) {
		PlayerPos.x += PlayerSpeed;
		Player::Dir = true;
		WalkingLeft = false;
		WalkingRight = true;
	}

	return PlayerPos;
}
void Player::DrawPlayer(olc::TransformedView& tv) {
	if (Player::Dir == false) {
		tv.DrawDecal({ PlayerPos.x - 1.5f, PlayerPos.y - 1.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
		tv.DrawRectDecal(PlayerPos, { 1.0f, 2.0f }, olc::WHITE);
	}
	if (Player::Dir == true) {
		tv.DrawDecal({ PlayerPos.x - 1.4f, PlayerPos.y - 1.0f }, PlayerRightDecal, { 4.0f, 4.0f });
		tv.DrawRectDecal(PlayerPos, { 1.0f, 2.0f }, olc::WHITE);
	}
	else {
		tv.DrawDecal({ PlayerPos.x - 1.5f, PlayerPos.y - 1.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
		tv.DrawRectDecal(PlayerPos, { 1.0f, 2.0f }, olc::WHITE);
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