#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

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

            // 🌟 城堡顯示：當遊戲開始時，讓城堡出現
            if (m_Castle) m_Castle->SetVisible(true);

            // 遊戲開始時，把所有磚塊顯示出來
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

        // --- 🧱 磚塊更新與破壞判定 ---
        for (auto it = m_Blocks.begin(); it != m_Blocks.end(); ) {
            (*it)->Update(dt, m_WorldOffset);

            // 如果磚塊碎了，就從畫面上移除
            if ((*it)->IsDestroyed()) {
                m_Root.RemoveChild((*it)->GetCharacter());
                it = m_Blocks.erase(it);
            }
            else {
                ++it;
            }
        }

        // 你最新的城堡座標
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

            // 🌟 傳入 m_Collision 讓栗子球看得到水管
            (*it)->Update(dt, m_WorldOffset, m_Collision);

            glm::vec2 pPos = m_Player->GetPosition();
            Rect marioScreenRect = { pPos.x - 18.0f, pPos.y - 25.0f, 36.0f, 20.0f };
            Rect goombaScreenRect = (*it)->GetRect(m_WorldOffset);

            if ((*it)->GetState() == Goomba::State::WALKING &&
                CollisionHandler::CheckCollision(marioScreenRect, goombaScreenRect)) {

                float marioBottom = pPos.y - 25.0f;
                float goombaCenter = goombaScreenRect.y + (goombaScreenRect.height / 2.0f);

                if (m_Player->GetVelocityY() < 0.0f && marioBottom > goombaCenter) {
                    (*it)->Stomp();
                } else {
                    // 撞到受傷邏輯留白
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