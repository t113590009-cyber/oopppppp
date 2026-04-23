#pragma once
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Player.hpp"
#include <vector>
#include <string>

class Koopatroopa : public Util::GameObject {
public:
    Koopatroopa(float spawnWorldX, float spawnWorldY);

    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles);

    // 處理與玩家的互動
    void Interact(Player* player, float worldOffset);

    Rect GetRect(float worldOffset) const;

    bool IsDead() const { return m_State == State::DEAD; }

private:
    enum class State {
        WALKING,
        DEAD
    };

    State m_State = State::WALKING;

    float m_WorldX;
    float m_WorldY;
    float m_VelocityY = 0.0f;

    // 🌟 速度設定比較慢
    float m_SpeedX = -1.0f;
    const float GRAVITY = 40.0f;

    // 動畫相關
    std::vector<std::string> m_WalkImages;
    std::string m_DieImage;
    float m_AnimTimer = 0.0f;
    int m_AnimFrame = 0;

    float m_DeadTimer = 0.0f;
};