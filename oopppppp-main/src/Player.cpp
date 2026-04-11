#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <cmath> 

Player::Player() {
    // ==========================================
    // 🍄 載入正常狀態 (無星) 的瑪利歐
    // ==========================================
    m_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/stand.png" };
    m_SmallRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run3.png"
    };
    m_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/jump.png" };
    m_SmallCrouchImages = m_SmallStandImages;

    m_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/stand.png" };
    m_BigRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run3.png"
    };
    m_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/jump.png" };
    m_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/squat.png" };

    // ==========================================
    // ⭐ 載入星星狀態 1 (Star1)
    // ==========================================
    m_Star1_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/stand.png" };
    m_Star1_SmallRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/run3.png"
    };
    m_Star1_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/small/jump.png" };
    m_Star1_SmallCrouchImages = m_Star1_SmallStandImages;

    m_Star1_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/stand.png" };
    m_Star1_BigRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/run3.png"
    };
    m_Star1_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/jump.png" };
    m_Star1_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/big/squat.png" };

    // ==========================================
    // ⭐ 載入星星狀態 2 (Star2)
    // ==========================================
    m_Star2_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/stand.png" };
    m_Star2_SmallRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/run3.png"
    };
    m_Star2_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/small/jump.png" };
    m_Star2_SmallCrouchImages = m_Star2_SmallStandImages;

    m_Star2_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/stand.png" };
    m_Star2_BigRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/run3.png"
    };
    m_Star2_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/jump.png" };
    m_Star2_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/big/squat.png" };

    // ==========================================
    // ⭐ 載入星星狀態 3 (Star3)
    // ==========================================
    m_Star3_SmallStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/stand.png" };
    m_Star3_SmallRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/run3.png"
    };
    m_Star3_SmallJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/small/jump.png" };
    m_Star3_SmallCrouchImages = m_Star3_SmallStandImages;

    m_Star3_BigStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/stand.png" };
    m_Star3_BigRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/run3.png"
    };
    m_Star3_BigJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/jump.png" };
    m_Star3_BigCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/big/squat.png" };

    // ==========================================
    // 💫 載入變身過場閃爍 (包含無敵彩虹狀態)
    // ==========================================
    m_ChangeImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/normal/change/change3.png"
    };

    m_Star1_ChangeImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star1/change/change3.png"
    };

    m_Star2_ChangeImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star2/change/change3.png"
    };

    m_Star3_ChangeImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/star/overworld/star3/change/change3.png"
    };

    RefreshAnimations();
    m_Mario = std::make_shared<AnimatedCharacter>(*m_CurrentStandImages);
    m_Mario->SetZIndex(10);
    m_Mario->SetVisible(false);
}

void Player::RefreshAnimations() {
    if (!m_IsStarMode || m_StarColorIndex == 0) {
        m_CurrentChangeImages = &m_ChangeImages; // 🌟 正常變身
        if (m_IsBig) {
            m_CurrentRunImages = &m_BigRunImages;
            m_CurrentStandImages = &m_BigStandImages;
            m_CurrentJumpImages = &m_BigJumpImages;
            m_CurrentCrouchImages = &m_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_SmallRunImages;
            m_CurrentStandImages = &m_SmallStandImages;
            m_CurrentJumpImages = &m_SmallJumpImages;
            m_CurrentCrouchImages = &m_SmallCrouchImages;
        }
    }
    else if (m_StarColorIndex == 1) {
        m_CurrentChangeImages = &m_Star1_ChangeImages; // 🌟 星星1變身
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star1_BigRunImages;
            m_CurrentStandImages = &m_Star1_BigStandImages;
            m_CurrentJumpImages = &m_Star1_BigJumpImages;
            m_CurrentCrouchImages = &m_Star1_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star1_SmallRunImages;
            m_CurrentStandImages = &m_Star1_SmallStandImages;
            m_CurrentJumpImages = &m_Star1_SmallJumpImages;
            m_CurrentCrouchImages = &m_Star1_SmallCrouchImages;
        }
    }
    else if (m_StarColorIndex == 2) {
        m_CurrentChangeImages = &m_Star2_ChangeImages; // 🌟 星星2變身
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star2_BigRunImages;
            m_CurrentStandImages = &m_Star2_BigStandImages;
            m_CurrentJumpImages = &m_Star2_BigJumpImages;
            m_CurrentCrouchImages = &m_Star2_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star2_SmallRunImages;
            m_CurrentStandImages = &m_Star2_SmallStandImages;
            m_CurrentJumpImages = &m_Star2_SmallJumpImages;
            m_CurrentCrouchImages = &m_Star2_SmallCrouchImages;
        }
    }
    else if (m_StarColorIndex == 3) {
        m_CurrentChangeImages = &m_Star3_ChangeImages; // 🌟 星星3變身
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star3_BigRunImages;
            m_CurrentStandImages = &m_Star3_BigStandImages;
            m_CurrentJumpImages = &m_Star3_BigJumpImages;
            m_CurrentCrouchImages = &m_Star3_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star3_SmallRunImages;
            m_CurrentStandImages = &m_Star3_SmallStandImages;
            m_CurrentJumpImages = &m_Star3_SmallJumpImages;
            m_CurrentCrouchImages = &m_Star3_SmallCrouchImages;
        }
    }
}

