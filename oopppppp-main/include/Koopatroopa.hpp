#pragma once
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Player.hpp"
#include <vector>
#include <string>

class Koopatroopa : public Util::GameObject {
public:
    // 🌟 1. 將 State 移到 public 區塊，這樣 AppUpdate 才能看到它！
    enum class State {
        WALKING,
        DEAD
    };

    Koopatroopa(float spawnWorldX, float spawnWorldY);

    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles);

    // 處理與玩家的互動
    void Interact(Player* player, float worldOffset);

    Rect GetRect(float worldOffset) const;

    bool IsDead() const { return m_State == State::DEAD; }

    // 🌟 2. 補上讓外部取得當前狀態的函式
    State GetState() const { return m_State; }

    // 🌟 3. 補上受擊函式 (被踩踏或被火球炸)
    void Stomp();

private:
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