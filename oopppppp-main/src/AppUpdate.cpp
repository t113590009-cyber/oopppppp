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
#include "FireFlower.hpp" // 🔥 必須引入火之花
#include "Fireball.hpp"   // 🔥 必須引入火球

void App::Update() {
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

            // 🚩 按 Enter 後顯示旗桿與旗子
            if (m_Flagpole) m_Flagpole->SetVisible(true);
            if (m_Flag) m_Flag->SetVisible(true);

            for (auto& block : m_Blocks) {
                if (block->GetCharacter()) {
                    block->GetCharacter()->SetVisible(true);
                }
            }

            // 🌟 顯示計分板 UI
            if (m_TopUI) m_TopUI->SetVisible(true);
        }
    }
    // --- 2. 遊戲邏輯 ---
    else {
        // 💀 死亡與重生判定
        if (m_Player->IsDead()) {
            m_DeathTimer += dt;
            if (m_DeathTimer > 2.5f) {
                m_Lives--;
                m_DeathTimer = 0.0f;
                if (m_Lives > 0) {
                    ResetLevel(); // 小馬力歐死亡重置
                }
                else {
                    m_CurrentState = State::END;
                }
            }
        }
        else {
            // 🕳️ 深淵判定
            if (m_Player->GetPosition().y < -400.0f) {
                m_Player->Die();
            }

            // ⏰ 時間倒數與 UI 更新
            if (m_CurrentState != State::END) {
                m_GameTime -= dt * 2.5f;
                if (m_GameTime < 0.0f) m_GameTime = 0.0f;
            }
            if (m_TopUI) {
                m_TopUI->Update(m_Score, m_Coins, static_cast<int>(m_GameTime));
            }

            // ==========================================
            // 🏃‍♂️ 玩家物理更新 (包含傳送門與磚塊碰撞)
            // ==========================================
            m_Player->Update(m_WorldOffset, m_Collision, m_Blocks, dt);

            // 🌟 落地時重置踩踏連擊
            if (m_Player->IsOnGround()) {
                m_ComboCount = 0;
            }
            const int STOMP_SCORES[] = { 100, 200, 400, 500, 800, 1000, 2000, 4000, 8000, -1 };

            // ==========================================
            // 🔥 發射火球邏輯 (按下 Space 鍵發射)
            // ==========================================
            if (m_Player->IsFire() && Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
                if (m_Fireballs.size() < 2) {
                    bool faceRight = (m_Player->GetCharacter()->m_Transform.scale.x > 0);
                    glm::vec2 pPos = m_Player->GetPosition();

                    // 🌟 關鍵修改：發射位置 = 螢幕位置 + 鏡頭位移
                    auto fireball = std::make_shared<Fireball>(m_WorldOffset + pPos.x, pPos.y, faceRight);
                    m_Root.AddChild(fireball);
                    m_Fireballs.push_back(fireball);
                }
            }

            // ==========================================
            // 🧱 磚塊更新、道具生成與破壞判定 (合併優化版)
            // ==========================================
            for (auto it = m_Blocks.begin(); it != m_Blocks.end(); ) {
                (*it)->Update(dt, m_WorldOffset);

                // 🌟 1. 先檢查這個磚塊是不是剛被撞過、要噴道具？
                if ((*it)->HasJustSpawnedItem()) {
                    Block::ItemType type = (*it)->GetItemType();

                    if (type == Block::ItemType::MUSHROOM) {
                        // 🌟 大隻或火球撞蘑菇方塊，會噴出火之花
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

                // 🌟 2. 噴完道具後，才判斷這個磚塊是否該消失
                if ((*it)->IsDestroyed()) {
                    m_Root.RemoveChild((*it)->GetCharacter());
                    it = m_Blocks.erase(it);
                }
                else {
                    ++it;
                }
            }

            // ==========================================
            // 🧱 打包地形碰撞箱 (給道具與火球用)
            // ==========================================
            std::vector<Rect> allObstacles = m_Collision.GetObstacles();
            for (const auto& block : m_Blocks) {
                Rect hit = block->GetHitbox();
                if (hit.width > 0) {
                    allObstacles.push_back(hit);
                }
            }

            // ==========================================
            // 🔥 火球物理更新與清理
            // ==========================================
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

            // ==========================================
            // 🍄 道具物理更新與吃相判定
            // ==========================================
            for (auto it = m_Items.begin(); it != m_Items.end(); ) {
                (*it)->Update(dt, m_WorldOffset, allObstacles);

                glm::vec2 pPos = m_Player->GetPosition();
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

            if (m_Castle) {
                m_Castle->SetPosition({ 9436.0f - m_WorldOffset, -145.0f });
            }

            // ==========================================================
            // 🚩 過關判定與旗桿動畫
            // ==========================================================
            if (m_Flagpole) {
                m_Flagpole->SetPosition({ 9147.0f - m_WorldOffset, 6.0f });
            }

            Rect marioRect = m_Player->GetRect(m_WorldOffset);
            Rect flagpoleRect;
            if (marioRect.height > 50.0f) {
                flagpoleRect = { 9180.0f, -250.0f, 50.0f, 500.0f };
            }
            else {
                flagpoleRect = { 9150.0f, -300.0f, 50.0f, 500.0f };
            }

            if (!m_IsLevelClear && CollisionHandler::CheckCollision(marioRect, flagpoleRect)) {
                m_IsLevelClear = true;
                m_IsFlagSliding = true;

                float hitY = m_Player->GetPosition().y;
                int flagScore = 100;
                if (hitY > -100) flagScore = 400;
                if (hitY > 0)    flagScore = 800;
                if (hitY > 100)  flagScore = 2000;
                if (hitY > 150)  flagScore = 5000;
                m_Score += flagScore;

                m_Player->StartFlagSlide(9147.0f - m_WorldOffset);
                m_Flag->SetPosition({ 9120.0f - m_WorldOffset, -20.0f });
                m_Player->GetCharacter()->SetPosition({ 9147.0f - m_WorldOffset - 10.0f, -20.0f + 50.0f });
            }

            if (m_Flag) {
                static bool isBigDuringSlide = false;
                static bool hasLockedSize = false;

                if (!m_IsFlagSliding && !m_IsLevelClear) {
                    m_Flag->SetPosition({ 9120.0f - m_WorldOffset, -20.0f });
                    hasLockedSize = false;
                }
                else if (m_IsFlagSliding) {
                    if (!hasLockedSize) {
                        isBigDuringSlide = (m_Player->GetRect(m_WorldOffset).height > 50.0f);
                        hasLockedSize = true;
                    }

                    glm::vec2 flagPos = m_Flag->GetPosition();
                    if (flagPos.y > -235.0f) {
                        float nextY = flagPos.y - (300.0f * dt);
                        m_Flag->SetPosition({ 9120.0f - m_WorldOffset, nextY });
                        m_Player->GetCharacter()->SetPosition({ 9147.0f - m_WorldOffset - 10.0f, nextY + 40.0f });
                    }
                    else {
                        m_IsFlagSliding = false;
                        std::vector<std::string> runAnims;

                        if (isBigDuringSlide) {
                            m_Player->GetCharacter()->SetPosition({ 9170.0f - m_WorldOffset, -216.0f });
                            runAnims = {
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run1.png",
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run2.png",
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run3.png"
                            };
                        }
                        else {
                            m_Player->GetCharacter()->SetPosition({ 9170.0f - m_WorldOffset, -240.0f });
                            runAnims = {
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run1.png",
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run2.png",
                                GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run3.png"
                            };
                        }
                        m_Player->GetCharacter()->SetAnimation(runAnims, 100);
                        m_Player->GetCharacter()->Play();
                    }
                }
            }

            if (m_IsLevelClear && !m_IsFlagSliding) {
                glm::vec2 pPos = m_Player->GetCharacter()->GetPosition();
                float castleDoorX = 9436.0f - m_WorldOffset;
                if (pPos.x < castleDoorX) {
                    m_Player->GetCharacter()->SetPosition({ pPos.x + (150.0f * dt), pPos.y });
                    m_Player->GetCharacter()->Play();
                    m_Player->GetCharacter()->m_Transform.scale.x = 3.0f;
                }
                else {
                    m_Player->GetCharacter()->SetVisible(false);
                }
            }

            // 💯 分數特效更新與清理
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

            // --- 🍄 栗子球與 🐢 烏龜分段生成邏輯 ---
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

            // --- 🍄 栗子球更新與互動判定 ---
            for (auto it = m_Goombas.begin(); it != m_Goombas.end(); ) {
                (*it)->Update(dt, m_WorldOffset, m_Collision);

                glm::vec2 pPos = m_Player->GetPosition();
                Rect marioScreenRect = { pPos.x - 18.0f, pPos.y - 25.0f, 36.0f, 20.0f };
                Rect goombaScreenRect = (*it)->GetRect(m_WorldOffset);

                // 🔥 偵測火球擊殺栗子球
                bool hitByFireball = false;
                for (auto& fb : m_Fireballs) {
                    Rect fbScreenRect = fb->GetRect();
                    fbScreenRect.x -= m_WorldOffset;

                    // 🌟 關鍵修正：加上 !fb->IsExploding()，爆炸中的火球不能再判定傷害！
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
                    // 🌟 刪除 continue; 讓迴圈能順利往下走！
                }
                // 🌟 加一個 else if，如果是被火球打死，就不會再被瑪利歐判定一次受傷
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

            // --- 🐢 烏龜更新與互動判定 ---
            for (auto& koopa : m_Koopatroopas) {
                koopa->Update(dt, m_WorldOffset, allObstacles);
                Rect koopaRect = koopa->GetRect(m_WorldOffset); // 烏龜回傳的是世界座標

                // 🔥 偵測火球擊殺烏龜
                bool hitByFireball = false;
                for (auto& fb : m_Fireballs) {
                    // 🌟 關鍵修正：加上 !fb->IsExploding()
                    if (!fb->IsDestroyed() && !fb->IsExploding() && CollisionHandler::CheckCollision(fb->GetRect(), koopaRect)) {
                        fb->Explode();
                        hitByFireball = true;
                        break;
                    }
                }
                if (hitByFireball && koopa->GetState() != Koopatroopa::State::DEAD) {
                    koopa->Stomp();
                    m_Score += 200;
                    float kX = koopaRect.x + 18.0f; // 烏龜本來就是世界座標，不用加 Offset
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

    // 🌟 最終繪製：這是整個畫面能被看到的關鍵！
    m_Root.Update();
}

void App::ResetLevel() {
    m_WorldOffset = 0.0f;
    m_GameTime = 400.0f;

    if (m_Player) {
        m_Player->ResetStatus();
        if (m_Player->GetCharacter()) {
            m_Player->GetCharacter()->SetPosition({ -300.0f, -264.0f });
            m_Player->GetCharacter()->SetVisible(true);
        }
    }

    if (m_Map) m_Map->Update(0.0f);

    // 🧹 第一階段：大掃除
    for (auto& goomba : m_Goombas) { if (goomba) m_Root.RemoveChild(goomba->GetDrawable()); }
    m_Goombas.clear();

    for (auto& koopa : m_Koopatroopas) { if (koopa) m_Root.RemoveChild(koopa); }
    m_Koopatroopas.clear();

    for (auto& effect : m_ScoreEffects) { if (effect) m_Root.RemoveChild(effect->GetDrawable()); }
    m_ScoreEffects.clear();

    for (auto& item : m_Items) { if (item) m_Root.RemoveChild(item); }
    m_Items.clear();

    // 🔥 清除殘留火球
    for (auto& fb : m_Fireballs) { if (fb) m_Root.RemoveChild(fb); }
    m_Fireballs.clear();

    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) m_Root.RemoveChild(block->GetCharacter());
    }
    m_Blocks.clear();

    if (m_Flagpole) {
        m_Flagpole->SetPosition({ 9147.0f, -95.0f });
        m_Flagpole->SetVisible(true);
    }
    if (m_Flag) {
        m_Flag->SetPosition({ 9110.0f, 150.0f });
        m_Flag->SetVisible(true);
    }
    if (m_Castle) m_Castle->SetVisible(true);

    m_SpawnPhase = 0;
    m_IsLevelClear = false;  // 重置過關狀態
    m_IsFlagSliding = false; // 重置拉旗子狀態

    // ✨ 第二階段：重生！
    LoadLevelObjects();

    // 🌟 第三階段：防呆機制
    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) block->GetCharacter()->SetVisible(true);
    }
    for (auto& item : m_Items) { if (item) item->SetVisible(true); }
}