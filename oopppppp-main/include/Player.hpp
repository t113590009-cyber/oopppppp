#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp"
#include "Block.hpp"
#include <memory>
#include <vector>
#include <string>

enum class AnimState {
    IDLE,
    RUN,
    JUMP,
    CROUCHING,
    SPRINT,       // 🌟 新增：衝刺起步與打滑專用狀態
    CHANGING,
    WARP_DOWN_A,
    WARP_RIGHT_B,
    WARP_UP_OUT,
    DEAD,
    FLAG_SLIDE,
    AUTO_WALK,
};

class Player {
public:
    Player();
    void Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime);

    void GrowUp();
    void TakeDamage();
    void GetStar();
    void GetFireFlower();
    void Die();

    // 🌟 新增：瑪利歐踩到敵人時的彈跳動作
    void Bounce();

    void StartFlagSlide(float poleWorldX);
    bool IsFlagSliding() const { return m_CurrentState == AnimState::FLAG_SLIDE; }

    Rect GetRect(float worldOffset) const;
    Rect GetFeetRect(float worldOffset) const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Mario; }
    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }

    float GetVelocityY() const { return m_Velocity.y; }
    float GetVelocityX() const { return m_Velocity.x; }

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

    bool m_IsBig = false;
    bool m_IsFire = false;
    bool m_IsInvincible = false;
    float m_InvincibleTimer = 0.0f;
    float m_BlinkTimer = 0.0f;
    float m_ChangeTimer = 0.0f;
    bool m_IsOnGround = true;

    float m_DeathTimer = 0.0f;

    bool m_IsStarMode = false;
    float m_StarTimer = 0.0f;
    float m_StarAnimTimer = 0.0f;
    int m_StarColorIndex = 0;

    float m_RunAnimTimer = 0.0f;
    int m_RunFrameIndex = 0;

    // 🏃 原版物理與衝刺計時變數
    glm::vec2 m_Velocity = { 0.0f, 0.0f };
    bool m_IsSkidding = false;
    bool m_WasJumping = false;
    bool m_FacingRight = true;

    bool m_WasDashing = false;    // 🌟 紀錄前一幀是否按住衝刺
    float m_DashAnimTimer = 0.0f; // 🌟 衝刺初期的動畫計時器

    const float NES_SCALE = 180.0f;

    float m_WarpStartY = 0.0f;
    float m_WarpStartX = 0.0f;

    // --- 動畫資源路徑陣列 ---
    std::vector<std::string> m_SmallStandImages;
    std::vector<std::string> m_SmallRunImages;
    std::vector<std::string> m_SmallJumpImages;
    std::vector<std::string> m_SmallCrouchImages;
    std::vector<std::string> m_SmallSprintImages; // 🌟 新增

    std::vector<std::string> m_BigStandImages;
    std::vector<std::string> m_BigRunImages;
    std::vector<std::string> m_BigJumpImages;
    std::vector<std::string> m_BigCrouchImages;
    std::vector<std::string> m_BigSprintImages; // 🌟 新增

    std::vector<std::string> m_FireStandImages;
    std::vector<std::string> m_FireRunImages;
    std::vector<std::string> m_FireJumpImages;
    std::vector<std::string> m_FireCrouchImages;
    std::vector<std::string> m_FireSprintImages; // 🌟 新增

    // 星星狀態陣列
    std::vector<std::string> m_Star1_SmallStandImages; std::vector<std::string> m_Star1_SmallRunImages; std::vector<std::string> m_Star1_SmallJumpImages; std::vector<std::string> m_Star1_SmallCrouchImages; std::vector<std::string> m_Star1_SmallSprintImages;
    std::vector<std::string> m_Star1_BigStandImages; std::vector<std::string> m_Star1_BigRunImages; std::vector<std::string> m_Star1_BigJumpImages; std::vector<std::string> m_Star1_BigCrouchImages; std::vector<std::string> m_Star1_BigSprintImages;

    std::vector<std::string> m_Star2_SmallStandImages; std::vector<std::string> m_Star2_SmallRunImages; std::vector<std::string> m_Star2_SmallJumpImages; std::vector<std::string> m_Star2_SmallCrouchImages; std::vector<std::string> m_Star2_SmallSprintImages;
    std::vector<std::string> m_Star2_BigStandImages; std::vector<std::string> m_Star2_BigRunImages; std::vector<std::string> m_Star2_BigJumpImages; std::vector<std::string> m_Star2_BigCrouchImages; std::vector<std::string> m_Star2_BigSprintImages;

    std::vector<std::string> m_Star3_SmallStandImages; std::vector<std::string> m_Star3_SmallRunImages; std::vector<std::string> m_Star3_SmallJumpImages; std::vector<std::string> m_Star3_SmallCrouchImages; std::vector<std::string> m_Star3_SmallSprintImages;
    std::vector<std::string> m_Star3_BigStandImages; std::vector<std::string> m_Star3_BigRunImages; std::vector<std::string> m_Star3_BigJumpImages; std::vector<std::string> m_Star3_BigCrouchImages; std::vector<std::string> m_Star3_BigSprintImages;

    std::vector<std::string> m_ChangeImages;
    std::vector<std::string> m_Star1_ChangeImages;
    std::vector<std::string> m_Star2_ChangeImages;
    std::vector<std::string> m_Star3_ChangeImages;
    std::vector<std::string> m_DeadImages;

    std::vector<std::string>* m_CurrentStandImages = nullptr;
    std::vector<std::string>* m_CurrentRunImages = nullptr;
    std::vector<std::string>* m_CurrentJumpImages = nullptr;
    std::vector<std::string>* m_CurrentCrouchImages = nullptr;
    std::vector<std::string>* m_CurrentSprintImages = nullptr; // 🌟 新增指標
    std::vector<std::string>* m_CurrentChangeImages = nullptr;
};

#endif // PLAYER_HPP