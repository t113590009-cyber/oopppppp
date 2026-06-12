#include "Koopatroopa.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp" 

// 🌟 建構子加上 isFlying 參數
Koopatroopa::Koopatroopa(float spawnWorldX, float spawnWorldY, bool isFlying) {
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;

    m_WalkImages = {
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle.png",
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle2.png"
    };
    // 🌟 載入你準備好的飛天圖片
    m_FlyImages = {
        GA_RESOURCE_DIR"/Image/Props/turtlefly1.png",
        GA_RESOURCE_DIR"/Image/Props/turtlefly2.png"
    };
    m_DieImage = GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtledie.png";

    // 🌟 根據開關決定初始狀態
    m_State = isFlying ? State::FLYING : State::WALKING;
    this->SetDrawable(std::make_shared<Util::Image>(isFlying ? m_FlyImages[0] : m_WalkImages[0]));

    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(4);
}

void Koopatroopa::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    if (m_State == State::DEAD) {
        m_VelocityY -= GRAVITY * deltaTime;
        m_WorldY += m_VelocityY;
        this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY };

        m_DeadTimer += deltaTime;
        if (m_DeadTimer > 1.5f) {
            this->SetVisible(false);
        }
        return;
    }

    if (m_State == State::SHELL_IDLE) {
        m_IdleTimer -= deltaTime;
        if (m_IdleTimer <= 0.0f) {
            m_State = State::WALKING;
            this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[0]));
        }
    }

    // 🌟 飛天、走路、高速滑行 都需要移動與動畫
    if (m_State == State::WALKING || m_State == State::SHELL_MOVING || m_State == State::FLYING) {

        // 1. 動畫播放
        if (m_State == State::WALKING || m_State == State::FLYING) {
            m_AnimTimer += deltaTime;
            if (m_AnimTimer >= 0.2f) {
                m_AnimTimer = 0.0f;
                m_AnimFrame = (m_AnimFrame + 1) % 2;
                // 根據狀態切換圖片
                if (m_State == State::FLYING) {
                    this->SetDrawable(std::make_shared<Util::Image>(m_FlyImages[m_AnimFrame]));
                }
                else {
                    this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[m_AnimFrame]));
                }
            }
        }

        float moveSpeed = (m_State == State::SHELL_MOVING) ? 300.0f : 60.0f;
        float dirSign = (m_SpeedX > 0) ? 1.0f : -1.0f;
        float nextX = m_WorldX + (dirSign * moveSpeed * deltaTime);
        Rect nextXRect = { nextX - 20.0f, m_WorldY - 20.0f, 40.0f, 40.0f };

        // X軸撞牆判定
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
            nextX = m_WorldX;
        }

        // Y軸重力判定
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

        // ==========================================
        // 🌟 飛天烏龜的核心：碰到地板就往上彈！
        // ==========================================
        if (onGround && m_State == State::FLYING) {
            m_VelocityY = 12.0f; // 給予向上的彈跳力
        }

        if (!onGround) m_WorldY = nextY;

        // 懸崖防落機制 (飛天烏龜不需要防落，它會直接彈過去)
        if (m_State == State::WALKING && onGround && !hitWall) {
            float checkX = (dirSign > 0) ? nextX + 22.0f : nextX - 22.0f;
            Rect cliffCheck = { checkX, m_WorldY - 30.0f, 10.0f, 10.0f };

            bool isSafe = false;
            for (const auto& obs : obstacles) {
                if (CollisionHandler::CheckCollision(cliffCheck, obs)) {
                    isSafe = true;
                    break;
                }
            }

            if (!isSafe) {
                m_SpeedX = -m_SpeedX;
                this->m_Transform.scale.x = (m_SpeedX > 0) ? -3.0f : 3.0f;
            }
            else {
                m_WorldX = nextX;
            }
        }
        else {
            m_WorldX = nextX;
        }
    }

    if (m_WorldY < -400.0f) {
        m_State = State::DEAD;
        this->SetVisible(false);
    }

    this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY + 10.0f };
}

void Koopatroopa::Interact(Player* player, float worldOffset) {
    if (m_State == State::DEAD || !player || player->IsDead()) return;

    Rect koopaRect = GetRect(worldOffset);
    Rect playerBody = {
        worldOffset + player->GetPosition().x - 18.0f,
        player->GetPosition().y - 20.0f,
        36.0f, 40.0f
    };

    if (!CollisionHandler::CheckCollision(koopaRect, playerBody)) return;

    if (player->IsStarMode()) {
        m_State = State::DEAD;
        m_VelocityY = 15.0f;
        this->m_Transform.scale.y = -3.0f;
        this->SetDrawable(std::make_shared<Util::Image>(m_DieImage));
        return;
    }

    Rect playerFeet = player->GetFeetRect(worldOffset);

    if (player->GetVelocityY() < 0.0f && playerFeet.y > koopaRect.y + 10.0f) {
        player->Bounce();

        if (m_State == State::SHELL_IDLE) {
            bool kickRight = (playerBody.x < koopaRect.x);
            Kick(kickRight);
        }
        else {
            Stomp(); // 🌟 飛天或走路被踩都會進這裡
        }
    }
    else {
        // 🌟 側邊碰撞加上 FLYING 的判定
        if (m_State == State::WALKING || m_State == State::SHELL_MOVING || m_State == State::FLYING) {
            player->TakeDamage();
        }
        else if (m_State == State::SHELL_IDLE) {
            bool kickRight = (playerBody.x < koopaRect.x);
            Kick(kickRight);
        }
    }
}

Rect Koopatroopa::GetRect(float worldOffset) const {
    if (m_State == State::DEAD) {
        return { -9999.0f, -9999.0f, 0.0f, 0.0f };
    }
    if (m_State == State::SHELL_IDLE || m_State == State::SHELL_MOVING) {
        return { m_WorldX - 22.0f, m_WorldY - 22.0f, 44.0f, 24.0f };
    }
    return { m_WorldX - 22.0f, m_WorldY - 22.0f, 44.0f, 44.0f };
}

void Koopatroopa::Stomp() {
    // 🌟 新增：如果是飛天烏龜被踩到，只會失去翅膀變成走路模式
    if (m_State == State::FLYING) {
        m_State = State::WALKING;
        m_VelocityY = 0.0f; // 強制中斷它往上彈的動能
        this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[0]));
    }
    else if (m_State == State::WALKING || m_State == State::SHELL_MOVING) {
        m_State = State::SHELL_IDLE;
        m_IdleTimer = 5.0f;
        this->SetDrawable(std::make_shared<Util::Image>(m_DieImage));
    }
}

void Koopatroopa::Kick(bool toRight) {
    m_State = State::SHELL_MOVING;
    m_SpeedX = toRight ? 1.0f : -1.0f;
}