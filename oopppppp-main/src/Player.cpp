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
    // 🔥 載入火球狀態 (Fire) 的瑪利歐
    // ==========================================
    m_FireStandImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/stand.png" };
    m_FireRunImages = {
        GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run1.png",
        GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run2.png",
        GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run3.png"
    };
    m_FireJumpImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/jump.png" };
    m_FireCrouchImages = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/squat.png" };

    // ==========================================
    // ⭐ 載入星星狀態 1, 2, 3 (Star1 ~ Star3)
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

    // 💀 載入死亡圖片 (從 V1 整合)
    m_DeadImages = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/mariodie.png" };

    RefreshAnimations();
    m_Mario = std::make_shared<AnimatedCharacter>(*m_CurrentStandImages);
    m_Mario->SetZIndex(10);
    m_Mario->SetVisible(false);
}

void Player::RefreshAnimations() {
    if (!m_IsStarMode || m_StarColorIndex == 0) {
        m_CurrentChangeImages = &m_ChangeImages;
        // 🔥 優先判斷是否為火球狀態
        if (m_IsFire) {
            m_CurrentRunImages = &m_FireRunImages;
            m_CurrentStandImages = &m_FireStandImages;
            m_CurrentJumpImages = &m_FireJumpImages;
            m_CurrentCrouchImages = &m_FireCrouchImages;
        }
        else if (m_IsBig) {
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
        m_CurrentChangeImages = &m_Star1_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star1_BigRunImages; m_CurrentStandImages = &m_Star1_BigStandImages;
            m_CurrentJumpImages = &m_Star1_BigJumpImages; m_CurrentCrouchImages = &m_Star1_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star1_SmallRunImages; m_CurrentStandImages = &m_Star1_SmallStandImages;
            m_CurrentJumpImages = &m_Star1_SmallJumpImages; m_CurrentCrouchImages = &m_Star1_SmallCrouchImages;
        }
    }
    else if (m_StarColorIndex == 2) {
        m_CurrentChangeImages = &m_Star2_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star2_BigRunImages; m_CurrentStandImages = &m_Star2_BigStandImages;
            m_CurrentJumpImages = &m_Star2_BigJumpImages; m_CurrentCrouchImages = &m_Star2_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star2_SmallRunImages; m_CurrentStandImages = &m_Star2_SmallStandImages;
            m_CurrentJumpImages = &m_Star2_SmallJumpImages; m_CurrentCrouchImages = &m_Star2_SmallCrouchImages;
        }
    }
    else if (m_StarColorIndex == 3) {
        m_CurrentChangeImages = &m_Star3_ChangeImages;
        if (m_IsBig) {
            m_CurrentRunImages = &m_Star3_BigRunImages; m_CurrentStandImages = &m_Star3_BigStandImages;
            m_CurrentJumpImages = &m_Star3_BigJumpImages; m_CurrentCrouchImages = &m_Star3_BigCrouchImages;
        }
        else {
            m_CurrentRunImages = &m_Star3_SmallRunImages; m_CurrentStandImages = &m_Star3_SmallStandImages;
            m_CurrentJumpImages = &m_Star3_SmallJumpImages; m_CurrentCrouchImages = &m_Star3_SmallCrouchImages;
        }
    }
}

void Player::GrowUp() {
    if (!m_IsBig) {
        m_IsBig = true;
        RefreshAnimations();
        m_CurrentState = AnimState::CHANGING;
        m_ChangeTimer = 0.4f;
        m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
    }
}

// 🔥 吃到火之花！
void Player::GetFireFlower() {
    if (!m_IsFire) {
        m_IsBig = true;  // 防呆：小瑪利歐直接吃到火花也會變大
        m_IsFire = true;
        RefreshAnimations();
        m_CurrentState = AnimState::CHANGING;
        m_ChangeTimer = 0.4f;
        m_Mario->SetAnimation(*m_CurrentChangeImages, 100);
    }
}

void Player::TakeDamage() {
    // 1. 如果無敵星狀態、正在受傷後的無敵時間中、或已經死了，就免疫傷害！
    if (m_IsStarMode || m_InvincibleTimer > 0.0f || m_CurrentState == AnimState::DEAD) {
        return;
    }

    // 🔥 只要是大隻或是火球狀態，受傷一律退回小瑪利歐 (經典初代設定)
    if (m_IsBig || m_IsFire) {
        m_IsBig = false;
        m_IsFire = false; // 失去火球能力
        m_InvincibleTimer = 2.0f; // 🌟 關鍵：給予 2 秒的無敵時間

        RefreshAnimations();
        m_CurrentState = AnimState::CHANGING;
        m_ChangeTimer = 0.4f;
        m_Mario->SetAnimation(*m_CurrentChangeImages, 100);

        LOG_DEBUG("Mario: Big/Fire -> Small (Invincible for 2s)");
    }
    else {
        // 💀 小瑪利歐：沒救了，觸發死亡
        Die();
    }
}

