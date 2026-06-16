#pragma once
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Player.hpp"
#include <vector>
#include <string>

class Koopatroopa : public Util::GameObject {
public:
    enum class State {
        FLYING,        // 🌟 像電梯一樣上下滑動
        WALKING,       // 正常走路
        SHELL_IDLE,    // 縮在殼裡不動
        SHELL_MOVING,  // 被踢飛高速滑行
        DEAD           // 翻肚死亡
    };

    Koopatroopa(float spawnWorldX, float spawnWorldY, bool isFlying = false);

    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles);
    void Interact(Player* player, float worldOffset);
    Rect GetRect(float worldOffset) const;

    bool IsDead() const { return m_State == State::DEAD; }
    State GetState() const { return m_State; }

    void Stomp();
    void Kick(bool toRight);

private:
    State m_State;

    float m_WorldX;
    float m_WorldY;
    float m_VelocityY = 0.0f;

    float m_SpeedX = -1.0f;
    const float GRAVITY = 40.0f;

    // ==========================================
    // 🌟 上下滑動專用的變數
    // ==========================================
    float m_StartY = 0.0f;         // 記錄一開始生出來的高度
    float m_FlyRange = 100.0f;     // 上下移動範圍 (100代表往上100、往下100)
    int m_FlyDirection = 1;        // 1 往上，-1 往下
    float m_FlySpeed = 60.0f;      // 上下移動速度

    // 動畫與圖片
    std::vector<std::string> m_WalkImages;
    std::vector<std::string> m_FlyImages;
    std::string m_DieImage;

    float m_AnimTimer = 0.0f;
    int m_AnimFrame = 0;

    float m_DeadTimer = 0.0f;
    float m_IdleTimer = 0.0f;
};