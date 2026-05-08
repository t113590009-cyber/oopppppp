#include "FireFlower.hpp"
#include "Player.hpp" 

FireFlower::FireFlower(float worldX, float worldY) : Item({}) {
    m_WorldX = worldX;
    m_WorldY = worldY;

    std::vector<std::string> flowerImages = {
        GA_RESOURCE_DIR"/Image/Items/flower/flower1.png",
        GA_RESOURCE_DIR"/Image/Items/flower/flower2.png",
        GA_RESOURCE_DIR"/Image/Items/flower/flower3.png",
        GA_RESOURCE_DIR"/Image/Items/flower/flower4.png"
    };

    SetAnimation(flowerImages, 100);

    // 🌟 1. 大小修正：讓火之花變為三倍 (3.0f, 3.0f)
    this->m_Transform.scale = { 3.0f, 3.0f };

    // 🌟 2. 圖層修正：初始圖層設為 Z=4 (確保躲在 Z=5 的磚塊後面)
    SetZIndex(4);

    SetVisible(true);

    SetPosition({ m_WorldX, m_WorldY });
    m_TargetY = m_WorldY + 48.0f;
}

void FireFlower::Update(float dt, float worldOffset, const std::vector<Rect>& obstacles) {
    (void)obstacles;

    // 🌼 出土動畫邏輯
    if (m_IsSpawning) {
        m_SpawnTimer += dt;
        m_WorldY += 30.0f * dt;

        if (m_WorldY >= m_TargetY) {
            m_WorldY = m_TargetY;
            m_IsSpawning = false;

            // 🌟 3. 圖層修正：出土完成後，圖層升到 Z=6 (在所有地形的前面，让瑪利歐可吃到)
            SetZIndex(6);
        }
    }

    SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void FireFlower::ApplyEffect(Player* player) {
    if (!m_IsSpawning) {
        player->GetFireFlower();
        Destroy();
    }
}

Rect FireFlower::GetRect(float worldOffset) const {
    // 因為花變大了，碰撞箱也要跟著變大 (以 3x 大小的中心點推算)
    return { m_WorldX - 18.0f, m_WorldY - 18.0f, 36.0f, 36.0f };
}