// 💀 完整的死亡觸發函式 (這份檔案只能留這一個！)
void Player::Die() {
    // 如果已經是死亡狀態，就不要重複執行
    if (m_CurrentState == AnimState::DEAD) return;

    m_CurrentState = AnimState::DEAD;

    // 1. 切換成死亡圖片
    if (!m_DeadImages.empty()) {
        m_Mario->SetAnimation(m_DeadImages);
    }

    // 2. 🌟 確保死亡時，瑪利歐維持 3 倍大小（不縮水）
    m_Mario->m_Transform.scale = { 3.0f, 3.0f };

    // 3. 設定死亡噴出的物理效果
    m_Velocity.x = 0.0f;  // 停止水平移動
    m_Velocity.y = 15.0f; // 往上彈一下的動作
}

// 🔄 復活重置：將瑪利歐狀態完全洗白 (這份檔案只能留這一個！)
void Player::ResetStatus() {
    m_CurrentState = AnimState::IDLE;
    m_IsBig = false;
    m_IsFire = false; // 🔥 重置火球狀態
    m_IsStarMode = false;
    m_Velocity = { 0.0f, 0.0f }; // 速度歸零

    RefreshAnimations(); // 重新整理指標

    m_Mario->SetAnimation(*m_CurrentStandImages);
    m_Mario->Play();
}

void Player::GetStar() {
    m_IsStarMode = true;
    m_StarTimer = 10.0f;
    m_StarColorIndex = 1;
    m_StarAnimTimer = 0.05f;
    RefreshAnimations();
}

