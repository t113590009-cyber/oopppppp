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
    DEAD,         // 💀 死亡狀態
    FLAG_SLIDE,   // 🚩 抓著旗桿往下滑
    AUTO_WALK,    // 🚩 落地後自動走向城堡
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
    void GetFireFlower();
    void Die();

    // 過關旗桿邏輯
    void StartFlagSlide(float poleWorldX);
    bool IsFlagSliding() const { return m_CurrentState == AnimState::FLAG_SLIDE; }

    // 碰撞箱取得
    Rect GetRect(float worldOffset) const;
    Rect GetFeetRect(float worldOffset) const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Mario; }
    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }

    // 🌟 原版物理系統需要提供雙軸速度
    float GetVelocityY() const { return m_Velocity.y; }
    float GetVelocityX() const { return m_Velocity.x; }

    // 狀態讀取
    bool IsStarMode() const { return m_IsStarMode; }
    bool IsOnGround() const { return m_IsOnGround; }
    bool IsDead() const { return m_CurrentState == AnimState::DEAD; }

    void ResetStatus();

    bool IsBig() const { return m_IsBig; }
    bool IsFire() const { return m_IsFire; }

    void SetState(AnimState state);
    AnimState GetState() const { return m_CurrentState; }
    void SetWorldPosition(float worldX, float y);
    float GetWorldX(float worldOffset) const;

private:
    void RefreshAnimations();

    std::shared_ptr<AnimatedCharacter> m_Mario;
    AnimState m_CurrentState = AnimState::IDLE;

    // --- 狀態變數 ---
    bool m_IsBig = false;
    bool m_IsFire = false;
    bool m_IsInvincible = false;
    float m_InvincibleTimer = 0.0f;
    float m_BlinkTimer = 0.0f;
    float m_ChangeTimer = 0.0f;
    bool m_IsOnGround = true;

    // 瑪利歐專屬的死亡計時器
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

    // ==========================================
    // 🏃 原版 NES 物理狀態與變數 (新增/改寫)
    // ==========================================
    glm::vec2 m_Velocity = { 0.0f, 0.0f }; // X和Y的速度

    bool m_IsSkidding = false; // 是否正在煞車打滑
    bool m_WasJumping = false; // 紀錄前一幀是否按住跳躍 (用來控制動態重力)
    bool m_FacingRight = true; // 面向右邊

    // 將 NES 的 "像素/幀" 轉換為我們引擎的 "像素/秒" 
    // 假設 60 FPS 且圖片放大 3 倍，係數約為 180.0f
    const float NES_SCALE = 180.0f;

    // 原本簡單的 m_WalkSpeed, m_JumpImpulse, m_Gravity 被淘汰了！

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

    // 🔥 火球瑪利歐專屬動畫陣列
    std::vector<std::string> m_FireStandImages;
    std::vector<std::string> m_FireRunImages;
    std::vector<std::string> m_FireJumpImages;
    std::vector<std::string> m_FireCrouchImages;

    // 星星狀態陣列
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
    std::vector<std::string> m_DeadImages;

    std::vector<std::string>* m_CurrentStandImages = nullptr;
    std::vector<std::string>* m_CurrentRunImages = nullptr;
    std::vector<std::string>* m_CurrentJumpImages = nullptr;
    std::vector<std::string>* m_CurrentCrouchImages = nullptr;
    std::vector<std::string>* m_CurrentChangeImages = nullptr;
};

#endif // PLAYER_HPP