void Player::GrowUp() {
    if (!m_IsBig) {
        m_IsBig = true;
        RefreshAnimations();
        m_CurrentState = AnimState::CHANGING;
        m_ChangeTimer = 0.4f;
        // 🌟 修正：不管有沒有無敵，都會吃到正確的變身動畫陣列
        m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
    }
}

void Player::TakeDamage() {
    // 🌟 在無敵狀態下受傷，完全不會觸發變小或死亡
    if (m_IsStarMode || m_IsInvincible || m_CurrentState == AnimState::CHANGING) return;

    if (m_IsBig) {
        m_IsBig = false;
        RefreshAnimations();
        m_CurrentState = AnimState::CHANGING;
        m_ChangeTimer = 0.4f;
        m_Mario->SetAnimation(*m_CurrentChangeImages, 100); // 🌟 修正

        m_IsInvincible = true;
        m_InvincibleTimer = 2.0f;
    }
    else {
        LOG_DEBUG("MARIO DIED! GAME OVER!");
    }
}

void Player::GetStar() {
    m_IsStarMode = true;
    m_StarTimer = 10.0f;
    m_StarColorIndex = 1;
    m_StarAnimTimer = 0.05f;
    RefreshAnimations();
}

void Player::Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime) {

    // ==========================================
    // 🌟 座標探測器：按 P 鍵在 Console 印出瑪利歐絕對座標 (找隱形牆用)
    // ==========================================
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        glm::vec2 pos = m_Mario->GetPosition();
        float absoluteX = worldOffset + pos.x;
        float absoluteY = pos.y;
        LOG_DEBUG("📍 瑪利歐座標 -> X: {}, Y: {}", absoluteX, absoluteY);
    }

    // ==========================================
    // 🌟 手動計算跑步動畫幀數 (防卡幀機制)
    // ==========================================
    if (m_CurrentState == AnimState::RUN) {
        m_RunAnimTimer += deltaTime;
        if (m_RunAnimTimer >= 0.1f) {
            m_RunAnimTimer = 0.0f;
            m_RunFrameIndex = (m_RunFrameIndex + 1) % 3; // 跑步圖有3張，0, 1, 2 循環

            // 🛡️ 保護機制：只有在星星狀態且「沒有在變身時」，才套用跑步圖
            if (m_IsStarMode && m_CurrentState != AnimState::CHANGING) {
                std::vector<std::string> singleFrame = { (*m_CurrentRunImages)[m_RunFrameIndex] };
                m_Mario->SetAnimation(singleFrame);
            }
        }
    }

    // ==========================================
    // 🌟 無敵星星：快速循環變色邏輯
    // ==========================================
    if (m_IsStarMode) {
        m_StarTimer -= deltaTime;
        m_StarAnimTimer -= deltaTime;

        if (m_StarAnimTimer <= 0.0f) {
            m_StarAnimTimer = 0.05f;
            m_StarColorIndex++;
            if (m_StarColorIndex > 3) m_StarColorIndex = 1;

            RefreshAnimations();

            // 🛡️ 保護機制：變身時星星不要干擾動畫切換，但如果是變身中，我們還是要把動畫指派給它讓他閃
            if (m_CurrentState == AnimState::CHANGING) {
                m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
            }
            else {
                if (m_CurrentState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
                else if (m_CurrentState == AnimState::RUN) {
                    std::vector<std::string> singleFrame = { (*m_CurrentRunImages)[m_RunFrameIndex] };
                    m_Mario->SetAnimation(singleFrame);
                }
                else if (m_CurrentState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
                else m_Mario->SetAnimation(*m_CurrentStandImages);
            }
        }

        if (m_StarTimer <= 0.0f) {
            m_IsStarMode = false;
            m_StarColorIndex = 0;
            RefreshAnimations();

            if (m_CurrentState == AnimState::CHANGING) {
                m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
            }
            else {
                if (m_CurrentState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
                else if (m_CurrentState == AnimState::RUN) m_Mario->SetAnimation(*m_CurrentRunImages, 100);
                else if (m_CurrentState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
                else m_Mario->SetAnimation(*m_CurrentStandImages);
            }
        }
    }

    // ==========================================
    // 💫 變身過場動畫 (CHANGING)
    // ==========================================
    if (m_CurrentState == AnimState::CHANGING) {
        m_ChangeTimer -= deltaTime;

        if (m_ChangeTimer <= 0.0f) {
            m_CurrentState = AnimState::IDLE;
            m_Mario->SetAnimation(*m_CurrentStandImages);
        }
        m_Mario->Play(); // 確保過場動畫會繼續播放
    }

    // ==========================================
    // 🛡️ 無敵受傷閃爍邏輯 (一般狀態下受傷)
    // ==========================================
    if (m_IsInvincible && !m_IsStarMode) {
        m_InvincibleTimer -= deltaTime;
        m_BlinkTimer += deltaTime;
        if (m_BlinkTimer > 0.1f) {
            static bool blinkToggle = true;
            blinkToggle = !blinkToggle;
            m_Mario->SetVisible(blinkToggle);
            m_BlinkTimer = 0.0f;
        }
        if (m_InvincibleTimer <= 0.0f) {
            m_IsInvincible = false;
            m_Mario->SetVisible(true);
        }
    }

    // ==========================================
    // 🏃‍♂️ 物理、移動與碰撞邏輯
    // ==========================================
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isMovingX = false;
    static bool faceRight = true;
    AnimState nextState = m_CurrentState;

    if (m_CurrentState == AnimState::WARP_DOWN_A) {
        currentPos.y -= 1.5f * (deltaTime * 60.0f);
        if (currentPos.y < m_WarpStartY - 60.0f) {
            worldOffset = 15384.0f;
            currentPos.x = -264.0f;
            currentPos.y = 216.0f;
            nextState = AnimState::JUMP;
            m_Velocity.y = 0.0f;
        }
        m_Mario->SetPosition(currentPos);
        m_CurrentState = nextState;
        return;
    }
    if (m_CurrentState == AnimState::WARP_RIGHT_B) {
        m_Mario->m_Transform.scale = { -3.0f, 3.0f };
        currentPos.x += 1.0f * (deltaTime * 60.0f);
        if (currentPos.x > m_WarpStartX + 50.0f) {
            worldOffset = 7444.0f + 48.0f;
            currentPos.x = 0.0f;
            currentPos.y = -240.0f;
            nextState = AnimState::WARP_UP_OUT;
            m_Velocity.y = 0.0f;
        }
        m_Mario->SetPosition(currentPos);
        m_CurrentState = nextState;
        return;
    }
    if (m_CurrentState == AnimState::WARP_UP_OUT) {
        currentPos.y += 1.5f * (deltaTime * 60.0f);
        if (currentPos.y >= -168.0f) {
            currentPos.y = -168.0f;
            nextState = AnimState::IDLE;
        }
        m_Mario->SetPosition(currentPos);
        m_CurrentState = nextState;
        return;
    }

    if (!m_IsOnGround) {
        nextState = AnimState::JUMP;
    }
    else {
        float absoluteX = worldOffset + currentPos.x;
        if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
            if (std::abs(absoluteX - 2404.0f) < 15.0f) {
                nextState = AnimState::WARP_DOWN_A;
                m_WarpStartY = currentPos.y;
            }
            else if (m_IsBig) {
                nextState = AnimState::CROUCHING;
            }
            else {
                nextState = AnimState::IDLE;
            }
        }
        else if (Util::Input::IsKeyPressed(Util::Keycode::A) || Util::Input::IsKeyPressed(Util::Keycode::D)) {
            nextState = AnimState::RUN;
        }
        else {
            nextState = AnimState::IDLE;
        }

        if (absoluteX > 15600.0f && absoluteX < 15648.0f && faceRight) {
            nextState = AnimState::WARP_RIGHT_B;
            m_WarpStartX = currentPos.x;
        }
    }

    float moveStep = (nextState == AnimState::CROUCHING) ? 0.0f : (m_WalkSpeed * deltaTime * 60.0f);
    glm::vec2 nextPosX = currentPos;

    if (nextState != AnimState::CROUCHING) {
        if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
            nextPosX.x -= moveStep;
            faceRight = false;
            isMovingX = true;
        }
        else if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
            nextPosX.x += moveStep;
            faceRight = true;
            isMovingX = true;
        }
    }

    bool isCrouching = (nextState == AnimState::CROUCHING);
    float bodyHeight;

    if (m_IsBig) {
        bodyHeight = isCrouching ? 72.0f : 96.0f;
    }
    else {
        bodyHeight = 48.0f;
    }

    float halfHeight = bodyHeight / 2.0f;

    // 落地時的高度補償 (防亂跳機制)
    if (m_IsOnGround && m_CurrentState != AnimState::CHANGING && nextState != AnimState::CHANGING) {
        float prevHalf = m_IsBig ? (m_CurrentState == AnimState::CROUCHING ? 36.0f : 48.0f) : 24.0f;
        if (prevHalf != halfHeight) {
            currentPos.y += (halfHeight - prevHalf);
        }
    }

    float bodyYOffset = -halfHeight;
    float feetYOffset = -halfHeight - 2.0f;
    float headYOffset = halfHeight - 14.0f;
    float centerOffset = halfHeight;

    Rect marioBodyBox = { worldOffset + nextPosX.x - 18.0f, currentPos.y + bodyYOffset, 36.0f, bodyHeight };

    bool canMove = true;
    for (const auto& obs : collision.GetObstacles()) {
        if (CollisionHandler::CheckCollision(marioBodyBox, obs)) {
            if (currentPos.y - 20.0f < obs.y + obs.height - 2.0f) { canMove = false; break; }
        }
    }
    if (canMove) {
        for (auto& block : blocks) {
            if (CollisionHandler::CheckCollision(marioBodyBox, block->GetHitbox())) {
                if (currentPos.y - 20.0f < block->GetHitbox().y + block->GetHitbox().height - 2.0f) { canMove = false; break; }
            }
        }
    }

    if (canMove) {
        currentPos.x = nextPosX.x;
        if (worldOffset > 15000.0f) {
            if (currentPos.x < -360.0f) currentPos.x = -360.0f;
            if (currentPos.x > 450.0f) currentPos.x = 450.0f;
        }
        else {
            if (currentPos.x > 0.0f) {
                worldOffset += currentPos.x;
                currentPos.x = 0.0f;
            }
            if (currentPos.x < -360.0f) currentPos.x = -360.0f;
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::W) && m_IsOnGround && !isCrouching) {
        m_Velocity.y = m_JumpImpulse;
        m_IsOnGround = false;
    }

    if (m_Velocity.y > 0.0f) {
        Rect marioHead = { worldOffset + currentPos.x - 10.0f, currentPos.y + headYOffset, 20.0f, 14.0f };
        for (auto& block : blocks) {
            if (CollisionHandler::CheckCollision(marioHead, block->GetHitbox())) {
                m_Velocity.y = -2.0f;
                if (m_IsBig) block->Hit(m_IsBig);
                else block->Hit();
                break;
            }
        }
    }

    if (!m_IsOnGround) m_Velocity.y -= m_Gravity * (deltaTime * 60.0f);
    currentPos.y += m_Velocity.y;

    Rect marioFeet = { worldOffset + currentPos.x - 18.0f, currentPos.y + feetYOffset, 36.0f, 5.0f };
    float groundHeight = collision.GetGroundHeight(marioFeet, -360.0f);

    for (auto& block : blocks) {
        if (CollisionHandler::CheckCollision(marioFeet, block->GetHitbox())) {
            float blockTop = block->GetHitbox().y + block->GetHitbox().height;
            if (blockTop > groundHeight) groundHeight = blockTop;
        }
    }

    if (currentPos.y <= groundHeight + centerOffset + 2.0f && m_Velocity.y <= 0.0f) {
        currentPos.y = groundHeight + centerOffset;
        m_Velocity.y = 0.0f;
        m_IsOnGround = true;
    }
    else {
        m_IsOnGround = false;
    }

    m_Mario->m_Transform.scale = { (faceRight ? 3.0f : -3.0f), 3.0f };

    // ==========================================
    // 🎨 狀態切換與動畫賦值
    // ==========================================
    if (m_CurrentState != AnimState::CHANGING) {
        if (m_CurrentState != nextState) {
            if (nextState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
            else if (nextState == AnimState::RUN) {
                // 當剛開始跑步時，重置手動計時器
                m_RunFrameIndex = 0;
                m_RunAnimTimer = 0.0f;
                if (m_IsStarMode) {
                    std::vector<std::string> singleFrame = { (*m_CurrentRunImages)[m_RunFrameIndex] };
                    m_Mario->SetAnimation(singleFrame);
                }
                else {
                    m_Mario->SetAnimation(*m_CurrentRunImages, 100);
                }
            }
            else if (nextState == AnimState::CROUCHING) m_Mario->SetAnimation(*m_CurrentCrouchImages);
            else m_Mario->SetAnimation(*m_CurrentStandImages);

            m_Mario->Play();
            m_CurrentState = nextState;
        }
    }

    m_Mario->SetPosition(currentPos);
}

Rect Player::GetFeetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isCrouching = (m_CurrentState == AnimState::CROUCHING);

    float bodyHeight;
    if (m_IsBig) {
        bodyHeight = isCrouching ? 72.0f : 96.0f;
    }
    else {
        bodyHeight = 48.0f;
    }

    float feetYOffset = -(bodyHeight / 2.0f) - 2.0f;
    return { worldOffset + currentPos.x - 18.0f, currentPos.y + feetYOffset, 36.0f, 5.0f };
}