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

        // --- 🧱 水管碰撞預測邏輯 ---
        float nextX = m_WorldX + m_VelocityX;

        // 建立預測的「世界座標」碰撞箱
        Rect nextRect = { nextX - 24.0f, m_WorldY - 24.0f, 48.0f, 48.0f };
        bool hitWall = false;

        // 檢查是否撞到地圖上的障礙物
        for (const auto& obs : collision.GetObstacles()) {
            if (CollisionHandler::CheckCollision(nextRect, obs)) {
                // 確保撞到的是突起的水管或磚塊 (把容錯值稍微拉高到 5.0f，避免被地板卡住)
                if (obs.y + obs.height > nextRect.y + 5.0f) {

                    // 🌟 關鍵修正：判斷障礙物在栗子球的哪一邊！
                    float goombaCenter = nextX;
                    float obsCenter = obs.x + (obs.width / 2.0f);

                    // 如果往左走，且水管在左邊 -> 才算真正撞牆
                    if (m_VelocityX < 0 && obsCenter < goombaCenter) {
                        hitWall = true;
                        break;
                    }
                    // 如果往右走，且水管在右邊 -> 才算真正撞牆
                    else if (m_VelocityX > 0 && obsCenter > goombaCenter) {
                        hitWall = true;
                        break;
                    }
                    // 如果屁股稍微碰到背後的水管，就會直接無視，順利走出去！
                }
            }
        }

        // 撞牆就反轉方向，沒撞就繼續走
        if (hitWall) {
            m_VelocityX = -m_VelocityX;
        } else {
            m_WorldX = nextX;
        }

        // --- 🏃 動畫更新邏輯 ---
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