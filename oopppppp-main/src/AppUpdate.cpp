#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "Fireball.hpp"

// 🌟 一鍵隱藏/顯示遊戲背景的小幫手
void App::SetGameElementsVisible(bool visible) {
    if (m_Map) m_Map->SetVisible(visible);
    if (m_Castle) m_Castle->SetVisible(visible);
    if (m_Flagpole) m_Flagpole->SetVisible(visible);
    if (m_Flag) m_Flag->SetVisible(visible);
    if (m_Player && m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(visible);
    for (auto& block : m_Blocks) if (block && block->GetCharacter()) block->GetCharacter()->SetVisible(visible);
    if (m_TopUI) m_TopUI->SetVisible(visible);
}

void App::Update() {
    float dt = Util::Time::GetDeltaTime();

    // ==========================================
    // 🌟 懶人載入法：遊戲一開先把 hp.png 與 fail.png 準備好
    // ==========================================
    if (!m_LifeBg) {
        m_LifeBg = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/hp.png");
        m_LifeBg->SetZIndex(98);
        m_LifeBg->m_Transform.scale = { 3.0f, 3.0f };
        m_LifeBg->SetPosition({ 0.0f, 0.0f });
        m_Root.AddChild(m_LifeBg);

        m_LifeWorldNum = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/1.png");
        m_LifeWorldNum->SetZIndex(99);
        m_Root.AddChild(m_LifeWorldNum);

        m_LifeStageNum = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/1.png");
        m_LifeStageNum->SetZIndex(99);
        m_Root.AddChild(m_LifeStageNum);

        m_LifeCountNum = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/3.png");
        m_LifeCountNum->SetZIndex(99);
        m_Root.AddChild(m_LifeCountNum);

        m_LifeBg->SetVisible(false);
        m_LifeWorldNum->SetVisible(false);
        m_LifeStageNum->SetVisible(false);
        m_LifeCountNum->SetVisible(false);
    }

    if (!m_FailScreen) {
        m_FailScreen = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/fail.png");
        m_FailScreen->SetZIndex(100);
        m_FailScreen->m_Transform.scale = { 3.0f, 3.0f };
        m_FailScreen->SetPosition({ 0.0f, 0.0f });
        m_Root.AddChild(m_FailScreen);
        m_FailScreen->SetVisible(false);
    }

    static bool s_ShowGameOver = false;
    static float s_GameOverTimer = 0.0f;

    if (Util::Input::IsKeyDown(Util::Keycode::NUM_2)) { m_CurrentLevel = 2; ResetLevel(); }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_1)) { m_CurrentLevel = 1; ResetLevel(); }
    if (Util::Input::IsKeyDown(Util::Keycode::NUM_3)) { m_CurrentLevel = 3; ResetLevel(); }

    // --- 1. 選單邏輯 ---
    if (m_Menu->GetVisibility()) {
        m_Menu->Update(dt);
        if (m_Menu->IsStartPressed()) {
            m_Menu->SetVisible(false);

            m_Lives = 3; m_Score = 0; m_Coins = 0; m_CurrentLevel = 1;

            m_ShowLifeScreen = true;
            m_LifeScreenTimer = 2.5f;
            SetGameElementsVisible(false);
        }
    }
    // --- 2. 💀 GAME OVER 畫面邏輯 ---
    else if (s_ShowGameOver) {
        s_GameOverTimer -= dt;

        if (s_GameOverTimer <= 0.0f) {
            s_ShowGameOver = false;
            m_FailScreen->SetVisible(false);

            m_CurrentState = State::START;
            m_Menu->SetVisible(true);

            ResetLevel();
            SetGameElementsVisible(false);
        }
    }
    // --- 3. 🌟 生命值過場邏輯 ---
    else if (m_ShowLifeScreen) {

        static bool s_isLifeUIReady = false;

        if (!s_isLifeUIReady) {
            s_isLifeUIReady = true;

            m_LifeBg->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/UI/hp.png"));
            m_LifeBg->m_Transform.scale = { 3.0f, 3.0f };
            m_LifeBg->SetPosition({ 0.0f, 0.0f });
            m_LifeBg->SetZIndex(98);
            m_LifeBg->SetVisible(true);

            m_LifeWorldNum->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/UI/top/1.png"));
            m_LifeWorldNum->m_Transform.scale = { 3.0f, 3.0f };
            m_LifeWorldNum->SetPosition({ 36.0f, 108.0f });
            m_LifeWorldNum->SetZIndex(99);
            m_LifeWorldNum->SetVisible(true);

            m_LifeStageNum->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/UI/top/" + std::to_string(m_CurrentLevel) + ".png"));
            m_LifeStageNum->m_Transform.scale = { 3.0f, 3.0f };
            m_LifeStageNum->SetPosition({ 84.0f, 108.0f });
            m_LifeStageNum->SetZIndex(99);
            m_LifeStageNum->SetVisible(true);

            m_LifeCountNum->SetDrawable(std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/UI/top/" + std::to_string(m_Lives) + ".png"));
            m_LifeCountNum->m_Transform.scale = { 3.0f, 3.0f };
            m_LifeCountNum->SetPosition({ 48.0f, 12.0f });
            m_LifeCountNum->SetZIndex(99);
            m_LifeCountNum->SetVisible(true);
        }

        m_LifeScreenTimer -= dt;

        if (m_LifeScreenTimer <= 0.0f) {
            m_ShowLifeScreen = false;
            s_isLifeUIReady = false;

            m_LifeBg->SetVisible(false);
            m_LifeWorldNum->SetVisible(false);
            m_LifeStageNum->SetVisible(false);
            m_LifeCountNum->SetVisible(false);

            SetGameElementsVisible(true);
            ResetLevel();
        }
    }
    // --- 4. 遊戲主迴圈邏輯 ---
    else {
        if (m_Player->IsDead()) {
            m_DeathTimer += dt;
            if (m_DeathTimer > 2.5f) {
                m_Lives--;
                m_DeathTimer = 0.0f;

                if (m_Lives > 0) {
                    m_ShowLifeScreen = true;
                    m_LifeScreenTimer = 2.5f;
                    SetGameElementsVisible(false);
                }
                else {
                    s_ShowGameOver = true;
                    s_GameOverTimer = 4.0f;
                    m_FailScreen->SetVisible(true);
                    SetGameElementsVisible(false);
                }
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

            std::vector<Rect> allObstacles = m_Collision.GetObstacles();
            for (auto& block : m_Blocks) {
                Rect hit = block->GetHitbox();
                if (hit.width > 0) allObstacles.push_back(hit);
            }

            // 🌟 將更新磚塊、道具與敵人的工作交給 AppUpdate_Entities.cpp 處理
            UpdateBlocksAndItems(dt, allObstacles);
            UpdateEnemiesAndFireballs(dt, allObstacles, pPos);

            if (m_CurrentLevel == 1) {
                HandleLevel1ClearAnimation(dt, pPos.x + m_WorldOffset);
            }

            for (auto it = m_ScoreEffects.begin(); it != m_ScoreEffects.end(); ) {
                (*it)->Update(dt, m_WorldOffset);
                if ((*it)->IsDone()) { m_Root.RemoveChild((*it)->GetDrawable()); it = m_ScoreEffects.erase(it); }
                else ++it;
            }
            for (int i = 0; i < (int)m_ActiveFireworks.size(); ) {
                auto& fw = m_ActiveFireworks[i];
                fw->SetPosition({ m_FireworkWorldX[i] - m_WorldOffset, fw->GetPosition().y });
                if (fw->IfAnimationEnds()) {
                    m_Root.RemoveChild(fw); m_ActiveFireworks.erase(m_ActiveFireworks.begin() + i); m_FireworkWorldX.erase(m_FireworkWorldX.begin() + i);
                }
                else ++i;
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
// 🔄 重置關卡邏輯
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