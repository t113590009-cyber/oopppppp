#include "App.hpp"
#include "Mushroom.hpp"
#include "Star.hpp"
#include "Coin.hpp"
#include "FireFlower.hpp"
#include "MovingBlock.hpp" 

void App::UpdateBlocksAndItems(float dt, const std::vector<Rect>& allObstacles) {
    glm::vec2 pPos = m_Player->GetPosition();

    for (auto it = m_Blocks.begin(); it != m_Blocks.end(); ) {
        (*it)->Update(dt, m_WorldOffset);

        if ((*it)->HasJustSpawnedItem()) {
            Block::ItemType type = (*it)->GetItemType();
            if (type == Block::ItemType::MUSHROOM) {
                if (m_Player->IsBig() || m_Player->IsFire()) {
                    auto fireFlower = std::make_shared<FireFlower>((*it)->GetPosition().x, (*it)->GetPosition().y);
                    m_Root.AddChild(fireFlower);
                    m_Items.push_back(std::move(fireFlower));
                }
                else {
                    auto mushroom = std::make_shared<Mushroom>((*it)->GetPosition().x, (*it)->GetPosition().y);
                    m_Root.AddChild(mushroom);
                    m_Items.push_back(std::move(mushroom));
                }
            }
            else if (type == Block::ItemType::STAR) {
                auto star = std::make_shared<Star>((*it)->GetPosition().x, (*it)->GetPosition().y);
                m_Root.AddChild(star);
                m_Items.push_back(std::move(star));
            }
            else if (type == Block::ItemType::COIN) {
                auto coin = std::make_shared<Coin>((*it)->GetPosition().x, (*it)->GetPosition().y, 0);
                m_Root.AddChild(coin);
                m_Items.push_back(std::move(coin));
                m_Score += 200;
                m_Coins += 1;
                if (m_Coins >= 100) { m_Coins = 0; m_Lives += 1; }

                auto score = std::make_shared<ScoreEffect>(200, (*it)->GetPosition().x, (*it)->GetPosition().y + 50.0f);
                m_Root.AddChild(score->GetDrawable());
                m_ScoreEffects.push_back(score);
            }
        }
        if ((*it)->IsDestroyed()) {
            m_Root.RemoveChild((*it)->GetCharacter());
            it = m_Blocks.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = m_Items.begin(); it != m_Items.end(); ) {
        (*it)->Update(dt, m_WorldOffset, allObstacles);

        Rect marioBodyRect = { m_WorldOffset + pPos.x - 18.0f, pPos.y - 24.0f, 36.0f, 48.0f };
        Rect itemRect = (*it)->GetRect(m_WorldOffset);

        if (CollisionHandler::CheckCollision(marioBodyRect, itemRect)) {
            (*it)->ApplyEffect(m_Player.get());

            if ((*it)->IsDestroyed()) {
                int earnedScore = 0;
                if (std::dynamic_pointer_cast<Coin>(*it)) {
                    earnedScore = 200;
                    m_Coins += 1;
                    if (m_Coins >= 100) { m_Coins = 0; m_Lives += 1; }
                }
                else if (std::dynamic_pointer_cast<Mushroom>(*it) || std::dynamic_pointer_cast<FireFlower>(*it) || std::dynamic_pointer_cast<Star>(*it)) {
                    earnedScore = 1000;
                }

                if (earnedScore > 0) {
                    m_Score += earnedScore;
                    auto score = std::make_shared<ScoreEffect>(earnedScore, itemRect.x + 15.0f, itemRect.y + 40.0f);
                    m_Root.AddChild(score->GetDrawable());
                    m_ScoreEffects.push_back(score);
                }
            }
        }

        if ((*it)->IsDestroyed()) {
            m_Root.RemoveChild(*it);
            it = m_Items.erase(it);
        }
        else {
            ++it;
        }
    }
}

void App::UpdateEnemiesAndFireballs(float dt, const std::vector<Rect>& allObstacles, const glm::vec2& pPos) {
    if (m_Player->GetState() == AnimState::FLAG_SLIDE) {
        return;
    }

    // ==========================================
    // 🌟 更新並偵測火柱
    // ==========================================
    for (auto& fb : m_FireBars) {
        fb->Update(dt, m_WorldOffset);
        fb->Interact(m_Player.get(), m_WorldOffset);
    }

    for (auto it = m_Fireballs.begin(); it != m_Fireballs.end(); ) {
        (*it)->Update(dt, m_WorldOffset, allObstacles);
        if ((*it)->IsDestroyed()) {
            m_Root.RemoveChild(*it);
            it = m_Fireballs.erase(it);
        }
        else {
            ++it;
        }
    }

    if (m_CurrentLevel == 1) {
        if (m_SpawnPhase == 0 && m_WorldOffset + 650.0f > 696.0f) {
            auto g = std::make_unique<Goomba>(696.0f);
            m_Root.AddChild(g->GetDrawable());
            m_Goombas.push_back(std::move(g));
            m_SpawnPhase = 1;
        }
        else if (m_SpawnPhase == 1 && m_WorldOffset + 650.0f > 1560.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(1560.0f + (i * 72.0f));
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 2;
        }
        else if (m_SpawnPhase == 2 && m_WorldOffset + 650.0f > 2088.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(2088.0f + (i * 72.0f));
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 3;
        }
        else if (m_SpawnPhase == 3 && m_WorldOffset + 650.0f > 3480.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(3480.0f + (i * 72.0f));
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 4;
        }
        else if (m_SpawnPhase == 4 && m_WorldOffset + 650.0f > 4728.0f) {
            auto turtle = std::make_shared<Koopatroopa>(4728.0f, -168.0f);
            m_Root.AddChild(turtle);
            m_Koopatroopas.push_back(turtle);
            m_SpawnPhase = 5;
        }
        else if (m_SpawnPhase == 5 && m_WorldOffset + 650.0f > 5112.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(5112.0f + (i * 72.0f));
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 6;
        }
        else if (m_SpawnPhase == 6 && m_WorldOffset + 650.0f > 7992.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(7992.0f + (i * 72.0f));
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 7;
        }
    }
    else if (m_CurrentLevel == 2) {
        if (m_SpawnPhase == 0 && m_WorldOffset + 650.0f > 1008.0f) {
            auto ft = std::make_shared<Koopatroopa>(1008.0f, 120.0f, false);
            m_Root.AddChild(ft);
            m_Koopatroopas.push_back(ft);
            m_SpawnPhase = 1;
        }
        else if (m_SpawnPhase == 1 && m_WorldOffset + 500.0f > 1728.0f) {
            for (int i = 0; i < 2; ++i) {
                auto g = std::make_unique<Goomba>(1728.0f + (i * 96.0f), 168.0f);
                m_Root.AddChild(g->GetDrawable());
                m_Goombas.push_back(std::move(g));
            }
            m_SpawnPhase = 2;
        }
        else if (m_SpawnPhase == 2 && m_WorldOffset + 650.0f > 3120.0f) {
            auto ft = std::make_shared<Koopatroopa>(3120.0f, 120.0f, true);
            m_Root.AddChild(ft);
            m_Koopatroopas.push_back(ft);
            m_SpawnPhase = 3;
        }
        else if (m_SpawnPhase == 3 && m_WorldOffset + 650.0f > 3456.0f) {
            auto g = std::make_unique<Goomba>(3456.0f, 168.0f, false);
            m_Root.AddChild(g->GetDrawable());
            m_Goombas.push_back(std::move(g));
            m_SpawnPhase = 4;
        }
        else if (m_SpawnPhase == 4 && m_WorldOffset + 650.0f > 4848.0f) {
            auto ft = std::make_shared<Koopatroopa>(4848.0f, 0.0f, false);
            m_Root.AddChild(ft);
            m_Koopatroopas.push_back(ft);
            m_SpawnPhase = 5;
        }
        else if (m_SpawnPhase == 5 && m_WorldOffset + 650.0f > 5040.0f) {
            auto ft = std::make_shared<Koopatroopa>(5040.0f, 96.0f, true);
            m_Root.AddChild(ft);
            m_Koopatroopas.push_back(ft);
            m_SpawnPhase = 6;
        }
        else if (m_SpawnPhase == 6 && m_WorldOffset + 650.0f > 6000.0f) {
            auto turtle = std::make_shared<Koopatroopa>(6000.0f, -168.0f, false);
            m_Root.AddChild(turtle);
            m_Koopatroopas.push_back(turtle);
            m_SpawnPhase = 7;
        }
    }
    else if (m_CurrentLevel == 3) {
        if (m_SpawnPhase == 0 && m_WorldOffset + 1500.0f > 6116.0f) {
            auto bowser = std::make_shared<Bowser>(6116.0f, 0.0f);
            m_Root.AddChild(bowser);
            m_Bowsers.push_back(bowser);
            m_SpawnPhase = 1;
        }
    }

    const int STOMP_SCORES[] = { 100, 200, 400, 500, 800, 1000, 2000, 4000, 8000, -1 };
    Rect marioScreenRect = { pPos.x - 18.0f, pPos.y - 25.0f, 36.0f, 20.0f };

    for (auto it = m_Goombas.begin(); it != m_Goombas.end(); ) {
        (*it)->Update(dt, m_WorldOffset, m_Collision);
        Rect goombaScreenRect = (*it)->GetRect(m_WorldOffset);

        bool hitByFireball = false;
        for (auto& fb : m_Fireballs) {
            Rect fbScreenRect = fb->GetRect();
            fbScreenRect.x -= m_WorldOffset;
            if (!fb->IsDestroyed() && !fb->IsExploding() && CollisionHandler::CheckCollision(fbScreenRect, goombaScreenRect)) {
                fb->Explode();
                hitByFireball = true;
                break;
            }
        }

        if (hitByFireball && (*it)->GetState() != Goomba::State::DEAD) {
            (*it)->Stomp();
            m_Score += 200;
            float gX = goombaScreenRect.x + m_WorldOffset + 18.0f;
            auto score = std::make_shared<ScoreEffect>(200, gX, goombaScreenRect.y + 24.0f);
            m_Root.AddChild(score->GetDrawable());
            m_ScoreEffects.push_back(score);
        }
        else if ((*it)->GetState() == Goomba::State::WALKING && CollisionHandler::CheckCollision(marioScreenRect, goombaScreenRect)) {
            if (m_Player->IsStarMode()) {
                (*it)->Stomp();
                m_Score += 100;
                float gX = goombaScreenRect.x + m_WorldOffset + 18.0f;
                auto score = std::make_shared<ScoreEffect>(100, gX, goombaScreenRect.y + 24.0f);
                m_Root.AddChild(score->GetDrawable());
                m_ScoreEffects.push_back(score);
            }
            else {
                float marioBottom = pPos.y - 25.0f;
                float goombaCenter = goombaScreenRect.y + (goombaScreenRect.height / 2.0f);

                if (m_Player->GetVelocityY() < 0.0f && marioBottom > goombaCenter) {
                    (*it)->Stomp();
                    int scoreValue = STOMP_SCORES[std::min(m_ComboCount, 9)];
                    m_ComboCount++;
                    if (scoreValue == -1) m_Lives += 1;
                    else m_Score += scoreValue;

                    float gX = goombaScreenRect.x + m_WorldOffset + 18.0f;
                    auto score = std::make_shared<ScoreEffect>(scoreValue, gX, goombaScreenRect.y + 24.0f);
                    m_Root.AddChild(score->GetDrawable());
                    m_ScoreEffects.push_back(score);
                }
                else {
                    m_Player->TakeDamage();
                }
            }
        }

        if ((*it)->GetState() == Goomba::State::DEAD) {
            m_Root.RemoveChild((*it)->GetDrawable());
            it = m_Goombas.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto& koopa : m_Koopatroopas) {
        koopa->Update(dt, m_WorldOffset, allObstacles);
        Rect koopaRect = koopa->GetRect(m_WorldOffset);

        bool hitByFireball = false;
        for (auto& fb : m_Fireballs) {
            if (!fb->IsDestroyed() && !fb->IsExploding() && CollisionHandler::CheckCollision(fb->GetRect(), koopaRect)) {
                fb->Explode();
                hitByFireball = true;
                break;
            }
        }
        if (hitByFireball && koopa->GetState() != Koopatroopa::State::DEAD) {
            koopa->Stomp();
            m_Score += 200;
            float kX = koopaRect.x + 18.0f;
            auto score = std::make_shared<ScoreEffect>(200, kX, koopaRect.y + 24.0f);
            m_Root.AddChild(score->GetDrawable());
            m_ScoreEffects.push_back(score);
        }

        koopa->Interact(m_Player.get(), m_WorldOffset);
    }

    for (auto it = m_BowserFires.begin(); it != m_BowserFires.end(); ) {
        (*it)->Update(dt, m_WorldOffset);
        (*it)->Interact(m_Player.get(), m_WorldOffset);

        if ((*it)->IsDestroyed()) {
            m_Root.RemoveChild(*it);
            it = m_BowserFires.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto& bowser : m_Bowsers) {
        auto newFire = bowser->Update(dt, m_WorldOffset, allObstacles);
        if (newFire) {
            m_Root.AddChild(newFire);
            m_BowserFires.push_back(newFire);
        }

        bowser->Interact(m_Player.get(), m_WorldOffset);

        for (auto& fb : m_Fireballs) {
            Rect fbScreenRect = fb->GetRect();
            fbScreenRect.x -= m_WorldOffset;

            if (!fb->IsDestroyed() && !fb->IsExploding() && !bowser->IsDead()) {
                if (CollisionHandler::CheckCollision(fbScreenRect, bowser->GetRect(m_WorldOffset))) {
                    bowser->TakeDamage(1);
                    fb->Explode();

                    auto score = std::make_shared<ScoreEffect>(100, fbScreenRect.x + m_WorldOffset, fbScreenRect.y + 20.0f);
                    m_Root.AddChild(score->GetDrawable());
                    m_ScoreEffects.push_back(score);
                }
            }
        }
    }
}
//