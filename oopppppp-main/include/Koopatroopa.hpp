#pragma once
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Player.hpp"
#include <vector>
#include <string>

class Koopatroopa : public Util::GameObject {
public:
    // 🌟 1. 擴充飛天狀態
    enum class State {
        FLYING,        // 🌟 飛天彈跳中
        WALKING,       // 正常走路
        SHELL_IDLE,    // 縮在殼裡不動
        SHELL_MOVING,  // 被踢飛高速滑行
        DEAD           // 翻肚死亡
    };

    // 🌟 2. 加上 isFlying 開關，預設為 false (這樣舊的烏龜就不會受影響)
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

    // 動畫與圖片
    std::vector<std::string> m_WalkImages;
    std::vector<std::string> m_FlyImages; // 🌟 裝飛天圖片的背包
    std::string m_DieImage;

    float m_AnimTimer = 0.0f;
    int m_AnimFrame = 0;

    float m_DeadTimer = 0.0f;
    float m_IdleTimer = 0.0f;
};