#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <cmath>
#include "MovingBlock.hpp" 

Player::Player() {
    // ==========================================
    // 🍄 載入正常狀態 (無星) 的瑪利歐
    // ==========================================
    m_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/stand.png" };
    m_SmallRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run3.png" };
    m_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/jump.png" };
    m_SmallCrouchImages = m_SmallStandImages;
    m_SmallSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/sprint.png" };

    m_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/stand.png" };
    m_BigRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run3.png" };
    m_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/jump.png" };
    m_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/squat.png" };
    m_BigSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/sprint.png" };

    // ==========================================
    // 🔥 載入火球狀態 (Fire) 的瑪利歐
    // ==========================================
    m_FireStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/stand.png" };
    m_FireRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run3.png" };
    m_FireJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/jump.png" };
    m_FireCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/squat.png" };
    m_FireSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/sprint.png" };

    // ==========================================
    // ⭐ 載入星星狀態 1, 2, 3 
    // ==========================================
    m_Star1_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/stand.png" };
    m_Star1_SmallRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run3.png" };
    m_Star1_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/jump.png" };
    m_Star1_SmallCrouchImages = m_Star1_SmallStandImages;
    m_Star1_SmallSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/sprint.png" };

    m_Star1_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/stand.png" };
    m_Star1_BigRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run3.png" };
    m_Star1_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/jump.png" };
    m_Star1_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/squat.png" };
    m_Star1_BigSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/sprint.png" };

    m_Star2_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/stand.png" };
    m_Star2_SmallRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run3.png" };
    m_Star2_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/jump.png" };
    m_Star2_SmallCrouchImages = m_Star2_SmallStandImages;
    m_Star2_SmallSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/sprint.png" };

    m_Star2_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/stand.png" };
    m_Star2_BigRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run3.png" };
    m_Star2_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/jump.png" };
    m_Star2_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/squat.png" };
    m_Star2_BigSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/sprint.png" };

    m_Star3_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/stand.png" };
    m_Star3_SmallRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run3.png" };
    m_Star3_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/jump.png" };
    m_Star3_SmallCrouchImages = m_Star3_SmallStandImages;
    m_Star3_SmallSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/sprint.png" };

    m_Star3_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/stand.png" };
    m_Star3_BigRunImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run3.png" };
    m_Star3_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/jump.png" };
    m_Star3_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/squat.png" };
    m_Star3_BigSprintImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/sprint.png" };

    m_ChangeImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change3.png" };
    m_Star1_ChangeImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change3.png" };
    m_Star2_ChangeImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change3.png" };
    m_Star3_ChangeImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change1.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change2.png", GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change3.png" };
    m_DeadImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/mariodie.png" };

    RefreshAnimations();
    m_Mario = std::make_shared<AnimatedCharacter>(*m_CurrentStandImages);
    m_Mario->SetZIndex(10);
    m_Mario->SetVisible(false);
}

