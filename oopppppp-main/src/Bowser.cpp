#include "Bowser.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"

Bowser::Bowser(float spawnWorldX, float spawnWorldY)
    : m_WorldX(spawnWorldX), m_WorldY(spawnWorldY) {
    this->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/Props/Koopa.png"));
    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(4);
}

std::shared_ptr<BowserFire> Bowser::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_State == State::DEAD) {
        m_VelocityY -= GRAVITY * deltaTime;
        m_WorldY += m_VelocityY;
        this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY };
        if (m_WorldY < -500.0f) this->SetVisible(false);
        return nullptr;
    }

    std::shared_ptr<BowserFire> spawnedFire = nullptr;

    m_WalkTimer -= deltaTime;
    if (m_WalkTimer <= 0.0f) {
        m_SpeedX = -m_SpeedX;
        m_WalkTimer = 1.5f;
    }

    m_JumpTimer -= deltaTime;
    if (m_JumpTimer <= 0.0f && m_VelocityY == 0.0f) {
        m_VelocityY = 12.0f;
        m_JumpTimer = 3.0f;
    }

    m_FireTimer -= deltaTime;
    if (m_FireTimer <= 0.0f) {
        spawnedFire = std::make_shared<BowserFire>(m_WorldX - 45.0f, m_WorldY + 5.0f, true);
        m_FireTimer = 4.0f;
    }

    float nextX = m_WorldX + m_SpeedX * deltaTime;
    Rect nextXRect = { nextX - 30.0f, m_WorldY - 30.0f, 60.0f, 60.0f };

    bool hitWall = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextXRect, obs)) { hitWall = true; break; }
    }
    if (hitWall) m_SpeedX = -m_SpeedX;
    else m_WorldX = nextX;

    m_VelocityY -= GRAVITY * deltaTime;
    float nextY = m_WorldY + m_VelocityY;
    Rect nextYRect = { m_WorldX - 25.0f, nextY - 48.0f, 50.0f, 10.0f };

    bool onGround = false;
    for (const auto& obs : obstacles) {
        if (CollisionHandler::CheckCollision(nextYRect, obs)) {
            m_WorldY = obs.y + obs.height + 48.0f;
            m_VelocityY = 0.0f;
            onGround = true;
            break;
        }
    }
    if (!onGround) m_WorldY = nextY;

    this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY };
    this->m_Transform.scale.x = 3.0f;

    return spawnedFire;
}

void Bowser::Interact(Player* player, float worldOffset) {
    if (m_State == State::DEAD || !player || player->IsDead()) return;

    Rect bowserRect = GetRect(worldOffset);
    Rect playerBody = { worldOffset + player->GetPosition().x - 18.0f, player->GetPosition().y - 20.0f, 36.0f, 40.0f };

    if (CollisionHandler::CheckCollision(bowserRect, playerBody)) {
        if (player->IsStarMode()) {
            TakeDamage(999);
        }
        else {
            player->TakeDamage();
        }
    }
}

Rect Bowser::GetRect(float worldOffset) const {
    if (m_State == State::DEAD) return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    return { m_WorldX - 30.0f, m_WorldY - 30.0f, 60.0f, 60.0f };
}

void Bowser::TakeDamage(int damage) {
    m_HP -= damage;
    if (m_HP <= 0) {
        m_State = State::DEAD;
        m_VelocityY = 15.0f;
        this->m_Transform.scale.y = 3.0f;
        this->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/Props/koopadie.png"));
    }
}