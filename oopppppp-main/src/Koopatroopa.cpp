#include "Koopatroopa.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp" 

Koopatroopa::Koopatroopa(float spawnWorldX, float spawnWorldY, bool isFlying) {
    m_WorldX = spawnWorldX;
    m_WorldY = spawnWorldY;
    m_StartY = spawnWorldY; // 🌟 記錄起始高度，當作上下滑動的中心點！

    m_WalkImages = {
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle.png",
        GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtle2.png"
    };
    m_FlyImages = {
        GA_RESOURCE_DIR"/Image/Props/turtlefly1.png",
        GA_RESOURCE_DIR"/Image/Props/turtlefly2.png"
    };
    m_DieImage = GA_RESOURCE_DIR"/Image/Character/koopa troopa/turtledie.png";

    m_State = isFlying ? State::FLYING : State::WALKING;
    this->SetDrawable(std::make_shared<Util::Image>(isFlying ? m_FlyImages[0] : m_WalkImages[0]));

    this->m_Transform.scale = { 3.0f, 3.0f };
    this->SetZIndex(4);
}

void Koopatroopa::Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) {
    // 🌟 減少冷卻計時器，防止同一個動作在 0.1 秒內被連續觸發
    if (m_InteractTimer > 0.0f) {
        m_InteractTimer -= deltaTime;
    }

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

    if (m_State == State::WALKING || m_State == State::SHELL_MOVING || m_State == State::FLYING) {

        // 1. 動畫播放 (走路與飛天都要拍翅膀/走動)
        if (m_State == State::WALKING || m_State == State::FLYING) {
            m_AnimTimer += deltaTime;
            if (m_AnimTimer >= 0.2f) {
                m_AnimTimer = 0.0f;
                m_AnimFrame = (m_AnimFrame + 1) % 2;
                if (m_State == State::FLYING) {
                    this->SetDrawable(std::make_shared<Util::Image>(m_FlyImages[m_AnimFrame]));
                }
                else {
                    this->SetDrawable(std::make_shared<Util::Image>(m_WalkImages[m_AnimFrame]));
                }
            }
        }

        // ==========================================
        // 🌟 2. 移動邏輯 (依照狀態分歧)
        // ==========================================
        if (m_State == State::FLYING) {
            // 🌟 【飛天狀態】純粹上下移動，無視重力與地形
            m_WorldY += m_FlyDirection * m_FlySpeed * deltaTime;

            // 檢查是否超過設定的上下限
            if (m_FlyDirection > 0 && m_WorldY >= m_StartY + m_FlyRange) {
                m_WorldY = m_StartY + m_FlyRange;
                m_FlyDirection = -1; // 碰到最高點，往下
            }
            else if (m_FlyDirection < 0 && m_WorldY <= m_StartY - m_FlyRange) {
                m_WorldY = m_StartY - m_FlyRange;
                m_FlyDirection = 1;  // 碰到最低點，往上
            }

            this->m_Transform.scale.x = 3.0f; // 飛天時固定朝左
        }
        else {
            // 🌟 【走路/龜殼狀態】原版的水平移動、撞牆與重力邏輯
            float moveSpeed = (m_State == State::SHELL_MOVING) ? 300.0f : 60.0f;
            float dirSign = (m_SpeedX > 0) ? 1.0f : -1.0f;
            float nextX = m_WorldX + (dirSign * moveSpeed * deltaTime);
            Rect nextXRect = { nextX - 20.0f, m_WorldY - 20.0f, 40.0f, 40.0f };

            bool hitWall = false;
            for (const auto& obs : obstacles) {
                if (CollisionHandler::CheckCollision(nextXRect, obs)) {
                    hitWall = true; break;
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

            if (!onGround) m_WorldY = nextY;

            // 懸崖防落機制 (只有 WALKING 有效)
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
    }

    if (m_WorldY < -400.0f) {
        m_State = State::DEAD;
        this->SetVisible(false);
    }

    this->m_Transform.translation = { m_WorldX - worldOffset, m_WorldY + 10.0f };
}

void Koopatroopa::Interact(Player* player, float worldOffset) {
    if (m_State == State::DEAD || !player || player->IsDead()) return;

    bool isBig = player->IsBig() || player->IsFire();
    float marioHeight = isBig ? 96.0f : 48.0f;
    float marioBottom = player->GetPosition().y - (isBig ? 48.0f : 24.0f);

    Rect koopaRect = GetRect(worldOffset);
    Rect playerBody = {
        worldOffset + player->GetPosition().x - 18.0f,
        marioBottom,
        36.0f, marioHeight
    };

    if (!CollisionHandler::CheckCollision(koopaRect, playerBody)) return;

    if (player->IsStarMode()) {
        m_State = State::DEAD;
        m_VelocityY = 15.0f;
        this->m_Transform.scale.y = -3.0f;
        this->SetDrawable(std::make_shared<Util::Image>(m_DieImage));
        return;
    }

    float koopaCenter = koopaRect.y + (koopaRect.height / 2.0f);

    if (player->GetVelocityY() < 0.0f && marioBottom > koopaCenter - 15.0f) {
        player->Bounce();

        if (m_State == State::SHELL_IDLE) {
            bool kickRight = (playerBody.x < koopaRect.x);
            Kick(kickRight);
        }
        else {
            Stomp();
        }
    }
    else {
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
    if (m_State == State::FLYING) {
        // 被踩掉翅膀，回歸重力並開始走動
        m_State = State::WALKING;
        m_VelocityY = 0.0f;
        m_SpeedX = -1.0f; // 預設往左走
        this->m_Transform.scale.x = 3.0f;
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