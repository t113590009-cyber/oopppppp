#include "Goomba.hpp"

// 🌟 修正 C2511: 確保與標頭檔的 Goomba(float) 完全對應
Goomba::Goomba(float spawnWorldX) : m_WorldX(spawnWorldX) {
    m_Drawable = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/goomba/goomba1.png");
    m_Drawable->m_Transform.scale = { 3.0f, 3.0f };
    m_Drawable->SetZIndex(5);
}

// 🌟 參數加入 const CollisionHandler& collision
void Goomba::Update(float deltaTime, float worldOffset, const CollisionHandler& collision) {
    if (m_State == State::WALKING) {

        // ==========================================
        // 🧱 1. 水管碰撞預測邏輯 (X 軸)
        // ==========================================
        float nextX = m_WorldX + m_VelocityX;

        // 建立預測的「世界座標」碰撞箱
        Rect nextRect = { nextX - 24.0f, m_WorldY - 24.0f, 48.0f, 48.0f };
        bool hitWall = false;

        // 檢查是否撞到地圖上的障礙物
        for (const auto& obs : collision.GetObstacles()) {
            if (CollisionHandler::CheckCollision(nextRect, obs)) {
                // 確保撞到的是突起的水管或磚塊 (把容錯值稍微拉高到 5.0f，避免被地板卡住)
                if (obs.y + obs.height > nextRect.y + 5.0f) {
                    // 判斷障礙物在栗子球的哪一邊！
                    float goombaCenter = nextX;
                    float obsCenter = obs.x + (obs.width / 2.0f);

                    if (m_VelocityX < 0 && obsCenter < goombaCenter) {
                        hitWall = true; break;
                    } else if (m_VelocityX > 0 && obsCenter > goombaCenter) {
                        hitWall = true; break;
                    }
                }
            }
        }

        // 撞牆就反轉方向，沒撞就繼續走
        if (hitWall) {
            m_VelocityX = -m_VelocityX;
        } else {
            m_WorldX = nextX;
        }

        // ==========================================
        // 🕳️ 2. 地心引力與掉落邏輯 (Y 軸)
        // ==========================================
        // 建立栗子球的腳底板 (寬度稍微縮小一點點，這樣才不會在邊緣懸空卡住)
        Rect goombaFeet = { m_WorldX - 15.0f, m_WorldY - 25.0f, 30.0f, 5.0f };

        // 取得腳下的地板高度，預設深淵底線為 -400.0f
        float groundHeight = collision.GetGroundHeight(goombaFeet, -400.0f);

        // 如果不在地上，套用重力往下掉
        if (!m_IsOnGround) {
            m_VelocityY -= 0.8f * (deltaTime * 60.0f); // 模擬重力
        }

        m_WorldY += m_VelocityY;

        // 判斷是否踩到地上 (中心點到腳底距離大約 24.0f)
        if (m_WorldY <= groundHeight + 24.0f) {
            m_WorldY = groundHeight + 24.0f;
            m_VelocityY = 0.0f;
            m_IsOnGround = true;
        } else {
            m_IsOnGround = false; // 腳下懸空，切換為掉落狀態
        }

        // 💀 3. 掉進深淵死亡判定 (掉出畫面外)
        if (m_WorldY < -360.0f) {
            m_State = State::DEAD;
            m_Drawable->SetVisible(false);
            return; // 已經死透了，提早結束 Update 不再播動畫
        }

        // ==========================================
        // 🏃 4. 動畫更新邏輯
        // ==========================================
        m_AnimTimer += deltaTime;
        if (m_AnimTimer >= 0.2f) {
            m_CurrentFrame = (m_CurrentFrame == 1) ? 2 : 1;
            m_Drawable->SetImage(GA_RESOURCE_DIR"/Image/Character/goomba/goomba" + std::to_string(m_CurrentFrame) + ".png");
            m_AnimTimer = 0.0f;
        }
    }
    else if (m_State == State::STOMPED) {
        m_DeathTimer += deltaTime;
        if (m_DeathTimer >= 0.5f) {
            m_State = State::DEAD;
            m_Drawable->SetVisible(false);
        }
    }

    // 更新最終繪製座標
    m_Drawable->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Goomba::Stomp() {
    if (m_State == State::WALKING) {
        m_State = State::STOMPED;
        m_Drawable->SetImage(GA_RESOURCE_DIR"/Image/Character/goomba/goombadie.png");
    }
}

// 🌟 修正 C2270: 確保這行跟標頭檔一樣有 const，且屬於 Goomba::
Rect Goomba::GetRect(float worldOffset) const {
    Rect result;
    result.x = m_WorldX - worldOffset - 24.0f;
    result.y = m_WorldY - 24.0f;
    result.width = 48.0f;
    result.height = 48.0f;
    return result;
}