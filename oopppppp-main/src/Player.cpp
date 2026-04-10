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

    m_Mario = std::make_shared<AnimatedCharacter>(m_StandImages);
    m_Mario->SetZIndex(10);
    m_Mario->SetVisible(false);
}

void Player::Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime) {
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isMovingX = false;
    static bool faceRight = true;
    AnimState nextState = m_CurrentState;

    // ==========================================
    // 🚇 1. 魔法時空隧道邏輯 (最高優先級)
    // ==========================================

    // --- 【狀態 A：下沉傳送 ➡️ 進入遙遠的地下室】 ---
    if (m_CurrentState == AnimState::WARP_DOWN_A) {
        currentPos.y -= 1.5f * (deltaTime * 60.0f);

        if (currentPos.y < m_WarpStartY - 60.0f) {
            worldOffset = 15384.0f;   // 鏡頭對準地下室中央
            currentPos.x = -264.0f;   // 從 A 點 (左上角) 掉落
            currentPos.y = 216.0f;    // 從高處掉落

            nextState = AnimState::JUMP;
            m_Velocity.y = 0.0f;
        }
        m_Mario->SetPosition(currentPos);
        m_CurrentState = nextState;
        return;
    }

    // --- 【狀態 B：向右推進傳送 ➡️ 準備升起】 ---
    if (m_CurrentState == AnimState::WARP_RIGHT_B) {
        m_Mario->m_Transform.scale = { -3.0f, 3.0f }; // 背對鏡頭
        currentPos.x += 1.0f * (deltaTime * 60.0f);   // 推進去

        if (currentPos.x > m_WarpStartX + 50.0f) {
            // 🌟 瞬間移動回地表 7444 區的「水管底部」
            worldOffset = 7444.0f + 48.0f;
            currentPos.x = 0.0f;
            currentPos.y = -240.0f; // 藏在水管底層的 Y 座標

            nextState = AnimState::WARP_UP_OUT; // 切換到「向上冒出」狀態
            m_Velocity.y = 0.0f;
        }
        m_Mario->SetPosition(currentPos);
        m_CurrentState = nextState;
        return;
    }

    // --- 【狀態 C：從地表水管慢慢冒出】 ---
    if (m_CurrentState == AnimState::WARP_UP_OUT) {
        currentPos.y += 1.5f * (deltaTime * 60.0f); // 慢慢上升

        // 水管頂部高度大約在 -168.0f (若你覺得太高或太低可以改這個數字)
        if (currentPos.y >= -168.0f) {
            currentPos.y = -168.0f;
            nextState = AnimState::IDLE; // 出來後恢復自由站立
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

        // --- A 點觸發 (地表入口) ---
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

        // --- B 點觸發 (地下室出口) ---
        // 絕對 X 在 15576~15624 之間且面右撞牆
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

        // 🌟 鏡頭鎖定邏輯：判斷目前是不是在地下室
        if (worldOffset > 15000.0f) {
            if (currentPos.x < -360.0f) currentPos.x = -360.0f; // 左邊界
            if (currentPos.x > 450.0f) currentPos.x = 450.0f;   // 允許出右邊界一點點去撞水管
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

    // 套用重力
    if (!m_IsOnGround) m_Velocity.y -= m_Gravity * (deltaTime * 60.0f);
    currentPos.y += m_Velocity.y;

    // 🌟 將重力探測底線改回乾淨的 -360.0f
    Rect marioFeet = { worldOffset + currentPos.x - 18.0f, currentPos.y - 25.0f, 36.0f, 5.0f };
    float groundHeight = collision.GetGroundHeight(marioFeet, -360.0f);

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
    m_Mario->m_Transform.scale = { (faceRight ? 3.0f : -3.0f), 3.0f };
    if (m_CurrentState != nextState) {
        if (nextState == AnimState::JUMP) m_Mario->SetAnimation(m_JumpImages);
        else if (nextState == AnimState::RUN) m_Mario->SetAnimation(m_RunImages, 100);
        else m_Mario->SetAnimation(m_StandImages);
        m_Mario->Play();
        m_CurrentState = nextState;
    }
    m_Mario->SetPosition(currentPos);
}

Rect Player::GetFeetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    return { worldOffset + currentPos.x - 18.0f, currentPos.y - 25.0f, 36.0f, 5.0f };
}