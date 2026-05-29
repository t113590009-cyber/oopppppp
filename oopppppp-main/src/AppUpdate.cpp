#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "Fireball.hpp"

void App::Update() {
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_2)) { m_CurrentLevel = 2; ResetLevel(); }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_1)) { m_CurrentLevel = 1; ResetLevel(); }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_3)) { m_CurrentLevel = 3; ResetLevel(); }

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
                if (block->GetCharacter()) block->GetCharacter()->SetVisible(true);
            }
            if (m_TopUI) m_TopUI->SetVisible(true);
        }
    }
    // --- 2. 遊戲主迴圈邏輯 ---
    else {
        if (m_Player->IsDead()) {
            m_DeathTimer += dt;
            if (m_DeathTimer > 2.5f) {
                m_Lives--;
                m_DeathTimer = 0.0f;
                if (m_Lives > 0) ResetLevel();
                else m_CurrentState = State::END;
            }
        }
        else {
            if (m_Player->GetPosition().y < -400.0f) m_Player->Die();

            if (m_CurrentState != State::END && !m_IsLevelClear) {
                m_GameTime -= dt * 2.5f;
                if (m_GameTime < 0.0f) m_GameTime = 0.0f;
            }

            if (m_TopUI) m_TopUI->Update(m_Score, m_Coins, static_cast<int>(m_GameTime), m_CurrentLevel);

            if (!m_IsLevelClear) m_Player->Update(m_WorldOffset, m_Collision, m_Blocks, dt);

            glm::vec2 pPos = m_Player->GetPosition();
            if (m_Player->IsOnGround()) m_ComboCount = 0;

            if (!m_IsLevelClear && m_Player->IsFire() && Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
                if (m_Fireballs.size() < 2) {
                    bool faceRight = (m_Player->GetCharacter()->m_Transform.scale.x > 0);
                    auto fireball = std::make_shared<Fireball>(m_WorldOffset + pPos.x, pPos.y, faceRight);
                    m_Root.AddChild(fireball);
                    m_Fireballs.push_back(fireball);
                }
            }

            // 🌟 收集所有碰撞箱
            std::vector<Rect> allObstacles = m_Collision.GetObstacles();
            for (const auto& block : m_Blocks) {
                Rect hit = block->GetHitbox();
                if (hit.width > 0) allObstacles.push_back(hit);
            }

            // 🌟 呼叫瘦身後的函式：更新物件與敵人
            UpdateBlocksAndItems(dt, allObstacles);
            UpdateEnemiesAndFireballs(dt, allObstacles, pPos);

            // 🌟 呼叫瘦身後的函式：過關動畫
            if (m_CurrentLevel == 1) {
                HandleLevel1ClearAnimation(dt, pPos.x + m_WorldOffset);
            }

            // 更新特效與殘留煙火
            for (auto it = m_ScoreEffects.begin(); it != m_ScoreEffects.end(); ) {
                (*it)->Update(dt, m_WorldOffset);
                if ((*it)->IsDone()) { m_Root.RemoveChild((*it)->GetDrawable()); it = m_ScoreEffects.erase(it); }
                else ++it;
            }
            for (int i = 0; i < (int)m_ActiveFireworks.size(); ) {
                auto& fw = m_ActiveFireworks[i];
                fw->SetPosition({ m_FireworkWorldX[i] - m_WorldOffset, fw->GetPosition().y });
                if (fw->IfAnimationEnds()) {
                    m_Root.RemoveChild(fw);
                    m_ActiveFireworks.erase(m_ActiveFireworks.begin() + i);
                    m_FireworkWorldX.erase(m_FireworkWorldX.begin() + i);
                } else ++i;
            }

            if (m_Map) m_Map->Update(m_WorldOffset);
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    m_Root.Update();
}

// ==========================================
// 重置關卡邏輯保持不動
// ==========================================
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
    m_LevelClearTimer = 0.0f;

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
            if (m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(true);
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
            if (m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(true);
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