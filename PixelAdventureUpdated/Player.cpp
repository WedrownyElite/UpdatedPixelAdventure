#include "Player.h"
#include "olcPGEX_Animator2D.h"

olcPGEX_Animator2D animator;

bool Player::MovingCheck(olc::PixelGameEngine* pge) {
	if ((!pge->GetKey(olc::Key::D).bHeld || !pge->GetKey(olc::Key::LEFT).bHeld)
		&& (!pge->GetKey(olc::Key::A).bHeld || !pge->GetKey(olc::Key::RIGHT).bHeld)
		&& (!pge->GetKey(olc::Key::W).bHeld || !pge->GetKey(olc::Key::UP).bHeld)
		&& (!pge->GetKey(olc::Key::S).bHeld || !pge->GetKey(olc::Key::DOWN).bHeld)) {
		return false;
	}
	return true;
}
olc::vf2d Player::PlayerInput(olc::PixelGameEngine* pge, float PlayerSpeed) {
	//Not moving
	if (!MovingCheck(pge)) {
		PlayerWalking = false;
	}
	//Walking up
	if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld) {
		PlayerPos.y -= PlayerSpeed;
		PlayerWalking = true;
	}
	//Walking down
	if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld) {
		PlayerPos.y += PlayerSpeed;
		PlayerWalking = true;
	}
	//Walking left
	if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld) {
		PlayerPos.x -= PlayerSpeed;
		Dir = false;
		PlayerWalking = true;
	}
	//Walking right
	if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) {
		PlayerPos.x += PlayerSpeed;
		Dir = true;
		PlayerWalking = true;
	}

	return PlayerPos;
}
void Player::DrawPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime) {
	//Facing left
	if (Dir == false && PlayerWalking == false) {
		animator.StopAll();
		tv.DrawDecal({ PlayerPos.x - 2.0f, PlayerPos.y - 2.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
	}
	//Facing right
	if (Dir == true && PlayerWalking == false) {
		animator.StopAll();
		tv.DrawDecal({ PlayerPos.x - 2.0f, PlayerPos.y - 2.0f }, PlayerRightDecal, { 4.0f, 4.0f });
	}
	//Walking left
	if (Dir == false && PlayerWalking == true) {
		if (!animator.GetAnim("Walk_Left")->bIsPlaying) {
			animator.StopAll();
		}
		animator.Play("Walk_Left");
	}
	if (PlayerWalking == true) {
		animator.UpdateAnimations(fElapsedTime);
		animator.DrawAnimationFrame(tv.WorldToScreen({ PlayerPos.x - 2.0f, PlayerPos.y - 2.0f }));
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
	animator.AddAnimation("Walk_Left", 0.25f, 6, WalkLeftSSDecal, { 0.0f, 0.0f }, { 32, 32 }, { 0.0f, 0.0f });
	animator.ScaleAnimation("Walk_Left", { 4.0f, 4.0f });
}