void Player::RefreshAnimations() {
    if (!m_IsStarMode || m_StarColorIndex == 0) {
        m_CurrentChangeImages = &m_ChangeImages;
        if (m_IsFire) {
            m_CurrentRunImages = &m_FireRunImages; m_CurrentStandImages = &m_FireStandImages;
            m_CurrentJumpImages = &m_FireJumpImages; m_CurrentCrouchImages = &m_FireCrouchImages;
            m_CurrentSprintImages = &m_FireSprintImages;
        }
        else if (m_IsBig) {
            m_CurrentRunImages = &m_BigRunImages; m_CurrentStandImages = &m_BigStandImages;
            m_CurrentJumpImages = &m_BigJumpImages; m_CurrentCrouchImages = &m_BigCrouchImages;
            m_CurrentSprintImages = &m_BigSprintImages;
        }
        else {
            m_CurrentRunImages = &m_SmallRunImages; m_CurrentStandImages = &m_SmallStandImages;
            m_CurrentJumpImages = &m_SmallJumpImages; m_CurrentCrouchImages = &m_SmallCrouchImages;
            m_CurrentSprintImages = &m_SmallSprintImages;
        }
    }
    else if (m_StarColorIndex == 1) {
        m_CurrentChangeImages = &m_Star1_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star1_BigRunImages; m_CurrentStandImages = &m_Star1_BigStandImages;
            m_CurrentJumpImages = &m_Star1_BigJumpImages; m_CurrentCrouchImages = &m_Star1_BigCrouchImages;
            m_CurrentSprintImages = &m_Star1_BigSprintImages;
        }
        else {
            m_CurrentRunImages = &m_Star1_SmallRunImages; m_CurrentStandImages = &m_Star1_SmallStandImages;
            m_CurrentJumpImages = &m_Star1_SmallJumpImages; m_CurrentCrouchImages = &m_Star1_SmallCrouchImages;
            m_CurrentSprintImages = &m_Star1_SmallSprintImages;
        }
    }
    else if (m_StarColorIndex == 2) {
        m_CurrentChangeImages = &m_Star2_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star2_BigRunImages; m_CurrentStandImages = &m_Star2_BigStandImages;
            m_CurrentJumpImages = &m_Star2_BigJumpImages; m_CurrentCrouchImages = &m_Star2_BigCrouchImages;
            m_CurrentSprintImages = &m_Star2_BigSprintImages;
        }
        else {
            m_CurrentRunImages = &m_Star2_SmallRunImages; m_CurrentStandImages = &m_Star2_SmallStandImages;
            m_CurrentJumpImages = &m_Star2_SmallJumpImages; m_CurrentCrouchImages = &m_Star2_SmallCrouchImages;
            m_CurrentSprintImages = &m_Star2_SmallSprintImages;
        }
    }
    else if (m_StarColorIndex == 3) {
        m_CurrentChangeImages = &m_Star3_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star3_BigRunImages; m_CurrentStandImages = &m_Star3_BigStandImages;
            m_CurrentJumpImages = &m_Star3_BigJumpImages; m_CurrentCrouchImages = &m_Star3_BigCrouchImages;
            m_CurrentSprintImages = &m_Star3_BigSprintImages;
        }
        else {
            m_CurrentRunImages = &m_Star3_SmallRunImages; m_CurrentStandImages = &m_Star3_SmallStandImages;
            m_CurrentJumpImages = &m_Star3_SmallJumpImages; m_CurrentCrouchImages = &m_Star3_SmallCrouchImages;
            m_CurrentSprintImages = &m_Star3_SmallSprintImages;
        }
    }
}

void Player::GrowUp() {
    if (!m_IsBig) { m_IsBig = true; RefreshAnimations(); m_CurrentState = AnimState::CHANGING; m_ChangeTimer = 0.4f; m_Mario->SetAnimation(*m_CurrentChangeImages, 100); }
}

void Player::GetFireFlower() {
    if (!m_IsFire) { m_IsBig = true; m_IsFire = true; RefreshAnimations(); m_CurrentState = AnimState::CHANGING; m_ChangeTimer = 0.4f; m_Mario->SetAnimation(*m_CurrentChangeImages, 100); }
}

void Player::TakeDamage() {
    if (m_IsStarMode || m_InvincibleTimer > 0.0f || m_CurrentState == AnimState::DEAD) return;

    if (m_IsBig || m_IsFire) {
        m_IsBig = false; m_IsFire = false; m_InvincibleTimer = 2.0f;
        RefreshAnimations(); m_CurrentState = AnimState::CHANGING; m_ChangeTimer = 0.4f; m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
    }
    else {
        Die();
    }
}

void Player::Die() {
    if (m_CurrentState == AnimState::DEAD) return;
    m_CurrentState = AnimState::DEAD;

    m_IsBig = false;
    m_IsFire = false;
    m_IsStarMode = false;

    if (!m_DeadImages.empty()) m_Mario->SetAnimation(m_DeadImages);
    m_Mario->m_Transform.scale = { 3.0f, 3.0f };
    m_Velocity.x = 0.0f;
    m_Velocity.y = 15.0f * NES_SCALE;
}

void Player::ResetStatus() {
    m_CurrentState = AnimState::IDLE;

    m_IsStarMode = false;

    m_Velocity = { 0.0f, 0.0f };
    RefreshAnimations();
    m_Mario->SetAnimation(*m_CurrentStandImages);
    m_Mario->Play();
}

