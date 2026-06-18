#include "HammerProp.hpp"
#include "config.hpp"

HammerProp::HammerProp(float worldX, float worldY) : m_WorldX(worldX), m_WorldY(worldY) {
    m_ImagePaths = {
        "C:\\Users\\User\\Downloads\\oopppppp\\oopppppp-main\\Resources\\Image\\Props\\1-4\\hammer1.png",
        "C:\\Users\\User\\Downloads\\oopppppp\\oopppppp-main\\Resources\\Image\\Props\\1-4\\hammer2.png",
        "C:\\Users\\User\\Downloads\\oopppppp\\oopppppp-main\\Resources\\Image\\Props\\1-4\\hammer3.png",
        "C:\\Users\\User\\Downloads\\oopppppp\\oopppppp-main\\Resources\\Image\\Props\\1-4\\hammer4.png"
    };

    m_Animation = std::make_shared<AnimatedCharacter>(std::vector<std::string>{m_ImagePaths[0]});
    m_Animation->SetZIndex(5);
    m_Animation->SetPosition({worldX, worldY});
    m_Animation->m_Transform.scale = { NES_SCALE * 0.5f, NES_SCALE * 0.5f };
}

void HammerProp::Update(float deltaTime) {
    if (m_IsDestroyed) return;

    // 🌟 只處理動畫，不干涉位置
    m_AnimTimer += deltaTime;
    if (m_AnimTimer >= m_FrameDuration) {
        m_AnimTimer = 0.0f;
        m_FrameIndex = (m_FrameIndex + 1) % m_ImagePaths.size();
        m_Animation->SetDrawable(std::make_shared<Util::Image>(m_ImagePaths[m_FrameIndex]));
    }
}

// 🌟 這個新函式用來修正鏡頭！這就是為什麼不用大改 AppUpdate 的關鍵
void HammerProp::RenderWithCamera(float worldOffset) {
    if (m_IsDestroyed || !m_Animation) return;

    // 讓鐵鎚隨著地圖滾動
    m_Animation->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

HammerRect HammerProp::GetHitbox() const {
    float size = 16.0f * NES_SCALE;
    return HammerRect{ m_WorldX, m_WorldY, size, size };
}

void HammerProp::Touch() {
    m_IsDestroyed = true;
    if (m_Animation) {
        m_Animation->SetVisible(false); // 摸到就隱形不見（道具箱被觸發）
    }
}