#ifndef STAR_HPP
#define STAR_HPP

#include "Item.hpp"
#include "Player.hpp"

class Star : public Item {
public:
    enum class State { SPAWNING, MOVING };

    Star(float spawnWorldX, float spawnWorldY);
    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) override;
    void ApplyEffect(Player* player) override;
    Rect GetRect(float worldOffset) const override;

private:
    State m_State = State::SPAWNING;

    float m_SpawnStartY;
    float m_SpeedX = 4.0f;            // 星星水平移動速度 (稍快一點)
    float m_VelocityY = 0.0f;

    // 🌟 星星重力修正 (配合 Player 的 convention，使用正數)
    const float GRAVITY = 0.6f;

    // 🌟 星星彈跳力修正 (正數，用來賦予向上的初始速度)
    const float BOUNCE_FORCE = 12.0f;
};

#endif