void Player::GetStar() {
    m_IsStarMode = true; m_StarTimer = 10.0f; m_StarColorIndex = 1; m_StarAnimTimer = 0.05f; RefreshAnimations();
}

void Player::Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime) {

    if (m_CurrentState == AnimState::FLAG_SLIDE) { m_Mario->m_Transform.scale = { 3.0f, 3.0f }; return; }

    glm::vec2 currentPos = m_Mario->GetPosition();

    if (m_CurrentState == AnimState::DEAD) {
        m_DeathTimer += deltaTime;
        if (m_DeathTimer > 0.5f) {
            float deathGravity = 0.43f * NES_SCALE;
            m_Velocity.y -= deathGravity * deltaTime;
            currentPos.y += m_Velocity.y * deltaTime;
            m_Mario->m_Transform.scale = { 3.0f, 3.0f };
        }
        m_Mario->SetPosition(currentPos);
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::NUM_9)) {
        if (m_IsStarMode) {
            m_StarTimer = 0.0f;
        }
        else {
            GetStar();
            m_StarTimer = 99999.0f;
        }
    }

    if (m_CurrentState == AnimState::RUN) {
        m_RunAnimTimer += deltaTime;
        if (m_RunAnimTimer >= 0.1f) {
            m_RunAnimTimer = 0.0f; m_RunFrameIndex = (m_RunFrameIndex + 1) % 3;
            if (m_IsStarMode && m_CurrentState != AnimState::CHANGING) {
                std::vector<std::string> singleFrame = { (*m_CurrentRunImages)[m_RunFrameIndex] }; m_Mario->SetAnimation(singleFrame);
            }
        }
    }

    if (m_IsStarMode) {
        m_StarTimer -= deltaTime; m_StarAnimTimer -= deltaTime;
        if (m_StarAnimTimer <= 0.0f) {
            m_StarAnimTimer = 0.05f; m_StarColorIndex++; if (m_StarColorIndex > 3) m_StarColorIndex = 1;
            RefreshAnimations();
            if (m_CurrentState == AnimState::CHANGING) m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
            else {
                if (m_CurrentState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
                else if (m_CurrentState == AnimState::SPRINT) m_Mario->SetAnimation(*m_CurrentSprintImages);
                else if (m_CurrentState == AnimState::RUN) { std::vector<std::string> sf = { (*m_CurrentRunImages)[m_RunFrameIndex] }; m_Mario->SetAnimation(sf); }
                else if (m_CurrentState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
                else m_Mario->SetAnimation(*m_CurrentStandImages);
            }
        }
        if (m_StarTimer <= 0.0f) {
            m_IsStarMode = false; m_StarColorIndex = 0; RefreshAnimations();
            if (m_CurrentState == AnimState::CHANGING) m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
            else {
                if (m_CurrentState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
                else if (m_CurrentState == AnimState::SPRINT) m_Mario->SetAnimation(*m_CurrentSprintImages);
                else if (m_CurrentState == AnimState::RUN) m_Mario->SetAnimation(*m_CurrentRunImages, 100);
                else if (m_CurrentState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
                else m_Mario->SetAnimation(*m_CurrentStandImages);
            }
        }
    }

    if (m_CurrentState == AnimState::CHANGING) {
        m_ChangeTimer -= deltaTime;
        if (m_ChangeTimer <= 0.0f) { m_CurrentState = AnimState::IDLE; m_Mario->SetAnimation(*m_CurrentStandImages); }
        m_Mario->Play();
    }

    if (m_InvincibleTimer > 0.0f && !m_IsStarMode) {
        m_InvincibleTimer -= deltaTime; m_BlinkTimer += deltaTime;
        if (m_BlinkTimer > 0.1f) { static bool blinkToggle = true; blinkToggle = !blinkToggle; m_Mario->SetVisible(blinkToggle); m_BlinkTimer = 0.0f; }
        if (m_InvincibleTimer <= 0.0f) { m_InvincibleTimer = 0.0f; m_IsInvincible = false; if (m_CurrentState != AnimState::DEAD) m_Mario->SetVisible(true); }
    }

    AnimState nextState = m_CurrentState;
    if (m_CurrentState == AnimState::WARP_DOWN_A) {
        currentPos.y -= 1.5f * (deltaTime * 60.0f);
        if (currentPos.y < m_WarpStartY - 60.0f) { worldOffset = 15384.0f; currentPos.x = -264.0f; currentPos.y = 216.0f; nextState = AnimState::JUMP; m_Velocity.y = 0.0f; }
        m_Mario->SetPosition(currentPos); m_CurrentState = nextState; return;
    }
    if (m_CurrentState == AnimState::WARP_RIGHT_B) {
        m_Mario->m_Transform.scale = { -3.0f, 3.0f }; currentPos.x += 1.0f * (deltaTime * 60.0f);
        if (currentPos.x > m_WarpStartX + 50.0f) { worldOffset = 7444.0f + 48.0f; currentPos.x = 0.0f; currentPos.y = -240.0f; nextState = AnimState::WARP_UP_OUT; m_Velocity.y = 0.0f; }
        m_Mario->SetPosition(currentPos); m_CurrentState = nextState; return;
    }
    if (m_CurrentState == AnimState::WARP_UP_OUT) {
        currentPos.y += 1.5f * (deltaTime * 60.0f);
        if (currentPos.y >= -168.0f) { currentPos.y = -168.0f; nextState = AnimState::IDLE; }
        m_Mario->SetPosition(currentPos); m_CurrentState = nextState; return;
    }

    bool btnLeft = Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::LEFT);
    bool btnRight = Util::Input::IsKeyPressed(Util::Keycode::D) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT);
    bool btnJump = Util::Input::IsKeyPressed(Util::Keycode::W) || Util::Input::IsKeyPressed(Util::Keycode::UP);
    bool btnDash = Util::Input::IsKeyPressed(Util::Keycode::B);

    float absoluteX = worldOffset + currentPos.x;
    bool isCrouching = false;

    if (btnDash && !m_WasDashing && m_IsOnGround && std::abs(m_Velocity.x) > 0.5f * NES_SCALE) {
        m_DashAnimTimer = 0.2f;
    }
    m_WasDashing = btnDash;
    if (m_DashAnimTimer > 0.0f) m_DashAnimTimer -= deltaTime;

    if (Util::Input::IsKeyPressed(Util::Keycode::S) || Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (m_IsOnGround && std::abs(absoluteX - 2404.0f) < 25.0f) {
            m_CurrentState = AnimState::WARP_DOWN_A;
            m_WarpStartY = currentPos.y;
            m_Velocity = { 0.0f, 0.0f };
            return;
        }
        else if (m_IsOnGround) {
            isCrouching = true;
        }
    }
    else if (absoluteX > 15600.0f && absoluteX < 15648.0f && m_FacingRight && btnRight) {
        if (m_IsOnGround && std::abs((currentPos.y - (m_IsBig ? 48.0f : 24.0f)) - (-264.0f)) < 15.0f) {
            m_CurrentState = AnimState::WARP_RIGHT_B;
            m_WarpStartX = currentPos.x;
            m_Velocity = { 0.0f, 0.0f };
            return;
        }
    }

    float maxWalkSpeed = 1.56f * NES_SCALE;
    float maxRunSpeed = 2.66f * NES_SCALE;
    float maxSpeed = btnDash ? maxRunSpeed : maxWalkSpeed;

    float walkAccel = 0.037f * NES_SCALE * 60.0f;
    float runAccel = 0.055f * NES_SCALE * 60.0f;
    float accel = btnDash ? runAccel : walkAccel;

    float friction = 0.05f * NES_SCALE * 60.0f;
    float skidDecel = 0.10f * NES_SCALE * 60.0f;

    m_IsSkidding = false;

    if (!isCrouching) {
        if (btnRight) {
            m_FacingRight = true;
            if (m_Velocity.x < 0) {
                m_Velocity.x += skidDecel * deltaTime;
                m_IsSkidding = m_IsOnGround;
            }
            else {
                m_Velocity.x += accel * deltaTime;
            }
        }
        else if (btnLeft) {
            m_FacingRight = false;
            if (m_Velocity.x > 0) {
                m_Velocity.x -= skidDecel * deltaTime;
                m_IsSkidding = m_IsOnGround;
            }
            else {
                m_Velocity.x -= accel * deltaTime;
            }
        }
        else if (m_IsOnGround) {
            if (m_Velocity.x > 0) { m_Velocity.x -= friction * deltaTime; if (m_Velocity.x < 0) m_Velocity.x = 0; }
            else if (m_Velocity.x < 0) { m_Velocity.x += friction * deltaTime; if (m_Velocity.x > 0) m_Velocity.x = 0; }
        }
    }
    else {
        if (m_Velocity.x > 0) { m_Velocity.x -= friction * deltaTime; if (m_Velocity.x < 0) m_Velocity.x = 0; }
        else if (m_Velocity.x < 0) { m_Velocity.x += friction * deltaTime; if (m_Velocity.x > 0) m_Velocity.x = 0; }
    }

    if (!m_IsOnGround) {
        float currentMax = (std::abs(m_Velocity.x) > maxWalkSpeed) ? maxRunSpeed : maxWalkSpeed;
        if (m_Velocity.x > currentMax) m_Velocity.x = currentMax;
        if (m_Velocity.x < -currentMax) m_Velocity.x = -currentMax;
    }
    else {
        if (m_Velocity.x > maxSpeed) m_Velocity.x = maxSpeed;
        if (m_Velocity.x < -maxSpeed) m_Velocity.x = -maxSpeed;
    }

    if (!m_IsOnGround) nextState = AnimState::JUMP;
    else if (isCrouching) nextState = AnimState::CROUCHING;
    else if (m_IsSkidding || m_DashAnimTimer > 0.0f) nextState = AnimState::SPRINT;
    else if (std::abs(m_Velocity.x) > 0.05f) nextState = AnimState::RUN;
    else nextState = AnimState::IDLE;

    glm::vec2 nextPosX = currentPos;
    nextPosX.x += m_Velocity.x * deltaTime;

    float bodyHeight = m_IsBig ? (isCrouching ? 72.0f : 96.0f) : 48.0f;
    float halfHeight = bodyHeight / 2.0f;
    if (m_IsOnGround && m_CurrentState != AnimState::CHANGING && nextState != AnimState::CHANGING) {
        float prevHalf = m_IsBig ? (m_CurrentState == AnimState::CROUCHING ? 36.0f : 48.0f) : 24.0f;
        if (prevHalf != halfHeight) currentPos.y += (halfHeight - prevHalf);
    }

    // ==========================================
    // 🌟 真正包含「防瞬移邏輯」的完美防卡牆系統
    // ==========================================
    bool canMoveX = true;
    float adjustedX = nextPosX.x;

    auto checkWallCollision = [&](const Rect& obs) {
        if (obs.width == 0 || obs.height == 0) return;

        float marioTop = currentPos.y + halfHeight;
        float marioBottom = currentPos.y - halfHeight;
        float obsTop = obs.y + obs.height;
        float obsBottom = obs.y;

        // 【防護 1】踩在方塊上：容錯加到 12 像素，只要在方塊上緣，絕對不啟動推擠
        if (marioBottom >= obsTop - 12.0f) return;

        // 【防護 2】頂在方塊下：容錯加到 12 像素，只要在方塊下緣，絕對不啟動推擠
        if (marioTop <= obsBottom + 12.0f) return;

        // 【防護 3】把瑪利歐肚子判定區縮得更小 (上下各削掉 12 像素)
        Rect checkRect = { worldOffset + adjustedX - 18.0f, currentPos.y - halfHeight + 12.0f, 36.0f, bodyHeight - 24.0f };

        if (CollisionHandler::CheckCollision(checkRect, obs)) {
            float obsLeft = obs.x;
            float obsRight = obs.x + obs.width;
            float marioRight = worldOffset + adjustedX + 18.0f;
            float marioLeft = worldOffset + adjustedX - 18.0f;

            float overlapRight = marioRight - obsLeft;
            float overlapLeft = obsRight - marioLeft;

            // 🌟 核心防瞬移：只有「卡進去少於 24 像素」，我們才承認這是撞到牆壁！
            // 如果大於 24 像素，代表他是踩在超級大地板上，絕對不推！
            if (m_Velocity.x > 0 && overlapRight > 0 && overlapRight < 24.0f) {
                adjustedX = obsLeft - 18.1f - worldOffset;
                canMoveX = false;
            }
            else if (m_Velocity.x < 0 && overlapLeft > 0 && overlapLeft < 24.0f) {
                adjustedX = obsRight + 18.1f - worldOffset;
                canMoveX = false;
            }
            else if (m_Velocity.x == 0) {
                if (overlapRight > 0 && overlapRight < overlapLeft && overlapRight < 24.0f) {
                    adjustedX = obsLeft - 18.1f - worldOffset;
                    canMoveX = false;
                }
                else if (overlapLeft > 0 && overlapLeft < 24.0f) {
                    adjustedX = obsRight + 18.1f - worldOffset;
                    canMoveX = false;
                }
            }
        }
        };

    // 檢查所有的牆壁和障礙物
    for (const auto& obs : collision.GetObstacles()) checkWallCollision(obs);
    for (auto& block : blocks) checkWallCollision(block->GetHitbox());

    // 🌟 更新為安全座標
    currentPos.x = adjustedX;

    if (!canMoveX) {
        m_Velocity.x = 0.0f;
    }

    // 畫面邊界限制
    if (worldOffset > 15000.0f) {
        if (currentPos.x < -360.0f) { currentPos.x = -360.0f; m_Velocity.x = 0; }
        if (currentPos.x > 450.0f) { currentPos.x = 450.0f; m_Velocity.x = 0; }
    }
    else {
        if (currentPos.x > 0.0f) { worldOffset += currentPos.x; currentPos.x = 0.0f; }
        if (currentPos.x < -360.0f) { currentPos.x = -360.0f; m_Velocity.x = 0; }
    }

    // ==========================================
    // 🦘 動態跳躍與重力
    // ==========================================
    bool isCheatJump = (m_IsStarMode && m_StarTimer > 50000.0f);

    if ((m_IsOnGround || isCheatJump) && btnJump && !m_WasJumping && !isCrouching) {
        float absVx = std::abs(m_Velocity.x);
        float jumpInitVel = 4.10f;

        if (absVx >= maxRunSpeed - (0.1f * NES_SCALE)) jumpInitVel = 5.00f;
        else if (absVx >= maxWalkSpeed) jumpInitVel = 4.12f;

        m_Velocity.y = jumpInitVel * NES_SCALE;
        m_IsOnGround = false;
    }

    float gravity = 0.43f * NES_SCALE * 60.0f;

    if (btnJump && m_Velocity.y > 0) {
        float absVx = std::abs(m_Velocity.x);
        if (absVx > maxWalkSpeed) gravity = 0.15f * NES_SCALE * 60.0f;
        else gravity = 0.11f * NES_SCALE * 60.0f;
    }

    // 處理頭部往上跳的碰撞
    if (m_Velocity.y > 0.0f) {
        float headYOffset = halfHeight - 14.0f;
        Rect marioHead = { worldOffset + currentPos.x - 10.0f, currentPos.y + headYOffset, 20.0f, 14.0f };

        for (const auto& obs : collision.GetObstacles()) {
            if (obs.width == 0 || obs.height == 0) continue;
            if (CollisionHandler::CheckCollision(marioHead, obs)) {
                m_Velocity.y = -2.0f * NES_SCALE;
                break;
            }
        }

        for (auto& block : blocks) {
            if (block->GetHitbox().width == 0 || block->GetHitbox().height == 0) continue;
            if (CollisionHandler::CheckCollision(marioHead, block->GetHitbox())) {
                m_Velocity.y = -2.0f * NES_SCALE;
                if (m_IsBig) block->Hit(m_IsBig); else block->Hit();
                break;
            }
        }
    }

    if (!m_IsOnGround) {
        m_Velocity.y -= gravity * deltaTime;
        float maxFallSpeed = -4.00f * NES_SCALE;
        if (m_Velocity.y < maxFallSpeed) m_Velocity.y = maxFallSpeed;
    }

    currentPos.y += m_Velocity.y * deltaTime;
    m_WasJumping = btnJump;

    if (currentPos.y < -400.0f && m_CurrentState != AnimState::DEAD) {
        Die(); return;
    }

    // 地面高度偵測
    float feetYOffset = -halfHeight - 2.0f;
    float centerOffset = halfHeight;
    Rect marioFeet = { worldOffset + currentPos.x - 8.0f, currentPos.y + feetYOffset, 16.0f, 5.0f };
    float groundHeight = collision.GetGroundHeight(marioFeet, -600.0f);

    float platformVelocityX = 0.0f;

    for (auto& block : blocks) {
        if (block->GetHitbox().width == 0 || block->GetHitbox().height == 0) continue;
        if (CollisionHandler::CheckCollision(marioFeet, block->GetHitbox())) {
            float blockTop = block->GetHitbox().y + block->GetHitbox().height;
            if (blockTop > groundHeight) {
                groundHeight = blockTop;

                auto movingBlock = std::dynamic_pointer_cast<MovingBlock>(block);
                if (movingBlock) {
                    platformVelocityX = movingBlock->GetVelocityX();
                }
                else {
                    platformVelocityX = 0.0f;
                }
            }
        }
    }

    if (currentPos.y <= groundHeight + centerOffset + 2.0f && m_Velocity.y <= 0.0f) {
        currentPos.y = groundHeight + centerOffset;
        m_Velocity.y = 0.0f;
        m_IsOnGround = true;

        if (platformVelocityX != 0.0f) {
            currentPos.x += platformVelocityX * deltaTime;
        }
    }
    else {
        m_IsOnGround = false;
    }

    // 動畫更新與切換
    if (m_CurrentState != AnimState::CHANGING) {
        if (m_CurrentState != nextState) {
            if (nextState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
            else if (nextState == AnimState::SPRINT) m_Mario->SetAnimation(*m_CurrentSprintImages);
            else if (nextState == AnimState::RUN) {
                m_RunFrameIndex = 0; m_RunAnimTimer = 0.0f;
                if (m_IsStarMode) { std::vector<std::string> sf = { (*m_CurrentRunImages)[m_RunFrameIndex] }; m_Mario->SetAnimation(sf); }
                else m_Mario->SetAnimation(*m_CurrentRunImages, 100);
            }
            else if (nextState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
            else m_Mario->SetAnimation(*m_CurrentStandImages);

            m_Mario->Play();
            m_CurrentState = nextState;
        }
    }

    m_Mario->SetPosition(currentPos);
    m_Mario->m_Transform.scale = { (m_FacingRight ? 3.0f : -3.0f), 3.0f };
}

Rect Player::GetFeetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isCrouching = (m_CurrentState == AnimState::CROUCHING);
    float bodyHeight = m_IsBig ? (isCrouching ? 72.0f : 96.0f) : 48.0f;
    float feetYOffset = -(bodyHeight / 2.0f) - 2.0f;
    return { worldOffset + currentPos.x - 8.0f, currentPos.y + feetYOffset, 16.0f, 5.0f };
}

void Player::StartFlagSlide(float poleWorldX) {
    if (m_CurrentState == AnimState::DEAD || m_CurrentState == AnimState::FLAG_SLIDE) return;
    m_CurrentState = AnimState::FLAG_SLIDE;
    m_Velocity = { 0.0f, 0.0f };
}

Rect Player::GetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    float width = 36.0f;
    float height = m_IsBig ? 72.0f : 40.0f;
    return { worldOffset + currentPos.x - (width / 2.0f), currentPos.y - (height / 2.0f), width, height };
}

void Player::SetState(AnimState state) {
    m_CurrentState = state;
    if (state == AnimState::AUTO_WALK || state == AnimState::RUN) {
        if (m_CurrentRunImages) { m_Mario->SetAnimation(*m_CurrentRunImages, 100); m_Mario->Play(); }
    }
    else if (state == AnimState::IDLE) {
        if (m_CurrentStandImages) m_Mario->SetAnimation(*m_CurrentStandImages);
    }
}

void Player::SetWorldPosition(float worldX, float y) {
    if (m_Mario) m_Mario->SetPosition({ worldX, y });
}

float Player::GetWorldX(float worldOffset) const {
    return worldOffset + m_Mario->GetPosition().x;
}

void Player::Bounce() {
    m_Velocity.y = 400.0f;
}