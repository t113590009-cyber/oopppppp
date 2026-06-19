#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "Fireball.hpp"

void App::SetGameElementsVisible(bool visible) {
    if (m_Map) m_Map->SetVisible(visible);
    if (m_Castle) m_Castle->SetVisible(visible);
    if (m_Flagpole) m_Flagpole->SetVisible(visible);
    if (m_Flag) m_Flag->SetVisible(visible);

    if (m_CastleLvl2) m_CastleLvl2->SetVisible(visible);
    if (m_FlagpoleLvl2) m_FlagpoleLvl2->SetVisible(visible);
    if (m_FlagLvl2) m_FlagLvl2->SetVisible(visible);

    if (m_Player && m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(visible);
    for (auto& block : m_Blocks) if (block && block->GetCharacter()) block->GetCharacter()->SetVisible(visible);
    if (m_TopUI) m_TopUI->SetVisible(visible);
}

void App::Update() {
    float dt = Util::Time::GetDeltaTime();

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

    if (Util::Input::IsKeyDown(Util::Keycode::NUM_0)) {
        m_CheatEnabled = !m_CheatEnabled;
        ResetLevel(true);
    }

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

            UpdateBlocksAndItems(dt, allObstacles);
            UpdateEnemiesAndFireballs(dt, allObstacles, pPos);

            if (m_CurrentLevel == 3 && m_Level3Hammer && !m_Level3Hammer->IsDestroyed()) {
                m_Level3Hammer->Update(dt);
                m_Level3Hammer->RenderWithCamera(m_WorldOffset);

                auto marioBox = m_Player->GetRect(m_WorldOffset);
                HammerRect hammerBox = m_Level3Hammer->GetHitbox();

                bool isColliding = (marioBox.x < hammerBox.x + hammerBox.width &&
                    marioBox.x + marioBox.width > hammerBox.x &&
                    marioBox.y < hammerBox.y + hammerBox.height &&
                    marioBox.y + hammerBox.height > hammerBox.y);

                if (isColliding) {
                    m_Level3Hammer->Touch();

                    if (m_Toad) {
                        m_Toad->SetVisible(true);
                    }

                    m_IsLevelClear = true;
                }
            }
            if (m_CurrentLevel == 3 && m_Toad) {
                m_Toad->SetPosition({ m_ToadWorldX - m_WorldOffset, -225.0f });
            }

            if (m_CurrentLevel == 1) {
                HandleLevel1ClearAnimation(dt, pPos.x + m_WorldOffset);
            }
            else if (m_CurrentLevel == 2) {
                HandleLevel2ClearAnimation(dt, pPos.x + m_WorldOffset);
            }
            else if (m_CurrentLevel == 3) {
                HandleLevel3ClearAnimation(dt, pPos.x + m_WorldOffset);
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

void App::ResetLevel(bool keepPosition) {
    if (!keepPosition) {
        m_WorldOffset = 0.0f;
    }
    m_GameTime = 400.0f;

    if (m_Map) {
        m_Map->ShowLevel(m_CurrentLevel);
        m_Map->Update(m_WorldOffset);
    }

    for (auto& goomba : m_Goombas) { if (goomba) m_Root.RemoveChild(goomba->GetDrawable()); }
    m_Goombas.clear();
    for (auto& koopa : m_Koopatroopas) { if (koopa) m_Root.RemoveChild(koopa); }
    m_Koopatroopas.clear();

    for (auto& bowser : m_Bowsers) { if (bowser) m_Root.RemoveChild(bowser); }
    m_Bowsers.clear();
    for (auto& fire : m_BowserFires) { if (fire) m_Root.RemoveChild(fire); }
    m_BowserFires.clear();

    for (auto& fb : m_FireBars) {
        if (fb) {
            for (auto& anim : fb->GetDrawables()) {
                m_Root.RemoveChild(anim);
            }
        }
    }
    m_FireBars.clear();

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

    if (m_Level3Hammer && m_Level3Hammer->GetCharacter()) {
        m_Root.RemoveChild(m_Level3Hammer->GetCharacter());
    }
    m_Level3Hammer = nullptr;

    if (m_Toad) {
        m_Root.RemoveChild(m_Toad);
    }
    m_Toad = nullptr;

    m_Collision.ClearObstacles();

    m_SpawnPhase = 0;
    m_IsLevelClear = false;
    m_IsFlagSliding = false;
    m_LevelClearTimer = 0.0f;

    if (m_CurrentLevel == 1) {
        if (m_Player) {
            m_Player->ResetStatus();
            if (m_Player->GetCharacter()) {
                if (!keepPosition) {
                    m_Player->GetCharacter()->SetPosition({ -300.0f, -264.0f });
                }
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

        if (m_CastleLvl2) m_CastleLvl2->SetVisible(false);
        if (m_FlagpoleLvl2) m_FlagpoleLvl2->SetVisible(false);
        if (m_FlagLvl2) m_FlagLvl2->SetVisible(false);

        LoadLevelObjects();
    }
    else if (m_CurrentLevel == 2) {
        if (m_Player) {
            m_Player->ResetStatus();
            if (!keepPosition) m_Player->SetWorldPosition(-400.0f, -244.0f);
            if (m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(true);
        }

        if (m_Castle) m_Castle->SetVisible(false);
        if (m_Flagpole) m_Flagpole->SetVisible(false);
        if (m_Flag) m_Flag->SetVisible(false);

        if (m_CastleLvl2) {
            m_CastleLvl2->SetPosition({ 7104.0f, -145.0f });
            m_CastleLvl2->SetVisible(true);
        }
        if (m_FlagpoleLvl2) {
            m_FlagpoleLvl2->SetPosition({ 6867.0f, 6.0f });
            m_FlagpoleLvl2->SetVisible(true);
        }
        if (m_FlagLvl2) {
            m_FlagLvl2->SetPosition({ 6840.0f, 210.0f });
            m_FlagLvl2->SetVisible(true);
        }
        LoadLevel2Objects();
    }
    else if (m_CurrentLevel == 3) {
        if (m_Player) {
            m_Player->ResetStatus();
            if (!keepPosition) {
                m_Player->SetWorldPosition(-420.0f, -264.0f);
            }
            if (m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(true);
        }

        if (m_Castle) m_Castle->SetVisible(false);
        if (m_Flagpole) m_Flagpole->SetVisible(false);
        if (m_Flag) m_Flag->SetVisible(false);

        if (m_CastleLvl2) m_CastleLvl2->SetVisible(false);
        if (m_FlagpoleLvl2) m_FlagpoleLvl2->SetVisible(false);
        if (m_FlagLvl2) m_FlagLvl2->SetVisible(false);

        LoadLevel3Objects();

        float tileWidth = 16.0f * 3.0f;
        float tileHeight = 16.0f * 3.0f;
        float mapWidth = 16.0f * tileWidth;

        float hammerX = (8 * mapWidth) + (4 * tileWidth);
        float hammerY = -242.0f + (4 * tileHeight);

        m_Level3Hammer = std::make_shared<HammerProp>(hammerX, hammerY);

        if (m_Level3Hammer->GetCharacter()) {
            m_Root.AddChild(m_Level3Hammer->GetCharacter());
        }
        m_ToadWorldX = hammerX + 600.0f;

        m_Toad = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/toad/toadwin.png");
        m_Toad->SetZIndex(4);
        m_Toad->m_Transform.scale = { 3.0f, 3.0f };
        m_Toad->SetVisible(false);

        m_Root.AddChild(m_Toad);
    }

    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) block->GetCharacter()->SetVisible(true);
    }
    for (auto& item : m_Items) { if (item) item->SetVisible(true); }
}

void App::HandleLevel3ClearAnimation(float dt, float marioWorldX) {
    static bool s_AnimSet = false;
    static float s_StartX = 0.0f;

    static std::shared_ptr<Character> s_EndText = nullptr;

    if (!m_IsLevelClear) {
        s_AnimSet = false;

        if (s_EndText) {
            m_Root.RemoveChild(s_EndText);
            s_EndText = nullptr;
        }
        return;
    }

    if (!s_AnimSet) {
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

        s_StartX = marioWorldX;
        s_AnimSet = true;
    }

    float targetX = m_ToadWorldX - 70.0f;

    float groundY = (m_Player->IsBig() || m_Player->IsFire()) ? -216.0f : -240.0f;
    float bridgeGroundY = (m_Player->IsBig() || m_Player->IsFire()) ? -24.0f : -48.0f;
    float currentY = m_Player->GetCharacter()->GetPosition().y;

    if (marioWorldX < targetX) {
        float moveDist = 150.0f * dt;
        marioWorldX += moveDist;

        // 🌟 修正 1：加回這行！
        // 讓鏡頭跟著瑪利歐走，等到瑪利歐走到奇諾比奧面前停下時，鏡頭就會自然跟著鎖定了！
        m_WorldOffset += moveDist;

        if (marioWorldX > s_StartX + 150.0f) {
            if (currentY > groundY) {
                currentY -= 300.0f * dt;
                if (currentY < groundY) {
                    currentY = groundY;
                }
            }
        }
        else {
            if (currentY > bridgeGroundY) {
                currentY -= 300.0f * dt;
                if (currentY < bridgeGroundY) {
                    currentY = bridgeGroundY;
                }
            }
            else {
                currentY = bridgeGroundY;
            }
        }

        m_Player->GetCharacter()->SetPosition({ marioWorldX - m_WorldOffset, currentY });
        m_Player->GetCharacter()->m_Transform.scale.x = 3.0f;
    }
    else {
        m_Player->GetCharacter()->SetPosition({ targetX - m_WorldOffset, groundY });

        std::vector<std::string> idleAnim;
        if (m_Player->IsFire()) {
            idleAnim = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/stand.png" };
        }
        else if (m_Player->IsBig()) {
            idleAnim = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/stand.png" };
        }
        else {
            idleAnim = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/stand.png" };
        }
        m_Player->GetCharacter()->SetAnimation(idleAnim, 100);
        m_Player->GetCharacter()->Pause();

        if (!s_EndText) {
            s_EndText = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/end.png");
            s_EndText->SetZIndex(100);
            s_EndText->m_Transform.scale = { 3.0f, 3.0f };

            // 🌟 修正 2：文字向「右」移！
            // X 的數值「越大越靠右」。我先幫你設 200.0f，如果覺得還不夠右邊，可以自己改成 250.0f 或 300.0f！
            s_EndText->SetPosition({ 0.0f, 0.0f });

            m_Root.AddChild(s_EndText);
        }

        m_LevelClearTimer += dt;
        if (m_LevelClearTimer > 3.0f) {
        }
    }
}