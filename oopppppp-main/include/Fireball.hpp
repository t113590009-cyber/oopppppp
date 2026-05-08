#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp"
#include <memory>
#include <vector>

class Fireball : public AnimatedCharacter {
public:
    Fireball(float worldX, float worldY, bool faceRight);
    void Update(float dt, float worldOffset, const std::vector<Rect>& obstacles);
    Rect GetRect() const; // 🌟 取得火球的世界座標碰撞箱
    void Explode();
    bool IsDestroyed() const { return m_IsDestroyed; }
    bool IsExploding() const { return m_IsExploding; }

private:
    float m_WorldX; // 🌟 新增：記住真實世界的位置
    float m_WorldY;
    bool m_IsExploding = false;
    bool m_IsDestroyed = false;

    glm::vec2 m_Velocity;
    float m_Gravity = 2500.0f;
    float m_SpeedX = 600.0f;

    std::vector<std::string> m_FlyImages;
    std::vector<std::string> m_ExplodeImages;
};

#endif // FIREBALL_HPP