#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

// 🌟 確保所有道具與特效的標頭檔都有引入
#include "Mushroom.hpp"
#include "Star.hpp"
#include "Coin.hpp"
#include "ScoreEffect.hpp"
#include "FireFlower.hpp" 
#include "Fireball.hpp"   

void App::Update() {
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_2)) {
        m_CurrentLevel = 2;
        ResetLevel();
    }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_1)) {
        m_CurrentLevel = 1;
        ResetLevel();
    }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_3)) {
        m_CurrentLevel = 3;
        ResetLevel();
    }

    float dt = Util::Time::GetDeltaTime();

    // --- 1. 選單邏輯 ---
    if (m_Menu->GetVisibility()) {
        m_Menu->Update(dt);
        if (m_Menu->IsStartPressed()) {
            m_Menu->SetVisible(false);
            if (m_Player && m_Player->GetCharacter()) {
                m_Player->GetCharacter()->SetVisible(true);
                m_Player->GetCharacter()->SetPosition({ -300.0f, -264.0f });
            }
            if (m_Map) m_Map->SetVisible(true);
            if (m_Castle) m_Castle->SetVisible(true);

            if (m_Flagpole) m_Flagpole->SetVisible(true);
            if (m_Flag) m_Flag->SetVisible(true);

            for (auto& block : m_Blocks) {
                if (block->GetCharacter()) {
                    block->GetCharacter()->SetVisible(true);
                }
            }

            if (m_TopUI) m_TopUI->SetVisible(true);
        }
    }
    // --- 2. 遊戲邏輯 ---
    else {
        if (m_Player->IsDead()) {
            m_DeathTimer += dt;
            if (m_DeathTimer > 2.5f) {
                m_Lives--;
                m_DeathTimer = 0.0f;
                if (m_Lives > 0) {
                    ResetLevel();
                }
                else {
                    m_CurrentState = State::END;
                }
            }
        }
        else {
            if (m_Player->GetPosition().y < -400.0f) {
                m_Player->Die();
            }

            if (m_CurrentState != State::END && !m_IsLevelClear) {
                m_GameTime -= dt * 2.5f;
                if (m_GameTime < 0.0f) m_GameTime = 0.0f;
            }

            // 🌟 將 m_CurrentLevel 傳給 UI，讓它顯示 1-1, 1-2 等
            if (m_TopUI) {
                m_TopUI->Update(m_Score, m_Coins, static_cast<int>(m_GameTime), m_CurrentLevel);
            }

            if (!m_IsLevelClear) {
                m_Player->Update(m_WorldOffset, m_Collision, m_Blocks, dt);
            }

            glm::vec2 pPos = m_Player->GetPosition();

            if (m_Player->IsOnGround()) {
                m_ComboCount = 0;
            }
            const int STOMP_SCORES[] = { 100, 200, 400, 500, 800, 1000, 2000, 4000, 8000, -1 };

            if (!m_IsLevelClear && m_Player->IsFire() && Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
                if (m_Fireballs.size() < 2) {
                    bool faceRight = (m_Player->GetCharacter()->m_Transform.scale.x > 0);
                    auto fireball = std::make_shared<Fireball>(m_WorldOffset + pPos.x, pPos.y, faceRight);
                    m_Root.AddChild(fireball);
                    m_Fireballs.push_back(fireball);
                }
            }

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
                        if (m_Coins >= 100) {
                            m_Coins = 0;
                            m_Lives += 1;
                        }
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

            std::vector<Rect> allObstacles = m_Collision.GetObstacles();
            for (const auto& block : m_Blocks) {
                Rect hit = block->GetHitbox();
                if (hit.width > 0) {
                    allObstacles.push_back(hit);
                }
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

            if (m_CurrentLevel == 1) {
                if (m_Castle) m_Castle->SetPosition({ 9436.0f - m_WorldOffset, -145.0f });
                if (m_Flagpole) m_Flagpole->SetPosition({ 9147.0f - m_WorldOffset, 6.0f });
                if (m_Flag && !m_IsLevelClear) m_Flag->SetPosition({ 9120.0f - m_WorldOffset, 210.0f });

                float marioWorldX = pPos.x + m_WorldOffset;

                if (!m_IsLevelClear && marioWorldX >= 9140.0f && marioWorldX <= 9155.0f) {
                    m_IsLevelClear = true;
                    m_IsFlagSliding = true;

                    int timeDigit = static_cast<int>(m_GameTime) % 10;
                    if (timeDigit == 1 || timeDigit == 3 || timeDigit == 6) {
                        m_PendingFireworks = timeDigit;
                    }
                    else {
                        m_PendingFireworks = 0;
                    }
                    m_FireworkTimer = 0.0f;

                    float hitY = m_Player->GetCharacter()->GetPosition().y;
                    int flagScore = 100;

                    if (hitY > -100) flagScore = 400;
                    if (hitY > 0)    flagScore = 800;
                    if (hitY > 100)  flagScore = 2000;
                    if (hitY > 150)  flagScore = 5000;
                    m_Score += flagScore;

                    auto flagScoreFx = std::make_shared<ScoreEffect>(flagScore, 9147.0f - m_WorldOffset + 30.0f, hitY);
                    m_Root.AddChild(flagScoreFx->GetDrawable());
                    m_ScoreEffects.push_back(flagScoreFx);

                    m_Player->StartFlagSlide(9147.0f - m_WorldOffset);
                    m_Player->GetCharacter()->SetPosition({ 9147.0f - m_WorldOffset - 10.0f, hitY });
                }

                if (m_IsLevelClear && m_Flag && !m_IsFlagSliding) {
                    m_Flag->SetPosition({ 9120.0f - m_WorldOffset, m_Flag->GetPosition().y });
                }

                if (m_Flag && m_IsFlagSliding) {
                    bool flagReachedBottom = false;
                    bool marioReachedBottom = false;

                    glm::vec2 flagPos = m_Flag->GetPosition();
                    if (flagPos.y > -185.0f) {
                        m_Flag->SetPosition({ 9120.0f - m_WorldOffset, flagPos.y - (300.0f * dt) });
                    }
                    else {
                        flagReachedBottom = true;
                    }

                    glm::vec2 marioCharPos = m_Player->GetCharacter()->GetPosition();
                    float groundY = (m_Player->IsBig() || m_Player->IsFire()) ? -216.0f : -240.0f;

                    if (marioCharPos.y > groundY) {
                        m_Player->GetCharacter()->SetPosition({ 9147.0f - m_WorldOffset - 10.0f, marioCharPos.y - (300.0f * dt) });
                    }
                    else {
                        m_Player->GetCharacter()->SetPosition({ 9147.0f - m_WorldOffset - 10.0f, groundY });
                        marioReachedBottom = true;
                    }

                    if (flagReachedBottom && marioReachedBottom) {
                        m_IsFlagSliding = false;
                        m_ClearGroundY = groundY;

                        std::vector<std::string> runAnims;
                        if (m_Player->IsFire()) {
                            runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run3.png" };
                        }
                        else if (m_Player->IsBig()) {
                            runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run3.png" };
                        }
                        else {
                            runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run3.png" };
                        }
                        m_Player->GetCharacter()->SetAnimation(runAnims, 100);
                        m_Player->GetCharacter()->Play();
                    }
                }

                if (m_IsLevelClear && !m_IsFlagSliding) {
                    if (m_WorldOffset < 9250.0f) {
                        m_WorldOffset += 150.0f * dt;
                    }

                    float doorWorldX = 9436.0f;
                    if (marioWorldX < doorWorldX) {
                        marioWorldX += 150.0f * dt;
                        m_Player->GetCharacter()->SetPosition({ marioWorldX - m_WorldOffset, m_ClearGroundY });
                        m_Player->GetCharacter()->Play();
                        m_Player->GetCharacter()->m_Transform.scale.x = 3.0f;
                    }
                    else {
                        m_Player->GetCharacter()->SetVisible(false);

                        if (m_GameTime >= 1.0f) {
                            m_GameTime -= 1.0f;
                            m_Score += 50;
                        }
                        else if (m_GameTime > 0.0f) {
                            m_GameTime = 0.0f;
                        }
                        else {
                            if (m_PendingFireworks > 0) {
                                m_FireworkTimer -= dt;
                                if (m_FireworkTimer <= 0.0f) {
                                    m_PendingFireworks--;
                                    m_FireworkTimer = 0.5f;

                                    std::vector<std::string> boomImgs = {
                                        GA_RESOURCE_DIR"/Image/Items/fireball/boom1.png",
                                        GA_RESOURCE_DIR"/Image/Items/fireball/boom2.png",
                                        GA_RESOURCE_DIR"/Image/Items/fireball/boom3.png"
                                    };
                                    auto fw = std::make_shared<AnimatedCharacter>(boomImgs);
                                    fw->SetAnimation(boomImgs, 150);
                                    fw->m_Transform.scale = { 4.0f, 4.0f };

                                    float offsetX = (m_PendingFireworks % 2 == 0) ? 60.0f : -40.0f;
                                    float fwY = 50.0f + (m_PendingFireworks * 40.0f);
                                    float fwWorldX = 9436.0f + offsetX;

                                    fw->SetVisible(true);
                                    fw->SetPosition({ fwWorldX - m_WorldOffset, fwY });
                                    fw->SetZIndex(20);
                                    fw->SetLooping(false);
                                    fw->Play();

                                    m_Root.AddChild(fw);
                                    m_FireworkWorldX.push_back(fwWorldX);
                                    m_ActiveFireworks.push_back(fw);

                                    m_Score += 500;
                                    auto score = std::make_shared<ScoreEffect>(500, fwWorldX - m_WorldOffset + 20.0f, fwY + 30.0f);
                                    m_Root.AddChild(score->GetDrawable());
                                    m_ScoreEffects.push_back(score);
                                }
                            }
                            // 🌟 煙火放完之後，延遲 2 秒才切換到第二關
                            else if (m_ActiveFireworks.empty()) {
                                m_LevelClearTimer += dt;
                                if (m_LevelClearTimer >= 2.0f) {
                                    m_CurrentLevel = 2;
                                    ResetLevel();
                                }
                            }
                        }
                    }
                }
            } // 結束 if (m_CurrentLevel == 1)

            for (auto it = m_ScoreEffects.begin(); it != m_ScoreEffects.end(); ) {
                (*it)->Update(dt, m_WorldOffset);
                if ((*it)->IsDone()) {
                    m_Root.RemoveChild((*it)->GetDrawable());
                    it = m_ScoreEffects.erase(it);
                }
                else {
                    ++it;
                }
            }

            for (int i = 0; i < (int)m_ActiveFireworks.size(); ) {
                auto& fw = m_ActiveFireworks[i];
                fw->SetPosition({ m_FireworkWorldX[i] - m_WorldOffset, fw->GetPosition().y });
                if (fw->IfAnimationEnds()) {
                    m_Root.RemoveChild(fw);
                    m_ActiveFireworks.erase(m_ActiveFireworks.begin() + i);
                    m_FireworkWorldX.erase(m_FireworkWorldX.begin() + i);
                }
                else {
                    ++i;
                }
            }

            if (m_CurrentLevel == 1) {
                if (m_SpawnPhase == 0 && m_WorldOffset > 800.0f) {
                    auto g = std::make_unique<Goomba>(m_WorldOffset + 700.0f);
                    m_Root.AddChild(g->GetDrawable());
                    m_Goombas.push_back(std::move(g));
                    m_SpawnPhase = 1;
                }
                else if (m_SpawnPhase == 1 && m_WorldOffset > 2500.0f) {
                    for (int i = 0; i < 2; ++i) {
                        auto g = std::make_unique<Goomba>(m_WorldOffset + 700.0f + (i * 100.0f));
                        m_Root.AddChild(g->GetDrawable());
                        m_Goombas.push_back(std::move(g));
                    }
                    auto turtle = std::make_shared<Koopatroopa>(m_WorldOffset + 1200.0f, -100.0f);
                    m_Root.AddChild(turtle);
                    m_Koopatroopas.push_back(turtle);
                    m_SpawnPhase = 2;
                }
                else if (m_SpawnPhase == 2 && m_WorldOffset > 4500.0f) {
                    for (int i = 0; i < 3; ++i) {
                        auto g = std::make_unique<Goomba>(m_WorldOffset + 750.0f + (i * 120.0f));
                        m_Root.AddChild(g->GetDrawable());
                        m_Goombas.push_back(std::move(g));
                    }
                    m_SpawnPhase = 3;
                }
            }

            for (auto it = m_Goombas.begin(); it != m_Goombas.end(); ) {
                (*it)->Update(dt, m_WorldOffset, m_Collision);

                Rect marioScreenRect = { pPos.x - 18.0f, pPos.y - 25.0f, 36.0f, 20.0f };
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

            if (m_Map) m_Map->Update(m_WorldOffset);
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

void App::ResetLevel() {
    m_WorldOffset = 0.0f;
    m_GameTime = 400.0f;

    if (m_Map) {
        m_Map->ShowLevel(m_CurrentLevel);
        m_Map->Update(0.0f);
    }

    for (auto& goomba : m_Goombas) { if (goomba) m_Root.RemoveChild(goomba->GetDrawable()); }
    m_Goombas.clear();
    for (auto& koopa : m_Koopatroopas) { if (koopa) m_Root.RemoveChild(koopa); }
    m_Koopatroopas.clear();
    for (auto& effect : m_ScoreEffects) { if (effect) m_Root.RemoveChild(effect->GetDrawable()); }
    m_ScoreEffects.clear();
    for (auto& item : m_Items) { if (item) m_Root.RemoveChild(item); }
    m_Items.clear();
    for (auto& fb : m_Fireballs) { if (fb) m_Root.RemoveChild(fb); }
    m_Fireballs.clear();

    for (auto& fw : m_ActiveFireworks) { if (fw) m_Root.RemoveChild(fw); }
    m_ActiveFireworks.clear();
    m_FireworkWorldX.clear();
    m_PendingFireworks = 0;
    m_FireworkTimer = 0.0f;

    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) m_Root.RemoveChild(block->GetCharacter());
    }
    m_Blocks.clear();

    m_Collision.ClearObstacles();

    m_SpawnPhase = 0;
    m_IsLevelClear = false;
    m_IsFlagSliding = false;
    m_LevelClearTimer = 0.0f; // 🌟 記得重置計時器

    if (m_CurrentLevel == 1) {
        if (m_Player) {
            m_Player->ResetStatus();
            if (m_Player->GetCharacter()) {
                m_Player->GetCharacter()->SetPosition({ -300.0f, -264.0f });
                m_Player->GetCharacter()->SetVisible(true);
            }
        }
        if (m_Flagpole) {
            m_Flagpole->SetPosition({ 9147.0f, -95.0f });
            m_Flagpole->SetVisible(true);
        }
        if (m_Flag) {
            m_Flag->SetPosition({ 9110.0f, 150.0f });
            m_Flag->SetVisible(true);
        }
        if (m_Castle) m_Castle->SetVisible(true);

        LoadLevelObjects();
    }
    else if (m_CurrentLevel == 2) {
        if (m_Player) {
            m_Player->ResetStatus();
            m_Player->SetWorldPosition(-450.0f, -264.0f);
            if (m_Player->GetCharacter()) {
                m_Player->GetCharacter()->SetVisible(true);
            }
        }

        if (m_Castle) m_Castle->SetVisible(false);
        if (m_Flagpole) m_Flagpole->SetVisible(false);
        if (m_Flag) m_Flag->SetVisible(false);

        LoadLevel2Objects();
    }
    else if (m_CurrentLevel == 3) {
        if (m_Player) {
            m_Player->ResetStatus();
            m_Player->SetWorldPosition(-450.0f, -264.0f);
            if (m_Player->GetCharacter()) {
                m_Player->GetCharacter()->SetVisible(true);
            }
        }

        if (m_Castle) m_Castle->SetVisible(false);
        if (m_Flagpole) m_Flagpole->SetVisible(false);
        if (m_Flag) m_Flag->SetVisible(false);

        LoadLevel3Objects();
    }

    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) block->GetCharacter()->SetVisible(true);
    }
    for (auto& item : m_Items) { if (item) item->SetVisible(true); }
}