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

            // 🚩 新增：按 Enter 後顯示旗桿與旗子
            if (m_Flagpole) m_Flagpole->SetVisible(true);
            if (m_Flag) m_Flag->SetVisible(true);

            for (auto& block : m_Blocks) {
                if (block->GetCharacter()) {
                    block->GetCharacter()->SetVisible(true);
                }
            }

            // 🌟 按下 Enter 進遊戲時，讓計分板 UI 顯示出來！
            if (m_TopUI) m_TopUI->SetVisible(true);
        }
    }
    // --- 2. 遊戲邏輯 ---
    else {
        // ==========================================
        // ⏰ 時間倒數與 UI 更新
        // ==========================================
        // 瑪利歐遊戲裡的時間流逝速度比現實快大約 2.5 倍
        if (m_CurrentState != State::END && !m_Player->IsDead()) {
            m_GameTime -= dt * 2.5f;
            if (m_GameTime < 0.0f) m_GameTime = 0.0f;
            // TODO: 之後可以加上時間到 0 死掉的邏輯
        }

        // 🌟 每一幀都把最新的數值餵給 UI 讓它換圖片
        if (m_TopUI) {
            m_TopUI->Update(m_Score, m_Coins, static_cast<int>(m_GameTime));
        }

        // ==========================================
        // 🏃‍♂️ 玩家物理更新 (包含傳送門與磚塊碰撞)
        // ==========================================
        m_Player->Update(m_WorldOffset, m_Collision, m_Blocks, dt);

        // 🌟 落地時重置踩踏連擊！
        if (m_Player->IsOnGround()) {
            m_ComboCount = 0;
        }

        // 🌟 建立經典的踩踏連擊分數表 (-1 代表 1UP)
        const int STOMP_SCORES[] = { 100, 200, 400, 500, 800, 1000, 2000, 4000, 8000, -1 };

        // ==========================================
        // 💀 偵測瑪利歐死亡，並加上延遲再顯示 FAIL 畫面
        // ==========================================
        if (m_Player->IsDead() && m_FailScreen) {
            m_DeathTimer += dt; // 開始計時

            // 延遲 1.5 秒顯示 FAIL 畫面
            if (m_DeathTimer > 1.5f) {
                m_FailScreen->SetVisible(true);
            }
        }

        // --- 🧱 磚塊更新與破壞判定 ---
        for (auto it = m_Blocks.begin(); it != m_Blocks.end(); ) {
            (*it)->Update(dt, m_WorldOffset);

            if ((*it)->IsDestroyed()) {
                m_Root.RemoveChild((*it)->GetCharacter());
                it = m_Blocks.erase(it);
            }
            else {
                ++it;
            }
        }

        // ==========================================
        // 🎁 道具生成：根據方塊內容物生成對應道具
        // ==========================================
        for (auto& block : m_Blocks) {
            if (block->HasJustSpawnedItem()) {
                Block::ItemType type = block->GetItemType();

                if (type == Block::ItemType::MUSHROOM) {
                    auto mushroom = std::make_shared<Mushroom>(block->GetPosition().x, block->GetPosition().y);
                    m_Root.AddChild(mushroom);
                    m_Items.push_back(std::move(mushroom));
                }
                else if (type == Block::ItemType::STAR) {
                    auto star = std::make_shared<Star>(block->GetPosition().x, block->GetPosition().y);
                    m_Root.AddChild(star);
                    m_Items.push_back(std::move(star));
                }
                else if (type == Block::ItemType::COIN) {
                    // 生成方塊金幣 (type = 0)
                    auto coin = std::make_shared<Coin>(block->GetPosition().x, block->GetPosition().y, 0);
                    m_Root.AddChild(coin);
                    m_Items.push_back(std::move(coin));

                    m_Score += 200; // 頂出金幣加 200 分
                    m_Coins += 1;   // 金幣數加 1
                    if (m_Coins >= 100) {
                        m_Coins = 0;
                        m_Lives += 1; // 滿百加命 (1UP)
                    }

                    // 噴出金幣的同時，在方塊頭上噴出 200 分！
                    auto score = std::make_shared<ScoreEffect>(200, block->GetPosition().x, block->GetPosition().y + 50.0f);
                    m_Root.AddChild(score->GetDrawable());
                    m_ScoreEffects.push_back(score);
                }
            }
        }

        // ==========================================
        // 🧱 打包地形碰撞箱 (給道具物理用)
        // ==========================================
        std::vector<Rect> allObstacles = m_Collision.GetObstacles();
        for (const auto& block : m_Blocks) {
            Rect hit = block->GetHitbox();
            if (hit.width > 0) {
                allObstacles.push_back(hit);
            }
        }

        // ==========================================
        // 🍄 道具更新與瑪利歐「吃相」判定
        // ==========================================
        for (auto it = m_Items.begin(); it != m_Items.end(); ) {

            // 讓道具 (蘑菇/星星/金幣) 更新狀態
            (*it)->Update(dt, m_WorldOffset, allObstacles);

            // 取得瑪利歐的「身體」碰撞箱
            glm::vec2 pPos = m_Player->GetPosition();
            Rect marioBodyRect = { m_WorldOffset + pPos.x - 18.0f, pPos.y - 24.0f, 36.0f, 48.0f };
            Rect itemRect = (*it)->GetRect(m_WorldOffset);

            // 瑪利歐的身體碰到道具了！
            if (CollisionHandler::CheckCollision(marioBodyRect, itemRect)) {
                (*it)->ApplyEffect(m_Player.get()); // 呼叫效果

                // 🌟 根據吃到的不同道具，給予對應的經典分數！
                if ((*it)->IsDestroyed()) {
                    int earnedScore = 0;

                    if (std::dynamic_pointer_cast<Coin>(*it)) {
                        earnedScore = 200;
                        m_Coins += 1;
                        if (m_Coins >= 100) {
                            m_Coins = 0;
                            m_Lives += 1; // 滿百加命 (1UP)
                        }
                    }
                    else if (std::dynamic_pointer_cast<Mushroom>(*it)) earnedScore = 1000;
                    else if (std::dynamic_pointer_cast<Star>(*it)) earnedScore = 1000;

                    if (earnedScore > 0) {
                        m_Score += earnedScore;
                        auto score = std::make_shared<ScoreEffect>(earnedScore, itemRect.x + 15.0f, itemRect.y + 40.0f);
                        m_Root.AddChild(score->GetDrawable());
                        m_ScoreEffects.push_back(score);
                    }
                }
            }

            // 清理已經吃掉 (或掉下深淵) 的道具
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

        // ==========================================
        // 🚩 更新旗桿與旗子的位置，讓它們跟著捲動
        // ==========================================
        if (m_Flagpole) {
            m_Flagpole->SetPosition({ 9147.0f - m_WorldOffset, -95.0f });
        }
        if (m_Flag) {
            // 🌟 旗子只更新 X 座標讓它捲動，Y 座標保留原本的高度 (這樣之後才能實作滑下來)
            m_Flag->SetPosition({ 9120.0f - m_WorldOffset, m_Flag->GetPosition().y });
        }

        // ==========================================
        // 💯 分數特效更新與清理
        // ==========================================
        for (auto it = m_ScoreEffects.begin(); it != m_ScoreEffects.end(); ) {
            (*it)->Update(dt, m_WorldOffset);

            // 如果時間到了，就從畫面上移除並刪除記憶體
            if ((*it)->IsDone()) {
                m_Root.RemoveChild((*it)->GetDrawable());
                it = m_ScoreEffects.erase(it);
            }
            else {
                ++it;
            }
        }

        // --- 🍄 栗子球分段生成邏輯 ---
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

        // --- 🍄 栗子球更新與踩踏判定 ---
        for (auto it = m_Goombas.begin(); it != m_Goombas.end(); ) {

            (*it)->Update(dt, m_WorldOffset, m_Collision);

            glm::vec2 pPos = m_Player->GetPosition();
            Rect marioScreenRect = { pPos.x - 18.0f, pPos.y - 25.0f, 36.0f, 20.0f };
            Rect goombaScreenRect = (*it)->GetRect(m_WorldOffset);

            if ((*it)->GetState() == Goomba::State::WALKING &&
                CollisionHandler::CheckCollision(marioScreenRect, goombaScreenRect)) {

                // 保護機制：瑪利歐活著才算碰撞
                if (!m_Player->IsDead()) {
                    // 🌟 終極無敵判定：如果是星星狀態，直接秒殺怪物！
                    if (m_Player->IsStarMode()) {
                        (*it)->Stomp();

                        m_Score += 100; // 星星撞死怪物加 100 分

                        float gX = goombaScreenRect.x + m_WorldOffset + 18.0f;
                        float gY = goombaScreenRect.y + 24.0f;
                        auto score = std::make_shared<ScoreEffect>(100, gX, gY);
                        m_Root.AddChild(score->GetDrawable());
                        m_ScoreEffects.push_back(score);
                    }
                    else {
                        float marioBottom = pPos.y - 25.0f;
                        float goombaCenter = goombaScreenRect.y + (goombaScreenRect.height / 2.0f);

                        // 判斷是否踩在怪物頭上
                        if (m_Player->GetVelocityY() < 0.0f && marioBottom > goombaCenter) {
                            (*it)->Stomp();

                            // 🌟 取得連擊分數 (最高鎖定在陣列的第 9 格，也就是 1UP)
                            int scoreValue = STOMP_SCORES[std::min(m_ComboCount, 9)];
                            m_ComboCount++; // 連擊數 +1

                            if (scoreValue == -1) {
                                m_Lives += 1; // 1UP 獎勵一隻命！
                            }
                            else {
                                m_Score += scoreValue; // 正常加分
                            }

                            // 🌟 噴出分數特效！(ScoreEffect 收到 -1 會自動載入 1up.png)
                            float gX = goombaScreenRect.x + m_WorldOffset + 18.0f;
                            float gY = goombaScreenRect.y + 24.0f;
                            auto score = std::make_shared<ScoreEffect>(scoreValue, gX, gY);
                            m_Root.AddChild(score->GetDrawable());
                            m_ScoreEffects.push_back(score);
                        }
                        else {
                            m_Player->TakeDamage();
                        }
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

        if (m_Map) m_Map->Update(m_WorldOffset);
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}