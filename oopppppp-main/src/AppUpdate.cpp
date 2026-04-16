#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

// 🌟 記得要把這兩個道具的標頭檔都引進來！
#include "Mushroom.hpp"
#include "Star.hpp"

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

            for (auto& block : m_Blocks) {
                if (block->GetCharacter()) {
                    block->GetCharacter()->SetVisible(true);
                }
            }
        }
    }
    // --- 2. 遊戲邏輯 ---
    else {
        // 更新玩家物理 (包含傳送門與磚塊碰撞)
        m_Player->Update(m_WorldOffset, m_Collision, m_Blocks, dt);

        // ==========================================
        // 🌟 融合一：偵測瑪利歐死亡，並加上延遲再顯示 FAIL 畫面
        // ==========================================
        if (m_Player->IsDead() && m_FailScreen) {
            m_DeathTimer += dt; // 開始計時

            // 延遲 1.5 秒顯示 FAIL 畫面 (你可以依照動畫長短調整)
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
        // 🎁 融合二：道具系統 - 根據方塊內容物生成對應道具！
        // ==========================================
        for (auto& block : m_Blocks) {
            if (block->HasJustSpawnedItem()) {

                Block::ItemType type = block->GetItemType(); // 🌟 看看肚子裡裝什麼

                if (type == Block::ItemType::MUSHROOM) {
                    auto mushroom = std::make_shared<Mushroom>(block->GetPosition().x, block->GetPosition().y);
                    m_Root.AddChild(mushroom);
                    m_Items.push_back(std::move(mushroom));
                }
                else if (type == Block::ItemType::STAR) {
                    // 🌟 如果是星星，就生出星星！
                    auto star = std::make_shared<Star>(block->GetPosition().x, block->GetPosition().y);
                    m_Root.AddChild(star);
                    m_Items.push_back(std::move(star));
                }
            }
        }

        // ==========================================
        // 🌟 融合二：把地板水管跟天空磚塊打包成總名單 (給道具物理用)
        // ==========================================
        std::vector<Rect> allObstacles = m_Collision.GetObstacles();
        for (const auto& block : m_Blocks) {
            Rect hit = block->GetHitbox();
            if (hit.width > 0) {
                allObstacles.push_back(hit);
            }
        }

        // ==========================================
        // 🍄 融合二：道具物理更新與瑪利歐吃道具判定
        // ==========================================
        for (auto it = m_Items.begin(); it != m_Items.end(); ) {

            // 讓道具 (蘑菇或星星) 在磚塊上走動或彈跳
            (*it)->Update(dt, m_WorldOffset, allObstacles);

            Rect marioRect = m_Player->GetFeetRect(m_WorldOffset);
            Rect itemRect = (*it)->GetRect(m_WorldOffset);

            // 碰到道具的判定
            if (CollisionHandler::CheckCollision(marioRect, itemRect)) {
                (*it)->ApplyEffect(m_Player.get()); // 呼叫變大 (蘑菇) 或 無敵 (星星)！
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

                // 🌟 保護機制：瑪利歐活著才算碰撞
                if (!m_Player->IsDead()) {
                    // 🌟 終極無敵判定：如果是星星狀態，直接秒殺怪物！
                    if (m_Player->IsStarMode()) {
                        (*it)->Stomp();
                    }
                    else {
                        float marioBottom = pPos.y - 25.0f;
                        float goombaCenter = goombaScreenRect.y + (goombaScreenRect.height / 2.0f);

                        // 判斷是否踩在怪物頭上
                        if (m_Player->GetVelocityY() < 0.0f && marioBottom > goombaCenter) {
                            (*it)->Stomp();
                        }
                        else {
                            m_Player->TakeDamage(); // 呼叫 TakeDamage 而非直接死掉，支援大瑪利歐降級！
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