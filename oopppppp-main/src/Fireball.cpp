#include "Fireball.hpp"

Fireball::Fireball(float worldX, float worldY, bool faceRight) : AnimatedCharacter({}) {
    m_WorldX = worldX; // 🌟 存下真實世界座標
    m_WorldY = worldY;

    m_FlyImages = {
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball1.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball2.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball3.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/fireball4.png"
    };

    m_ExplodeImages = {
        GA_RESOURCE_DIR"/Image/Items/fireball/boom1.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/boom2.png",
        GA_RESOURCE_DIR"/Image/Items/fireball/boom3.png"
    };

    SetAnimation(m_FlyImages, 50);
    SetZIndex(6);
    m_Transform.scale = { 3.0f, 3.0f };

    m_Velocity.x = faceRight ? m_SpeedX : -m_SpeedX;
    m_Velocity.y = -300.0f;
    SetVisible(true);
    SetPosition({ m_WorldX, m_WorldY });
}

void Fireball::Update(float dt, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_IsDestroyed) return;

    if (m_IsExploding) {
        if (IfAnimationEnds()) {
            m_IsDestroyed = true;
        }
        SetPosition({ m_WorldX - worldOffset, m_WorldY }); // 爆炸時定在原地
        return;
    }

    m_WorldX += m_Velocity.x * dt;
    m_Velocity.y -= m_Gravity * dt;
    m_WorldY += m_Velocity.y * dt;

    Rect fireballBox = { m_WorldX - 12.0f, m_WorldY - 12.0f, 24.0f, 24.0f };

    bool touchedWall = false;

    for (const auto& obs : obstacles) {
        if (obs.width == 0 || obs.height == 0) continue;

        if (CollisionHandler::CheckCollision(fireballBox, obs)) {
            float fireballBottom = fireballBox.y;
            float obsTop = obs.y + obs.height;

            if (fireballBottom - m_Velocity.y * dt >= obsTop - 10.0f) {
                m_WorldY = obsTop + 12.0f;
                m_Velocity.y = 800.0f;
            }
            else {
                touchedWall = true;
            }
        }
    }

    float screenX = m_WorldX - worldOffset;

    // 掉到黑色長條 (-384.0f) 或撞牆就爆炸
    // 並且加上：如果螢幕座標飛出左右邊界 (大於 384 或小於 -384)，也視為越界引爆！
    if (touchedWall || m_WorldY < -384.0f || screenX < -384.0f || screenX > 384.0f) {
        Explode();
    }

    // 🌟 每一幀都扣掉鏡頭位移，這樣火球才不會跟著螢幕跑
    SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Fireball::Explode() {
    if (!m_IsExploding) {
        m_IsExploding = true;
        m_Velocity = { 0.0f, 0.0f };
        SetAnimation(m_ExplodeImages, 80);
        SetLooping(false);
    }
}

Rect Fireball::GetRect() const {
    return { m_WorldX - 12.0f, m_WorldY - 12.0f, 24.0f, 24.0f }; // 回傳世界座標
}