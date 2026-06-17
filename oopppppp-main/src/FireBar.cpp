#include "FireBar.hpp"
#include <cmath> // 為了使用 std::cos 和 std::sin

FireBar::FireBar(float spawnWorldX, float spawnWorldY, int numFireballs, float speed, float initialAngle)
    : m_WorldX(spawnWorldX), m_WorldY(spawnWorldY), m_NumFireballs(numFireballs), m_Speed(speed), m_Angle(initialAngle) {

    // 🌟 直接利用瑪利歐的火球圖片來當作火柱素材
    std::vector<std::string> fireballImages = {
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball1.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball2.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball3.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball4.png"
    };

    for (int i = 0; i < numFireballs; ++i) {
        auto fb = std::make_shared<AnimatedCharacter>(fireballImages);
        fb->SetAnimation(fireballImages, 50); // 每 50ms 閃爍一次
        fb->m_Transform.scale = { 3.0f, 3.0f }; // 大小要跟瑪利歐的火球一致
        fb->SetZIndex(6); // 顯示在比較上層，避免被磚塊擋住
        m_Fireballs.push_back(fb);
    }
}

void FireBar::Update(float deltaTime, float worldOffset) {
    // 1. 更新整根火柱的旋轉角度
    m_Angle += m_Speed * deltaTime;

    // 避免數值無限大，把角度限制在 0 ~ 2*PI 之間
    const float PI_2 = 2.0f * 3.14159265f;
    if (m_Angle > PI_2) m_Angle -= PI_2;
    if (m_Angle < 0.0f) m_Angle += PI_2;

    // 2. 更新每一顆火球的位置 (圓周運動公式)
    for (int i = 0; i < m_NumFireballs; ++i) {
        float radius = i * m_Spacing; // 第 i 顆火球離中心的距離

        // x = cx + r * cos(theta), y = cy + r * sin(theta)
        float fbX = m_WorldX + radius * std::cos(m_Angle);
        float fbY = m_WorldY + radius * std::sin(m_Angle);

        m_Fireballs[i]->SetPosition({ fbX - worldOffset, fbY });
    }
}

void FireBar::Interact(Player* player, float worldOffset) {
    if (!player || player->IsDead()) return;

    // 取得瑪利歐的「世界座標」碰撞箱
    Rect playerBody = { worldOffset + player->GetPosition().x - 18.0f, player->GetPosition().y - 20.0f, 36.0f, 40.0f };

    // 檢查瑪利歐有沒有碰到「任何一顆」火球
    for (int i = 0; i < m_NumFireballs; ++i) {
        float radius = i * m_Spacing;
        float fbX = m_WorldX + radius * std::cos(m_Angle);
        float fbY = m_WorldY + radius * std::sin(m_Angle);

        // 單顆火球的碰撞箱大約是 24x24
        Rect fbRect = { fbX - 12.0f, fbY - 12.0f, 24.0f, 24.0f };

        if (CollisionHandler::CheckCollision(playerBody, fbRect)) {
            // 如果瑪利歐是無敵星狀態，不會受傷 (經典設定火柱不能被消滅)
            if (!player->IsStarMode()) {
                player->TakeDamage();
            }
            break; // 只要撞到其中一顆火球就算受傷，直接跳出迴圈
        }
    }
}