#include "BowserFire.hpp"
#include "Util/Image.hpp"
#include "CollisionHandler.hpp"

BowserFire::BowserFire(float startX, float startY, bool movingLeft)
    : m_WorldX(startX), m_WorldY(startY) {

    // 🌟 修正點：給予火球超長的存活時間 (10秒)！
    // 這樣火焰就可以輕易飛越一整個螢幕而不會中途消失了。
    m_LifeTimer = 10.0f;

    m_SpeedX = movingLeft ? -150.0f : 150.0f; // 火焰飛行速度

    // 🌟 讀取你的火焰圖片
    this->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/Props/firespain.png"));
    this->m_Transform.scale = { (movingLeft ? 3.0f : -3.0f), 3.0f };
    this->SetZIndex(6); // 讓火焰顯示在比較上層
}

void BowserFire::Update(float deltaTime, float worldOffset) {
    m_LifeTimer -= deltaTime;
    if (m_LifeTimer <= 0.0f) m_IsDestroyed = true;

    // 火焰無視物理與地形，直接水平移動
    m_WorldX += m_SpeedX * deltaTime;
    this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY };
}

void BowserFire::Interact(Player* player, float worldOffset) {
    if (m_IsDestroyed || !player || player->IsDead()) return;

    Rect fireRect = GetRect(worldOffset);
    Rect playerBody = { worldOffset + player->GetPosition().x - 18.0f, player->GetPosition().y - 20.0f, 36.0f, 40.0f };

    // 碰到瑪利歐就讓他受傷
    if (CollisionHandler::CheckCollision(fireRect, playerBody)) {
        player->TakeDamage();
    }
}

Rect BowserFire::GetRect(float worldOffset) const {
    // 根據 firespain.png 抓取的扁平碰撞箱
    return { m_WorldX - 24.0f, m_WorldY - 8.0f, 48.0f, 16.0f };
}