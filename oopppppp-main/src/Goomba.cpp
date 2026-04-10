#include "Goomba.hpp"

// 🌟 修正 C2511: 確保與標頭檔的 Goomba(float) 完全對應
Goomba::Goomba(float spawnWorldX) : m_WorldX(spawnWorldX) {
    m_Drawable = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/goomba/goomba1.png");
    m_Drawable->m_Transform.scale = { 3.0f, 3.0f };
    m_Drawable->SetZIndex(5);
}

void Goomba::Update(float deltaTime, float worldOffset) {
    if (m_State == State::WALKING) {
        m_WorldX -= m_Speed;
        m_AnimTimer += deltaTime;
        if (m_AnimTimer >= 0.2f) {
            m_CurrentFrame = (m_CurrentFrame == 1) ? 2 : 1;
            m_Drawable->SetImage(GA_RESOURCE_DIR"/Image/Character/goomba/goomba" + std::to_string(m_CurrentFrame) + ".png");
            m_AnimTimer = 0.0f;
        }
    }
    else if (m_State == State::STOMPED) {
        m_DeathTimer += deltaTime;
        if (m_DeathTimer >= 0.5f) {
            m_State = State::DEAD;
            m_Drawable->SetVisible(false);
        }
    }
    m_Drawable->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Goomba::Stomp() {
    if (m_State == State::WALKING) {
        m_State = State::STOMPED;
        m_Drawable->SetImage(GA_RESOURCE_DIR"/Image/Character/goomba/goombadie.png");
    }
}

// 🌟 修正 C2270: 確保這行跟標頭檔一樣有 const，且屬於 Goomba::
Rect Goomba::GetRect(float worldOffset) const {
    Rect result;
    result.x = m_WorldX - worldOffset - 24.0f;
    result.y = m_WorldY - 24.0f;
    result.width = 48.0f;
    result.height = 48.0f;
    return result;
}