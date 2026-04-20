#include "Coin.hpp"
#include "Util/Logger.hpp"

Coin::Coin(float startWorldX, float startWorldY, int type)
    : Item({}), m_Type(type)
{
    m_WorldX = startWorldX;
    m_WorldY = startWorldY;
    m_SpawnStartY = startWorldY;

    if (m_Type == 0) {
        // 🌟 方塊彈出的金幣 (4張圖)
        m_Images = {
            GA_RESOURCE_DIR"/Image/Items/coin/box/coin1.png",
            GA_RESOURCE_DIR"/Image/Items/coin/box/coin2.png",
            GA_RESOURCE_DIR"/Image/Items/coin/box/coin3.png", // 👈 確保檔名已改對！
            GA_RESOURCE_DIR"/Image/Items/coin/box/coin4.png"
        };
        m_VelocityY = 12.0f; // 賦予往上衝的初速
        this->SetZIndex(4);  // 在方塊後方彈出
    }
    else {
        // 🌟 地圖懸浮的金幣 (3張圖)
        m_Images = {
            GA_RESOURCE_DIR"/Image/Items/coin/underground/coin1.png",
            GA_RESOURCE_DIR"/Image/Items/coin/underground/coin2.png",
            GA_RESOURCE_DIR"/Image/Items/coin/underground/coin3.png"
        };
        this->SetZIndex(5);
    }

    // 初始化第一張圖
    this->m_Drawable = std::make_shared<Util::Image>(m_Images[0]);
    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetPosition({ m_WorldX, m_WorldY });
}

void Coin::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_IsDestroyed) return;

    // --- 1. 金幣旋轉動畫 ---
    m_AnimTimer += deltaTime;
    if (m_AnimTimer >= 0.1f) { // 每 0.1 秒換一張圖
        m_AnimTimer = 0.0f;
        m_AnimFrame = (m_AnimFrame + 1) % m_Images.size();
        std::dynamic_pointer_cast<Util::Image>(m_Drawable)->SetImage(m_Images[m_AnimFrame]);
    }

    // --- 2. 物理邏輯 ---
    if (m_Type == 0) {
        // 🌟 方塊金幣：會受重力影響掉下來
        m_VelocityY -= 0.8f * (deltaTime * 60.0f);
        m_WorldY += m_VelocityY;

        // 當它掉回接近方塊頂端的位置時，就自我毀滅
        if (m_VelocityY < 0.0f && m_WorldY <= m_SpawnStartY + 48.0f) {
            Destroy();
            this->SetVisible(false);
            // TODO: 之後在這裡生成「飄浮的 200 分」物件
        }
    }

    this->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Coin::ApplyEffect(Player* player) {
    if (m_IsDestroyed) return;

    // 🌟 只有「地圖懸浮金幣」才需要被玩家實體觸碰吃掉
    if (m_Type == 1) {
        // player->AddCoin(); // TODO: 之後在 Player 加上金幣計數器
        Destroy();
        this->SetVisible(false);
    }
}

Rect Coin::GetRect(float worldOffset) const {
    // 方塊金幣 (Type 0) 是不能用身體吃的，所以回傳空碰撞箱
    if (m_IsDestroyed || m_Type == 0) {
        return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    }
    // 地圖金幣 (Type 1) 的碰撞箱
    return { m_WorldX - 15.0f, m_WorldY - 24.0f, 30.0f, 48.0f };
}