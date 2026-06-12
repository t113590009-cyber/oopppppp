#pragma once
#include "Util/GameObject.hpp"
#include "Player.hpp"

class BowserFire : public Util::GameObject {
public:
    BowserFire(float startX, float startY, bool movingLeft);

    void Update(float deltaTime, float worldOffset);
    void Interact(Player* player, float worldOffset);

    bool IsDestroyed() const { return m_IsDestroyed; }
    void Destroy() { m_IsDestroyed = true; }
    Rect GetRect(float worldOffset) const;

private:
    float m_WorldX;
    float m_WorldY;
    float m_SpeedX;
    bool m_IsDestroyed = false;
    float m_LifeTimer = 6.0f; // 火焰存活時間 (6秒後自動消失)
};