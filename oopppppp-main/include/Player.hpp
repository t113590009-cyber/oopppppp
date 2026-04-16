#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp"
#include "Block.hpp"
#include <memory>
#include <vector>
#include <string>
#include <glm/vec2.hpp> // 來自第一份：確保 vec2 正常運作

// 🌟 定義瑪利歐的所有動作狀態 (結合第一份的 DEAD 與第二份的變身 CHANGING)
enum class AnimState {
    IDLE,
    RUN,
    JUMP,
    CROUCHING,
    CHANGING,     // 變身/無敵暫停狀態
    WARP_DOWN_A,  // 進水管 (下)
    WARP_RIGHT_B, // 進水管 (右)
    WARP_UP_OUT,  // 出水管 (上)
    DEAD          // 💀 新增：死亡狀態 (來自第一份)
};

class Player {
public:
    Player();

    // 核心更新邏輯
    void Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime);

    // 道具與受傷邏輯 (來自第二份)
    void GrowUp();
    void TakeDamage();
    void GetStar(); // 🌟 吃到無敵星星！

    // 💀 死亡邏輯 (來自第一份)
    void Die();
    bool IsDead() const { return m_CurrentState == AnimState::DEAD; }

    // 取得瑪利歐資訊給外部 (如 AppUpdate) 使用
    Rect GetFeetRect(float worldOffset) const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Mario; }

    // 座標與顯示設定 (來自第一份，App 會頻繁呼叫)
    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }
    void SetPosition(const glm::vec2& pos) { m_Mario->SetPosition(pos); }
    void SetVisible(bool visible) { m_Mario->SetVisible(visible); }

    float GetVelocityY() const { return m_Velocity.y; }

    // 🌟 讓外部 (如栗子球碰撞判定) 知道瑪利歐現在是不是無敵星狀態
    bool IsStarMode() const { return m_IsStarMode; }

private:
    void RefreshAnimations(); // 🌟 更新動畫指標的輔助函式

    std::shared_ptr<AnimatedCharacter> m_Mario;
    AnimState m_CurrentState = AnimState::IDLE;

    // --- 狀態變數 ---
    bool m_IsBig = false;
    bool m_IsInvincible = false;
    float m_InvincibleTimer = 0.0f;
    float m_BlinkTimer = 0.0f;
    float m_ChangeTimer = 0.0f;
    bool m_IsOnGround = true;

    // 💀 死亡動畫相關計時器 (來自第一份)
    float m_DeathTimer = 0.0f;

    // ==========================================
    // 🌟 無敵星星專屬變數
    // ==========================================
    bool m_IsStarMode = false;
    float m_StarTimer = 0.0f;         // 記錄無敵總時間 (通常是 10 秒)
    float m_StarAnimTimer = 0.0f;     // 控制閃爍切換的速度
    int m_StarColorIndex = 0;         // 顏色狀態：0=正常, 1=star1, 2=star2, 3=star3

    // 🌟 手動控制跑步動畫的計時器與幀數 (解決星星閃爍重置問題)
    float m_RunAnimTimer = 0.0f;
    int m_RunFrameIndex = 0;

    // --- 物理與位置變數 ---
    glm::vec2 m_Velocity = { 0.0f, 0.0f };

    // 🌟 更新物理常數 (以第二份為主，手感更好)
    float m_WalkSpeed = 5.5f;
    float m_JumpImpulse = 24.0f;
    float m_Gravity = 1.0f;

    float m_WarpStartY = 0.0f;
    float m_WarpStartX = 0.0f;

    // --- 動畫資源路徑陣列 ---
    std::vector<std::string> m_SmallStandImages;
    std::vector<std::string> m_SmallRunImages;
    std::vector<std::string> m_SmallJumpImages;
    std::vector<std::string> m_SmallCrouchImages; // 小隻蹲下 (其實是站立圖)

    std::vector<std::string> m_BigStandImages;
    std::vector<std::string> m_BigRunImages;
    std::vector<std::string> m_BigJumpImages;
    std::vector<std::string> m_BigCrouchImages;   // 大隻專屬蹲下圖

    // 💀 死亡圖片陣列 (來自第一份)
    std::vector<std::string> m_DeadImages;

    // 🌟 星星 1 陣列
    std::vector<std::string> m_Star1_SmallStandImages;
    std::vector<std::string> m_Star1_SmallRunImages;
    std::vector<std::string> m_Star1_SmallJumpImages;
    std::vector<std::string> m_Star1_SmallCrouchImages;
    std::vector<std::string> m_Star1_BigStandImages;
    std::vector<std::string> m_Star1_BigRunImages;
    std::vector<std::string> m_Star1_BigJumpImages;
    std::vector<std::string> m_Star1_BigCrouchImages;

    // 🌟 星星 2 陣列
    std::vector<std::string> m_Star2_SmallStandImages;
    std::vector<std::string> m_Star2_SmallRunImages;
    std::vector<std::string> m_Star2_SmallJumpImages;
    std::vector<std::string> m_Star2_SmallCrouchImages;
    std::vector<std::string> m_Star2_BigStandImages;
    std::vector<std::string> m_Star2_BigRunImages;
    std::vector<std::string> m_Star2_BigJumpImages;
    std::vector<std::string> m_Star2_BigCrouchImages;

    // 🌟 星星 3 陣列
    std::vector<std::string> m_Star3_SmallStandImages;
    std::vector<std::string> m_Star3_SmallRunImages;
    std::vector<std::string> m_Star3_SmallJumpImages;
    std::vector<std::string> m_Star3_SmallCrouchImages;
    std::vector<std::string> m_Star3_BigStandImages;
    std::vector<std::string> m_Star3_BigRunImages;
    std::vector<std::string> m_Star3_BigJumpImages;
    std::vector<std::string> m_Star3_BigCrouchImages;

    // ==========================================
    // 💫 變身動畫陣列
    // ==========================================
    std::vector<std::string> m_ChangeImages;              // 一般變身閃爍圖
    std::vector<std::string> m_Star1_ChangeImages;        // 🌟 星星1變身圖
    std::vector<std::string> m_Star2_ChangeImages;        // 🌟 星星2變身圖
    std::vector<std::string> m_Star3_ChangeImages;        // 🌟 星星3變身圖

    // --- 當前使用的動畫指標 (動態切換) ---
    std::vector<std::string>* m_CurrentStandImages = nullptr;
    std::vector<std::string>* m_CurrentRunImages = nullptr;
    std::vector<std::string>* m_CurrentJumpImages = nullptr;
    std::vector<std::string>* m_CurrentCrouchImages = nullptr;
    std::vector<std::string>* m_CurrentChangeImages = nullptr;
};

#endif // PLAYER_HPP