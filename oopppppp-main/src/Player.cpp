#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cmath>

Player::Player() {
    m_StandImages = { GA_RESOURCE_DIR"/Image/Character/mario/stand.png" };
    m_RunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/run3.png"
    };
    m_JumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/jump.png" };

    // 💀 載入死亡圖片
    m_DeadImages = { GA_RESOURCE_DIR"/Image/Character/mario/mariodie.png" };

    m_Mario = std::make_shared<AnimatedCharacter>(m_StandImages);
    m_Mario->SetZIndex(10);
    m_Mario->SetVisible(false);
}

// 💀 觸發死亡的函式
void Player::Die() {
    if (m_CurrentState != AnimState::DEAD) {
        m_CurrentState = AnimState::DEAD;

        // 換上死亡圖片
        m_Mario->SetAnimation(m_DeadImages);
        m_Mario->Play();

        // 重置物理狀態，準備彈飛
        m_Velocity.x = 0.0f;
        m_Velocity.y = 15.0f; // 給一個向上的初速度
        m_DeathTimer = 0.0f;  // 重置計時器，做短暫的停頓特效
    }
}

void Player::Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime) {
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isMovingX = false;
    static bool faceRight = true;
    AnimState nextState = m_CurrentState;

    // ==========================================
    // 💀 0. 死亡狀態邏輯 (最高優先級，無視其他操作)
    // ==========================================
    if (m_CurrentState == AnimState::DEAD) {
        m_DeathTimer += deltaTime;

        // 停頓 0.5 秒後才開始掉落 (經典瑪利歐效果)
        if (m_DeathTimer > 0.5f) {
            m_Velocity.y -= m_Gravity * (deltaTime * 60.0f); // 套用重力
            currentPos.y += m_Velocity.y;

            // 確保瑪利歐永遠面向前方
            m_Mario->m_Transform.scale = { 3.0f, 3.0f };
        }

        m_Mario->SetPosition(currentPos);
        return; // 死亡後不再執行後面的移動和跳躍邏輯
    }

    // ==========================================
    // 🚇 1. 魔法時空隧道邏輯
    // ==========================================
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

    // ==========================================
    // 🌟 2. 輸入偵測與狀態切換
    // ==========================================
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
            else {
                nextState = AnimState::CROUCHING;
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

    // ==========================================
    // 🏃‍♂️ 3. 左右移動與物理碰撞
    // ==========================================
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

    float bodyHeight = (nextState == AnimState::CROUCHING) ? 20.0f : 36.0f;
    float bodyYOffset = (nextState == AnimState::CROUCHING) ? -22.0f : -18.0f;
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

    // ==========================================
    // 🦘 4. 跳躍、重力與落地
    // ==========================================
    if (Util::Input::IsKeyPressed(Util::Keycode::W) && m_IsOnGround && nextState != AnimState::CROUCHING) {
        m_Velocity.y = m_JumpImpulse;
        m_IsOnGround = false;
    }

    if (m_Velocity.y > 0.0f) {
        Rect marioHead = { worldOffset + currentPos.x - 10.0f, currentPos.y + 10.0f, 20.0f, 14.0f };
        for (auto& block : blocks) {
            if (CollisionHandler::CheckCollision(marioHead, block->GetHitbox())) {
                m_Velocity.y = -2.0f;
                block->Hit();
                break;
            }
        }
    }

    if (!m_IsOnGround) m_Velocity.y -= m_Gravity * (deltaTime * 60.0f);
    currentPos.y += m_Velocity.y;

    // 🌟 一旦掉進深淵 (-400.0f)，直接觸發死亡！
    if (currentPos.y < -400.0f && m_CurrentState != AnimState::DEAD) {
        Die();
        return; // 🛑 修正點：加上這行！觸發死亡後立刻中斷後面的更新，防止死亡狀態被覆蓋！
    }

    // 🌟 縮小腳底板寬度 (從 36.0f 縮小成 16.0f)，只要超出邊緣就會掉落
    Rect marioFeet = { worldOffset + currentPos.x - 8.0f, currentPos.y - 25.0f, 16.0f, 5.0f };

    // 🌟 將預設探測底線加深到 -600.0f，確保不會踩到隱形地板
    float groundHeight = collision.GetGroundHeight(marioFeet, -600.0f);

    for (auto& block : blocks) {
        if (CollisionHandler::CheckCollision(marioFeet, block->GetHitbox())) {
            float blockTop = block->GetHitbox().y + block->GetHitbox().height;
            if (blockTop > groundHeight) groundHeight = blockTop;
        }
    }

    float centerOffset = (nextState == AnimState::CROUCHING) ? 12.0f : 24.0f;
    if (currentPos.y <= groundHeight + centerOffset) {
        currentPos.y = groundHeight + centerOffset;
        m_Velocity.y = 0.0f;
        m_IsOnGround = true;
    }
    else {
        m_IsOnGround = false;
    }

    // ==========================================
    // 🎬 5. 動畫更新
    // ==========================================
    // 🛑 修正點：多加一層保護，只有活著的時候才允許切換動畫
    if (m_CurrentState != AnimState::DEAD) {
        m_Mario->m_Transform.scale = { (faceRight ? 3.0f : -3.0f), 3.0f };
        if (m_CurrentState != nextState) {
            if (nextState == AnimState::JUMP) m_Mario->SetAnimation(m_JumpImages);
            else if (nextState == AnimState::RUN) m_Mario->SetAnimation(m_RunImages, 100);
            else m_Mario->SetAnimation(m_StandImages);
            m_Mario->Play();
            m_CurrentState = nextState;
        }
    }
    m_Mario->SetPosition(currentPos);
}

Rect Player::GetFeetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    return { worldOffset + currentPos.x - 8.0f, currentPos.y - 25.0f, 16.0f, 5.0f };
}