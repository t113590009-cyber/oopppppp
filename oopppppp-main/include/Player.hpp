#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp"
#include "Block.hpp"
#include <memory>
#include <vector>
#include <string>

// 🌟 定義瑪利歐的所有動作狀態
enum class AnimState {
    IDLE,
    RUN,
    JUMP,
    CROUCHING,
    CHANGING,     // 變身/無敵暫停狀態
    WARP_DOWN_A,  // 進水管 (下)
    WARP_RIGHT_B, // 進水管 (右)
    WARP_UP_OUT,  // 出水管 (上)
    DEAD          // 💀 死亡狀態
};

class Player {
public:
    Player();

    // 核心更新邏輯
    void Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime);

    // 道具與受傷邏輯
    void GrowUp();
    void TakeDamage();
    void GetStar();
    void Die(); // 💀 死亡函式

    // 取得瑪利歐資訊給外部 (如 AppUpdate) 使用
    Rect GetFeetRect(float worldOffset) const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Mario; }
    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }
    float GetVelocityY() const { return m_Velocity.y; }

    bool IsStarMode() const { return m_IsStarMode; }
    bool IsOnGround() const { return m_IsOnGround; }

    // 🌟 補上這個！讓 AppUpdate 知道瑪利歐死了沒，用來觸發 Fail 畫面
    bool IsDead() const { return m_CurrentState == AnimState::DEAD; }

private:
    void RefreshAnimations();

    std::shared_ptr<AnimatedCharacter> m_Mario;
    AnimState m_CurrentState = AnimState::IDLE;

    // --- 狀態變數 ---
    bool m_IsBig = false;
    bool m_IsInvincible = false;
    float m_InvincibleTimer = 0.0f;
    float m_BlinkTimer = 0.0f;
    float m_ChangeTimer = 0.0f;
    bool m_IsOnGround = true;

    // 🌟 補上這個！瑪利歐專屬的死亡計時器 (控制掉落延遲)
    float m_DeathTimer = 0.0f;

    // ==========================================
    // 🌟 無敵星星專屬變數
    // ==========================================
    bool m_IsStarMode = false;
    float m_StarTimer = 0.0f;
    float m_StarAnimTimer = 0.0f;
    int m_StarColorIndex = 0;

    float m_RunAnimTimer = 0.0f;
    int m_RunFrameIndex = 0;

    // --- 物理與位置變數 ---
    glm::vec2 m_Velocity = { 0.0f, 0.0f };

    float m_WalkSpeed = 5.5f;
    float m_JumpImpulse = 24.0f;
    float m_Gravity = 1.0f;

    float m_WarpStartY = 0.0f;
    float m_WarpStartX = 0.0f;

    // --- 動畫資源路徑陣列 ---
    std::vector<std::string> m_SmallStandImages;
    std::vector<std::string> m_SmallRunImages;
    std::vector<std::string> m_SmallJumpImages;
    std::vector<std::string> m_SmallCrouchImages;

    std::vector<std::string> m_BigStandImages;
    std::vector<std::string> m_BigRunImages;
    std::vector<std::string> m_BigJumpImages;
    std::vector<std::string> m_BigCrouchImages;

    std::vector<std::string> m_Star1_SmallStandImages;
    std::vector<std::string> m_Star1_SmallRunImages;
    std::vector<std::string> m_Star1_SmallJumpImages;
    std::vector<std::string> m_Star1_SmallCrouchImages;
    std::vector<std::string> m_Star1_BigStandImages;
    std::vector<std::string> m_Star1_BigRunImages;
    std::vector<std::string> m_Star1_BigJumpImages;
    std::vector<std::string> m_Star1_BigCrouchImages;

    std::vector<std::string> m_Star2_SmallStandImages;
    std::vector<std::string> m_Star2_SmallRunImages;
    std::vector<std::string> m_Star2_SmallJumpImages;
    std::vector<std::string> m_Star2_SmallCrouchImages;
    std::vector<std::string> m_Star2_BigStandImages;
    std::vector<std::string> m_Star2_BigRunImages;
    std::vector<std::string> m_Star2_BigJumpImages;
    std::vector<std::string> m_Star2_BigCrouchImages;

    std::vector<std::string> m_Star3_SmallStandImages;
    std::vector<std::string> m_Star3_SmallRunImages;
    std::vector<std::string> m_Star3_SmallJumpImages;
    std::vector<std::string> m_Star3_SmallCrouchImages;
    std::vector<std::string> m_Star3_BigStandImages;
    std::vector<std::string> m_Star3_BigRunImages;
    std::vector<std::string> m_Star3_BigJumpImages;
    std::vector<std::string> m_Star3_BigCrouchImages;

    // ==========================================
    // 💫 變身與死亡動畫陣列
    // ==========================================
    std::vector<std::string> m_ChangeImages;
    std::vector<std::string> m_Star1_ChangeImages;
    std::vector<std::string> m_Star2_ChangeImages;
    std::vector<std::string> m_Star3_ChangeImages;
    std::vector<std::string> m_DeadImages; // 💀 死亡圖片

    std::vector<std::string>* m_CurrentStandImages = nullptr;
    std::vector<std::string>* m_CurrentRunImages = nullptr;
    std::vector<std::string>* m_CurrentJumpImages = nullptr;
    std::vector<std::string>* m_CurrentCrouchImages = nullptr;
    std::vector<std::string>* m_CurrentChangeImages = nullptr;
};

#endif // PLAYER_HPP