#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp" 
#include "Block.hpp" 
#include <memory>
#include <vector>
#include <string>
#include <glm/vec2.hpp>

class Player {
public:
    // 🌟 1. 擴充動畫狀態：區分 A 點下沉與 B 點右移
    enum class AnimState {
        IDLE,
        RUN,
        JUMP,
        CROUCHING,
        WARP_DOWN_A,   // 地表入口 -> A點
        WARP_RIGHT_B,  // 地下室B點 -> 地表出口
        WARP_UP_OUT    // 地表出口 -> 向上冒出
    };

    Player();

    void Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime);

    std::shared_ptr<AnimatedCharacter> GetCharacter() { return m_Mario; }

    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }
    void SetPosition(const glm::vec2& pos) { m_Mario->SetPosition(pos); }
    void SetVisible(bool visible) { m_Mario->SetVisible(visible); }

    Rect GetFeetRect(float worldOffset) const;

    // ➕ 朋友的新增功能：讓 App 知道瑪利歐是不是往下掉 (用於判定踩死栗子球)
    float GetVelocityY() const { return m_Velocity.y; }

private:
    std::shared_ptr<AnimatedCharacter> m_Mario;

    // --- 物理屬性 ---
    glm::vec2 m_Velocity = { 0.0f, 0.0f };
    bool m_IsOnGround = true;
    const float m_Gravity = 0.8f;
    const float m_JumpImpulse = 20.0f;
    const float m_WalkSpeed = 6.0f;

    // 🌟 2. 狀態管理與傳送記憶變數
    AnimState m_CurrentState = AnimState::IDLE;
    float m_WarpStartY = 0.0f;                  // 記錄 A 點開始下沉的高度
    float m_WarpStartX = 0.0f;                  // 記錄 B 點開始推進的橫向座標

    // --- 動畫資源路徑 ---
    std::vector<std::string> m_RunImages;
    std::vector<std::string> m_StandImages;
    std::vector<std::string> m_JumpImages;
};

#endif // PLAYER_HPP