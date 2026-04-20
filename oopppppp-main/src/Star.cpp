#include "Star.hpp"

Star::Star(float spawnWorldX, float spawnWorldY)
    : Item({
        GA_RESOURCE_DIR"/Image/Props/star/Star1.png",
        GA_RESOURCE_DIR"/Image/Props/star/Star2.png",
        GA_RESOURCE_DIR"/Image/Props/star/Star3.png",
        GA_RESOURCE_DIR"/Image/Props/star/Star4.png"
        })
{
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;
    m_SpawnStartY = spawnWorldY;

    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(3); // 初始在方塊下方
}

void Star::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_IsDestroyed) return;

    // --- 1. 鑽出方塊階段 (SPAWNING) ---
    if (m_State == State::SPAWNING) {
        m_WorldY += 40.0f * deltaTime; // 往上鑽
        if (m_WorldY >= m_SpawnStartY + 48.0f) {
            m_WorldY = m_SpawnStartY + 48.0f;
            m_State = State::MOVING; // 鑽完開始移動
            m_VelocityY = BOUNCE_FORCE; // 🌟 鑽完馬上給一個初速向上彈跳！
            this->SetZIndex(5);      // 移動到方塊前方
        }
        this->SetPosition({ m_WorldX - worldOffset, m_WorldY });
        return;
    }

    // --- 2. 移動與彈跳階段 (MOVING) ---
    // (A) 水平移動與撞牆判定
    float nextX = m_WorldX + m_SpeedX;
    Rect nextXRect = { nextX - 22.0f, m_WorldY - 22.0f, 44.0f, 44.0f }; // 身體完整碰撞箱

    bool hitWall = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextXRect, obs)) {
            hitWall = true;
            break;
        }
    }
    if (hitWall) m_SpeedX = -m_SpeedX; // 撞牆反向
    else m_WorldX = nextX;

    // (B) 垂直移動與重力 (Bouncing Physics)
    // 🌟 核心修正：重力應該是「減少」Y 軸速度 (配合 Player 的 convention)
    m_VelocityY -= GRAVITY * (deltaTime * 60.0f);

    float nextY = m_WorldY + m_VelocityY;

    // 🌟 星星腳底碰撞箱 (做窄一點，專門偵測落實地)
    Rect nextYRect = { m_WorldX - 15.0f, nextY - 22.0f, 30.0f, 5.0f };

    bool onGround = false;

    // 只在往下掉的時候才偵測落地，避免起跳時卡住
    if (m_VelocityY <= 0.0f) {
        for (const auto& obs : obstacles) {
            if (CollisionHandler::CheckCollision(nextYRect, obs)) {
                // 將星星位置修正到障礙物上方 (22.0f 是半高)
                m_WorldY = obs.y + obs.height + 22.0f;

                // 🌟 星星彈跳：碰到地板就給予一個向上的初速 (正數)
                m_VelocityY = BOUNCE_FORCE;
                onGround = true;
                break;
            }
        }
    }

    if (!onGround) m_WorldY = nextY;

    // 掉下地圖就清除
    if (m_WorldY < -400.0f) Destroy();

    this->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}

void Star::ApplyEffect(Player* player) {
    if (!m_IsDestroyed) {
        player->GetStar(); // 🌟 呼叫玩家的無敵狀態
        Destroy();
        this->SetVisible(false);
    }
}

Rect Star::GetRect(float worldOffset) const {
    // 鑽出階段不給瑪利歐吃
    if (m_State == State::SPAWNING) {
        return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    }
    // 吃到判定的碰撞箱 (稍微做大一點點，比較好接住)
    return { m_WorldX - 20.0f, m_WorldY - 20.0f, 40.0f, 40.0f };
}