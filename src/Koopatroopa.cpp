#include "Koopatroopa.hpp"
#include "Util/Logger.hpp"

Koopatroopa::Koopatroopa(float spawnWorldX, float spawnWorldY) {
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;

    m_WalkImages = {
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle.png",
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle2.png"
    };
    m_DieImage = GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtledie.png";

    this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[0]));
    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(4);
}

void Koopatroopa::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_State == State::DEAD) {
        m_DeadTimer += deltaTime;
        if (m_DeadTimer > 1.5f) {
            this->SetVisible(false);
        }
        this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY }; // 修正點 1
        return;
    }

    m_AnimTimer += deltaTime;
    if (m_AnimTimer >= 0.2f) {
        m_AnimTimer = 0.0f;
        m_AnimFrame = (m_AnimFrame + 1) % 2;
        this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[m_AnimFrame]));
    }

    float nextX = m_WorldX + (m_SpeedX * deltaTime * 60.0f);
    Rect nextXRect = { nextX - 20.0f, m_WorldY - 20.0f, 40.0f, 40.0f };

    bool hitWall = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextXRect, obs)) {
            hitWall = true;
            break;
        }
    }

    if (hitWall) {
        m_SpeedX = -m_SpeedX;
        this->m_Transform.scale.x = (m_SpeedX > 0) ? -3.0f : 3.0f;
    } else {
        m_WorldX = nextX;
    }

    m_VelocityY -= GRAVITY * deltaTime;
    float nextY = m_WorldY + m_VelocityY;
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

    if (m_WorldY < -400.0f) {
        m_State = State::DEAD;
        this->SetVisible(false);
    }

    this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY +10.0f }; // 修正點 2
}

void Koopatroopa::Interact(Player* player, float worldOffset) {
    if (m_State == State::DEAD) return; // 修正點 3 (移除了 GetVisible)

    Rect koopaRect = GetRect(worldOffset);
    Rect playerBody = {
        worldOffset + player->GetPosition().x - 18.0f,
        player->GetPosition().y - 20.0f,
        36.0f, 40.0f
    };

    if (!CollisionHandler::CheckCollision(koopaRect, playerBody)) return;

    // 🌟 判定：無敵星星模式 (使用 IsStarMode 方法避開 private 限制)
    if (player->IsStarMode()) {
        m_State = State::DEAD;
        this->SetDrawable(std::make_shared<Util::Image>(m_DieImage));
        return;
    }

    Rect playerFeet = player->GetFeetRect(worldOffset);

    // 🌟 判定：從上方踩踏 (使用 GetVelocityY 避開 private 限制)
    if (player->GetVelocityY() < 0.0f && playerFeet.y > koopaRect.y + 10.0f) {
        m_State = State::DEAD;
        this->SetDrawable(std::make_shared<Util::Image>(m_DieImage));
        // 注意：這裡如果 m_Velocity 還是報錯，要在 Player 加入一個 SetVelocityY(float)
        // 或者暫時註解掉這行測試其他功能
    }
    else {
        // 🌟 判定：撞到瑪利歐
        player->TakeDamage();
    }
}

Rect Koopatroopa::GetRect(float worldOffset) const {
    if (m_State == State::DEAD) {
        return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    }
    return { m_WorldX - 22.0f, m_WorldY - 22.0f, 44.0f, 44.0f };
}