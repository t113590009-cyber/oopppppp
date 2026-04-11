#include "Mushroom.hpp"

Mushroom::Mushroom(float spawnWorldX, float spawnWorldY)
    : Item({ GA_RESOURCE_DIR"/Image/Props/toad.png" })
{
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;
    m_SpawnStartY = spawnWorldY;

    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(3);
}

void Mushroom::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_IsDestroyed) return;

    // ==========================================
    // 🍄 狀態 A：從方塊中緩慢鑽出
    // ==========================================
    if (m_State == State::SPAWNING) {
        m_WorldY += 30.0f * deltaTime;

        // 升起大約 48 像素才算完全出土
        if (m_WorldY >= m_SpawnStartY + 48.0f) {
            m_WorldY = m_SpawnStartY + 48.0f;
            m_State = State::MOVING;
            this->SetZIndex(5);
        }

        this->SetPosition({ m_WorldX - worldOffset, m_WorldY });
        return;
    }

    // ==========================================
    // 🍄 狀態 B：正常的水平移動與重力 
    // ==========================================
    float nextX = m_WorldX + m_SpeedX;

    // 🌟 關鍵修正 1：物理判定必須用「世界座標 (World)」，絕對不能減去 worldOffset！
    // 這樣蘑菇才能看見天空中的磚塊
    Rect nextXRect = { nextX - 22.0f, m_WorldY - 20.0f, 44.0f, 40.0f };

    bool hitWall = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextXRect, obs)) {
            hitWall = true;
            break;
        }
    }

    if (hitWall) m_SpeedX = -m_SpeedX;
    else m_WorldX = nextX;

    m_VelocityY += GRAVITY * deltaTime;
    float nextY = m_WorldY + m_VelocityY;

    // 🌟 關鍵修正 2：垂直碰撞也必須用「世界座標」！
    Rect nextYRect = { m_WorldX - 15.0f, nextY - 24.0f, 30.0f, 10.0f };

    bool onGround = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextYRect, obs)) {
            m_WorldY = obs.y + obs.height + 24.0f;
            m_VelocityY = 0.0f;
            onGround = true;
            break;
        }
    }

    if (!onGround) m_WorldY = nextY;
    if (m_WorldY < -400.0f) Destroy();

    // 🌟 只有在最後要「畫在螢幕上」時，才需要減去 worldOffset
    this->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Mushroom::ApplyEffect(Player* player) {
    if (!m_IsDestroyed) {
        player->GrowUp();
        Destroy();
        this->SetVisible(false);
    }
}

Rect Mushroom::GetRect(float worldOffset) const {
    // 🌟 關鍵修正 3：還沒鑽出來時，把碰撞箱丟到地心，防止瑪利歐一頭撞上去就直接吃掉
    if (m_State == State::SPAWNING) {
        return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    }

    // 🌟 關鍵修正 4：回傳給 AppUpdate 的碰撞箱必須是「世界座標」
    // 因為瑪利歐的 GetFeetRect 也是世界座標，這樣才能完美對接判定吃香菇！
    Rect result;
    result.x = m_WorldX - 22.0f;
    result.y = m_WorldY - 22.0f;
    result.width = 44.0f;
    result.height = 44.0f;
    return result;
}