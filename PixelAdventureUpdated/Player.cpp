#include "Player.h"
#include "olcPGEX_Animator2D.h"

olcPGEX_Animator2D animator;

bool Player::AttackInput(olc::PixelGameEngine* pge, float fElapsedTime, bool PlayerAttacked, olc::vf2d MousePos) {
	//Attack input
	if (CanAttack == true && pge->GetMouse(0).bPressed) {
		olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
		//Mouse click left
		AttackAnim = true;
		if (PlayerDir.x < 0.0f && PlayerDir.y <= 1.0f) {
			Dir = false;
		}
		else {
			Dir = true;
		}
		CanAttack = false;
		PlayerAttacked = true;
	}
	//Attack cooldown
	if (CanAttack == false) {
		if (AttackCooldown < 1.0f) {
			AttackCooldown += fElapsedTime;
		}
		if (AttackCooldown >= 1.0f) {
			AttackCooldown = 0.0f;
			CanAttack = true;
		}
	}

	return PlayerAttacked;
}
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
	MovementDirection = { 0, 0 };
	//Not moving
	if (!MovingCheck(pge)) {
		PlayerWalking = false;
	}
	//Walking up
	if (pge->GetKey(olc::Key::W).bHeld || pge->GetKey(olc::Key::UP).bHeld) {
		MovementDirection.y = -1;
		PlayerWalking = true;
	}
	//Walking down
	if (pge->GetKey(olc::Key::S).bHeld || pge->GetKey(olc::Key::DOWN).bHeld) {
		MovementDirection.y = 1;
		PlayerWalking = true;
	}
	//Walking left
	if (pge->GetKey(olc::Key::A).bHeld || pge->GetKey(olc::Key::LEFT).bHeld) {
		MovementDirection.x = -1;
		Dir = false;
		PlayerWalking = true;
	}
	//Walking right
	if (pge->GetKey(olc::Key::D).bHeld || pge->GetKey(olc::Key::RIGHT).bHeld) {
		MovementDirection.x = 1;
		Dir = true;
		PlayerWalking = true;
	}
	if (PlayerWalking == true) {
		MovementDirection = MovementDirection.norm();
	}
	PlayerPos += PlayerSpeed * MovementDirection;
	return PlayerPos;
}
void Player::DrawPlayer(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime) {
	tv.DrawDecal({ PlayerPos.x - 0.7f, PlayerPos.y + 0.22f }, ShadowDecal, { 1.5f, 1.5f });
	//Facing Left
	if (Dir == false && PlayerWalking == false && AttackAnim == false) {
		animator.StopAll();
		tv.DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
	}
	//Facing Right
	if (Dir == true && PlayerWalking == false && AttackAnim == false) {
		animator.StopAll();
		tv.DrawDecal({ PlayerPos.x - 1.80f, PlayerPos.y - 2.0f }, PlayerRightDecal, { 4.0f, 4.0f });
	}
	//Walking Left
	if (Dir == false && PlayerWalking == true && AttackAnim == false) {
		if (!animator.GetAnim("Walk_Left")->bIsPlaying) {
			animator.StopAll();
			animator.Play("Walk_Left");
		}
	}
	//Walking Right
	if (Dir == true && PlayerWalking == true && AttackAnim == false) {
		if (!animator.GetAnim("Walk_Right")->bIsPlaying) {
			animator.StopAll();
			animator.Play("Walk_Right");
		}
	}
	//Attack right
	if (Dir == true && AttackAnim == true && PlayerWalking == false) {
		if (!animator.GetAnim("Attack_Right")->bIsPlaying) {
			animator.StopAll();
			animator.Play("Attack_Right", true);
		}
	}
	//Draw attack anim
	if (animator.GetAnim("Attack_Right")->bIsPlaying) {
		animator.UpdateAnimations(fElapsedTime);
		//Right
		if (Dir == true) {
			animator.DrawAnimationFrame(tv.WorldToScreen({ PlayerPos.x - 1.8f, PlayerPos.y - 2.0f }));
		}
	}
	//Stop attack anim when over
	if (!animator.GetAnim("Attack_Right")->bIsPlaying) {
		AttackAnim = false;
	}
	//Draw walk anim
	if (PlayerWalking == true && AttackAnim == false) {
		animator.UpdateAnimations(fElapsedTime);
		//Right
		if (Dir == true) {
			animator.DrawAnimationFrame(tv.WorldToScreen({ PlayerPos.x - 1.55f, PlayerPos.y - 2.0f }));
		}
		//Left
		if (Dir == false) {
			animator.DrawAnimationFrame(tv.WorldToScreen({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }));
		}
	}
}

void Player::Initialize() {
	MovementDirection = { 0, 0 };
	//Sprites
	PlayerRight = std::make_unique<olc::Sprite>("./Sprites/CharacterRightFacing.png");
	PlayerLeft = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing.png");
	PlayerDead = std::make_unique<olc::Sprite>("./Sprites/CharacterDeathPose.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	WalkLeftSS = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing-Sheet.png");
	WalkRightSS = std::make_unique<olc::Sprite>("./Sprites/CharacterRightFacing-Sheet.png");
	PlayerRightAttackSS = std::make_unique<olc::Sprite>("./Sprites/CharacterRightAttacking-Sheet.png");
	//Decals
	PlayerRightDecal = new olc::Decal(PlayerRight.get());
	PlayerLeftDecal = new olc::Decal(PlayerLeft.get());
	PlayerDeadDecal = new olc::Decal(PlayerDead.get());
	ShadowDecal = new olc::Decal(Shadow.get());
	WalkLeftSSDecal = new olc::Decal(WalkLeftSS.get());
	WalkRightSSDecal = new olc::Decal(WalkRightSS.get());
	PlayerRightAttackSSDecal = new olc::Decal(PlayerRightAttackSS.get());

	//Animations
	animator.AddAnimation("Walk_Right", 0.4f, 6, WalkRightSSDecal, { 0.0f, 0.0f }, { 32.0f, 32.0f }, { 0.0f, 0.0f });
	animator.ScaleAnimation("Walk_Right", { 4.0f, 4.0f });
	animator.AddAnimation("Walk_Left", 0.4f, 6, WalkLeftSSDecal, { 0.0f, 0.0f }, { 32.0f, 32.0f }, { 0.0f, 0.0f });
	animator.ScaleAnimation("Walk_Left", { 4.0f, 4.0f });
	animator.AddAnimation("Attack_Right", 0.2f, 6, PlayerRightAttackSSDecal, { 0.0f, 0.0f }, { 32.0f, 32.0f }, { 0.0f, 0.0f });
	animator.ScaleAnimation("Attack_Right", { 4.0f, 4.0f });
}