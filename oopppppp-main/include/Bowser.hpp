#pragma once
#include "Util/GameObject.hpp"
#include "Player.hpp"
#include "BowserFire.hpp"
#include <vector>
#include <memory>

class Bowser : public Util::GameObject {
public:
    enum class State { ALIVE, DEAD };

    Bowser(float spawnWorldX, float spawnWorldY);

    // 🌟 修改：不再需要傳入 Root，改成直接回傳產生的火球！
    std::shared_ptr<BowserFire> Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles);

    void Interact(Player* player, float worldOffset);
    Rect GetRect(float worldOffset) const;

    bool IsDead() const { return m_State == State::DEAD; }
    void TakeDamage(int damage);

private:
    State m_State = State::ALIVE;
    float m_WorldX;
    float m_WorldY;
    float m_VelocityY = 0.0f;
    float m_SpeedX = -30.0f;
    int m_HP = 5;

    float m_WalkTimer = 0.0f;
    float m_FireTimer = 2.0f;
    float m_JumpTimer = 3.0f;

    const float GRAVITY = 40.0f;
};