void Player::Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime) {

    // 🚩 旗桿與過關相關實作
    if (m_CurrentState == AnimState::FLAG_SLIDE) {
        m_Mario->m_Transform.scale = { 3.0f, 3.0f };
        glm::vec2 pos = m_Mario->GetPosition();

        // 往下掉的速度
        float slideSpeed = 200.0f * deltaTime;
        pos.y -= slideSpeed;

        // 偵測是否滑到底部 (這裡的 -264.0f 是地磚高度，請依你的地圖微調)
        if (pos.y <= -264.0f) {
            pos.y = -264.0f;
            // TODO: 等滑到底部後，這裡會再切換成 AUTO_WALK 讓他走向城堡
        }

        m_Mario->SetPosition(pos);
        return; // 🌟 關鍵：直接 return，跳過後面的所有鍵盤控制！
    }

    glm::vec2 currentPos = m_Mario->GetPosition();

    // 💀 0. 死亡狀態邏輯
    if (m_CurrentState == AnimState::DEAD) {
        m_DeathTimer += deltaTime;
        if (m_DeathTimer > 0.5f) {
            m_Velocity.y -= m_Gravity * (deltaTime * 60.0f);
            currentPos.y += m_Velocity.y;
            m_Mario->m_Transform.scale = { 3.0f, 3.0f };
        }
        m_Mario->SetPosition(currentPos);
        return;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        float absoluteX = worldOffset + currentPos.x;
        LOG_DEBUG("📍 瑪利歐座標 -> X: {}, Y: {}", absoluteX, currentPos.y);
    }

    if (m_CurrentState == AnimState::RUN) {
        m_RunAnimTimer += deltaTime;
        if (m_RunAnimTimer >= 0.1f) {
            m_RunAnimTimer = 0.0f;
            m_RunFrameIndex = (m_RunFrameIndex + 1) % 3;
            if (m_IsStarMode && m_CurrentState != AnimState::CHANGING) {
                std::vector<std::string> singleFrame = { (*m_CurrentRunImages)[m_RunFrameIndex] };
                m_Mario->SetAnimation(singleFrame);
            }
        }
    }

    if (m_IsStarMode) {
        m_StarTimer -= deltaTime;
        m_StarAnimTimer -= deltaTime;

        if (m_StarAnimTimer <= 0.0f) {
            m_StarAnimTimer = 0.05f;
            m_StarColorIndex++;
            if (m_StarColorIndex > 3) m_StarColorIndex = 1;

            RefreshAnimations();

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

    // 處理變身閃爍過場
    if (m_CurrentState == AnimState::CHANGING) {
        m_ChangeTimer -= deltaTime;
        if (m_ChangeTimer <= 0.0f) {
            m_CurrentState = AnimState::IDLE;
            m_Mario->SetAnimation(*m_CurrentStandImages);
        }
        m_Mario->Play();
    }

    // ==========================================
    // 👇 無敵倒數與閃爍邏輯！
    // ==========================================
    if (m_InvincibleTimer > 0.0f && !m_IsStarMode) {
        m_InvincibleTimer -= deltaTime;
        m_BlinkTimer += deltaTime;

        // 每 0.1 秒閃爍一次
        if (m_BlinkTimer > 0.1f) {
            static bool blinkToggle = true;
            blinkToggle = !blinkToggle;
            m_Mario->SetVisible(blinkToggle);
            m_BlinkTimer = 0.0f;
        }

        // 無敵時間結束時，確保瑪利歐顯示出來
        if (m_InvincibleTimer <= 0.0f) {
            m_InvincibleTimer = 0.0f;
            m_IsInvincible = false; // 保險起見同步關閉舊變數
            if (m_CurrentState != AnimState::DEAD) {
                m_Mario->SetVisible(true);
            }
        }
    }
    // ==========================================

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
    float bodyHeight = m_IsBig ? (isCrouching ? 72.0f : 96.0f) : 48.0f;
    float halfHeight = bodyHeight / 2.0f;

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
        if (obs.width == 0 || obs.height == 0) continue; // 🌟 加回幽靈方塊防護
        if (CollisionHandler::CheckCollision(marioBodyBox, obs)) {
            if (currentPos.y - 20.0f < obs.y + obs.height - 2.0f) { canMove = false; break; }
        }
    }
    if (canMove) {
        for (auto& block : blocks) {
            if (block->GetHitbox().width == 0 || block->GetHitbox().height == 0) continue; // 🌟 加回幽靈方塊防護
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
            if (block->GetHitbox().width == 0 || block->GetHitbox().height == 0) continue; // 🌟 加回幽靈方塊防護
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

    if (currentPos.y < -400.0f && m_CurrentState != AnimState::DEAD) {
        Die();
        return;
    }

    Rect marioFeet = { worldOffset + currentPos.x - 8.0f, currentPos.y + feetYOffset, 16.0f, 5.0f };
    float groundHeight = collision.GetGroundHeight(marioFeet, -600.0f);

    for (auto& block : blocks) {
        if (block->GetHitbox().width == 0 || block->GetHitbox().height == 0) continue; // 🌟 加回幽靈方塊防護
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

    // ==========================================
    // 🎨 狀態切換與動畫賦值
    // ==========================================
    if (m_CurrentState != AnimState::CHANGING) {
        if (m_CurrentState != nextState) {
            if (nextState == AnimState::JUMP) m_Mario->SetAnimation(*m_CurrentJumpImages);
            else if (nextState == AnimState::RUN) {
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

    // 🌟 修正核心：把縮放移到最後面！不管引擎剛剛怎麼重置動畫，我們最後一刻強制把它放大 3 倍！
    m_Mario->m_Transform.scale = { (faceRight ? 3.0f : -3.0f), 3.0f };
}

Rect Player::GetFeetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    bool isCrouching = (m_CurrentState == AnimState::CROUCHING);

    float bodyHeight = m_IsBig ? (isCrouching ? 72.0f : 96.0f) : 48.0f;
    float feetYOffset = -(bodyHeight / 2.0f) - 2.0f;

    return { worldOffset + currentPos.x - 8.0f, currentPos.y + feetYOffset, 16.0f, 5.0f };
}

void Player::StartFlagSlide(float poleWorldX) {
    if (m_CurrentState == AnimState::DEAD || m_CurrentState == AnimState::FLAG_SLIDE) {
        return;
    }

    m_CurrentState = AnimState::FLAG_SLIDE;
    m_Velocity = { 0.0f, 0.0f };
}

// 🌟 這個是給烏龜/道具判斷碰撞用的
Rect Player::GetRect(float worldOffset) const {
    glm::vec2 currentPos = m_Mario->GetPosition();
    float width = 36.0f;
    float height = m_IsBig ? 72.0f : 40.0f;

    return {
        worldOffset + currentPos.x - (width / 2.0f),
        currentPos.y - (height / 2.0f),
        width,
        height
    };
}