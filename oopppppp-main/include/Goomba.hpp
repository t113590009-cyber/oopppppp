#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include <memory>
#include <vector>
#include <string>
#include "Character.hpp"
#include "CollisionHandler.hpp"

class Goomba {
public:
    enum class State { WALKING, STOMPED, DEAD };

    // 🌟 確保建構子只有一個 float 參數
    Goomba(float spawnWorldX);

    void Update(float deltaTime, float worldOffset);
    void Stomp();

    // 🌟 確保這行結尾有 const
    Rect GetRect(float worldOffset) const;

    State GetState() const { return m_State; }
    std::shared_ptr<Character> GetDrawable() const { return m_Drawable; }

private:
    std::shared_ptr<Character> m_Drawable;
    State m_State = State::WALKING;

    float m_WorldX;
    const float m_WorldY = -240.0f; // 🌟 依照要求維持 -240.0f

    float m_Speed = 1.45f;
    float m_AnimTimer = 0.0f;
    int m_CurrentFrame = 1;
    float m_DeathTimer = 0.0f;
};

#endif