#include "MovingBlock.hpp"
#include "Util/Image.hpp" 

// ==========================================
// 🏗️ 建構子：初始化移動平台
// ==========================================
MovingBlock::MovingBlock(const std::string& imagePath, float startX, float startY, Axis axis, float range, float speed)
    : Block(Block::Type::BRICK_FRAGILE, glm::vec2{ startX, startY }),
    m_Axis(axis), m_Range(range), m_Speed(speed), m_StartX(startX), m_StartY(startY) {

    if (GetCharacter()) {
        std::vector<std::string> img = { imagePath };
        GetCharacter()->SetAnimation(img);
        GetCharacter()->SetZIndex(5);
    }

    m_Direction = 1;
    m_VelocityX = 0.0f;
    m_VelocityY = 0.0f;
}

// ==========================================
// 🔄 核心更新：處理平台的來回移動慣性
// ==========================================
void MovingBlock::Update(float dt, float worldOffset) {
    if (!GetCharacter()) return;

    if (m_Axis == Axis::HORIZONTAL) {
        m_VelocityX = m_Speed * m_Direction;
        m_VelocityY = 0.0f;
        m_CurrentPos.x += m_VelocityX * dt;

        if (m_Direction > 0 && m_CurrentPos.x >= m_StartX + m_Range) {
            m_CurrentPos.x = m_StartX + m_Range;
            m_Direction = -1;
        }
        else if (m_Direction < 0 && m_CurrentPos.x <= m_StartX - m_Range) {
            m_CurrentPos.x = m_StartX - m_Range;
            m_Direction = 1;
        }
    }
    else if (m_Axis == Axis::VERTICAL) {
        m_VelocityX = 0.0f;
        m_VelocityY = m_Speed * m_Direction;
        m_CurrentPos.y += m_VelocityY * dt;

        if (m_Direction > 0 && m_CurrentPos.y >= m_StartY + m_Range) {
            m_CurrentPos.y = m_StartY + m_Range;
            m_Direction = -1;
        }
        else if (m_Direction < 0 && m_CurrentPos.y <= m_StartY - m_Range) {
            m_CurrentPos.y = m_StartY - m_Range;
            m_Direction = 1;
        }
    }

    m_OriginalPos = m_CurrentPos;

    GetCharacter()->SetPosition({ m_CurrentPos.x - worldOffset, m_CurrentPos.y });
}

// ==========================================
// 🌟 修正 3：解決「被頂飛」
// ==========================================
void MovingBlock::Hit(bool isBigMario) {
    // 裡面什麼都不寫，瑪利歐頂到它就只會發出咚一聲，平台不會往上彈！
}

// ==========================================
// 🌟 修正 4：量身打造 144x24 的精準碰撞箱
// ==========================================
Rect MovingBlock::GetHitbox() const {
    float width = 144.0f;
    float height = 24.0f;

    // X 座標：從中心點往左退一半 (144 / 2 = 72)
    float startX = m_CurrentPos.x - 72.0f;

    // Y 座標：從中心點往下退一半 (24 / 2 = 12)
    // 💡 小秘訣：如果你測試後覺得瑪利歐的腳底「陷進圖片裡」了，可以把 -12.0f 稍微調高（例如改成 -4.0f 或 0.0f）
    // 反之，如果覺得瑪利歐「浮在半空中」，就把數字往下調（例如 -16.0f）
    float startY = m_CurrentPos.y - 12.0f;

    return { startX, startY, width, height };
}