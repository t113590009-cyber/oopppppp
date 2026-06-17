#include "Goomba.hpp"

Goomba::Goomba(float spawnWorldX, float spawnWorldY, bool moveLeft) {
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;

    m_VelocityX = moveLeft ? -1.45f : 1.45f;

    m_Drawable = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/goomba/goomba1.png");
    m_Drawable->m_Transform.scale = { 3.0f, 3.0f };
    m_Drawable->SetZIndex(5);
}

void Goomba::Update(float deltaTime, float worldOffset, const CollisionHandler& collision) {
    if (m_State == State::FLIPPED) {
        m_VelocityY -= 0.8f * (deltaTime * 60.0f);
        m_WorldY += m_VelocityY;

        if (m_WorldY < -400.0f) {
            m_State = State::DEAD;
            m_Drawable->SetVisible(false);
        }
        m_Drawable->SetPosition({ m_WorldX - worldOffset, m_WorldY });
        return;
    }

    if (m_State == State::WALKING) {
        float nextX = m_WorldX + m_VelocityX;
        Rect nextRect = { nextX - 24.0f, m_WorldY - 24.0f, 48.0f, 48.0f };
        bool hitWall = false;

        for (const auto& obs : collision.GetObstacles()) {
            if (CollisionHandler::CheckCollision(nextRect, obs)) {
                if (obs.y + obs.height > nextRect.y + 5.0f) {
                    float goombaCenter = nextX;
                    float obsCenter = obs.x + (obs.width / 2.0f);

                    if (m_VelocityX < 0 && obsCenter < goombaCenter) {
                        hitWall = true; break;
                    }
                    else if (m_VelocityX > 0 && obsCenter > goombaCenter) {
                        hitWall = true; break;
                    }
                }
            }
        }

        if (hitWall) {
            m_VelocityX = -m_VelocityX;
        }
        else {
            m_WorldX = nextX;
        }

        Rect goombaFeet = { m_WorldX - 15.0f, m_WorldY - 25.0f, 30.0f, 5.0f };
        float groundHeight = collision.GetGroundHeight(goombaFeet, -400.0f);

        if (!m_IsOnGround) {
            m_VelocityY -= 0.8f * (deltaTime * 60.0f);
        }

        m_WorldY += m_VelocityY;

        if (m_WorldY <= groundHeight + 24.0f) {
            m_WorldY = groundHeight + 24.0f;
            m_VelocityY = 0.0f;
            m_IsOnGround = true;
        }
        else {
            m_IsOnGround = false;
        }

        if (m_WorldY < -360.0f) {
            m_State = State::DEAD;
            m_Drawable->SetVisible(false);
            return;
        }

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

void Goomba::FlipDie() {
    if (m_State == State::WALKING) {
        m_State = State::FLIPPED;
        m_VelocityY = 15.0f;
        m_Drawable->m_Transform.scale.y = -3.0f;
        m_Drawable->SetImage(GA_RESOURCE_DIR"/Image/Character/goomba/goomba1.png");
    }
}

Rect Goomba::GetRect(float worldOffset) const {
    Rect result;
    result.x = m_WorldX - worldOffset - 24.0f;
    result.y = m_WorldY - 24.0f;
    result.width = 48.0f;
    result.height = 48.0f;
    return result;
}