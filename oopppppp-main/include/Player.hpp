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
    WARP_UP_OUT   // 出水管 (上)
};

class Player {
public:
    Player();

    // 核心更新邏輯
    void Update(float& worldOffset, const CollisionHandler& collision, std::vector<std::shared_ptr<Block>>& blocks, float deltaTime);

    // 道具與受傷邏輯
    void GrowUp();
    void TakeDamage();
    void GetStar(); // 🌟 吃到無敵星星！

    // 取得瑪利歐資訊給外部 (如 AppUpdate) 使用
    Rect GetFeetRect(float worldOffset) const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Mario; }
    glm::vec2 GetPosition() const { return m_Mario->GetPosition(); }
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

    // ==========================================
    // 🌟 無敵星星專屬變數
    // ==========================================
    bool m_IsStarMode = false;
    float m_StarTimer = 0.0f;         // 記錄無敵總時間 (通常是 10 秒)
    float m_StarAnimTimer = 0.0f;     // 控制閃爍切換的速度
    int m_StarColorIndex = 0;         // 顏色狀態：0=正常, 1=star1, 2=star2, 3=star3

    // 🌟 新增：手動控制跑步動畫的計時器與幀數 (解決星星閃爍重置問題)
    float m_RunAnimTimer = 0.0f;
    int m_RunFrameIndex = 0;

    // --- 物理與位置變數 ---
    glm::vec2 m_Velocity = { 0.0f, 0.0f };

    // 🌟 加速你的瑪利歐！(原本 3.0f)
    float m_WalkSpeed = 5.5f;

    // 🌟 增強大腿肌力！(原本 10.0f，為了抵抗更大的重力，起跳要給多一點)
    float m_JumpImpulse = 24.0f;

    // 🌟 回歸地球重力！(原本 0.4f，改大後掉下來會非常俐落，不會飄在空中)
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
    std::vector<std::string>* m_CurrentChangeImages = nullptr; // 🌟 負責切換一般變身還是彩色變身
};

#endif // PLAYER_HPP