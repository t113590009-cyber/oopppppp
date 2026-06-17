#include "App.hpp"

void App::HandleLevel1ClearAnimation(float dt, float marioWorldX) {
    if (m_Castle) m_Castle->SetPosition({ 9436.0f - m_WorldOffset, -145.0f });
    if (m_Flagpole) m_Flagpole->SetPosition({ 9147.0f - m_WorldOffset, 6.0f });
    if (m_Flag && !m_IsLevelClear) m_Flag->SetPosition({ 9120.0f - m_WorldOffset, 210.0f });

    // 碰到旗桿的瞬間
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

    // 往下滑動中
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

    // 走向城堡與放煙火
    if (m_IsLevelClear && !m_IsFlagSliding) {
        if (m_WorldOffset < 9250.0f) m_WorldOffset += 150.0f * dt;

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

                        std::vector<std::string> boomImgs = { GA_RESOURCE_DIR"/Image/Items/fireball/boom1.png", GA_RESOURCE_DIR"/Image/Items/fireball/boom2.png", GA_RESOURCE_DIR"/Image/Items/fireball/boom3.png" };
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
                else if (m_ActiveFireworks.empty()) {
                    m_LevelClearTimer += dt;
                    if (m_LevelClearTimer >= 2.0f) {
                        // 🌟 攔截點：煙火放完等兩秒後，不要直接進入下一關！
                        // 我們在這裡改變關卡，但將主控權交給 AppUpdate 的過場系統
                        m_CurrentLevel = 2;
                        m_ShowLifeScreen = true;
                        m_LifeScreenTimer = 2.5f;
                        SetGameElementsVisible(false); // 隱藏後面已經過完的背景
                    }
                }
            }
        }
    }
}
void App::HandleLevel2ClearAnimation(float dt, float marioWorldX) {
    // 🌟 距離校正：城堡在 7300，旗桿在 6868，旗子在 6845
    if (m_CastleLvl2) m_CastleLvl2->SetPosition({ 7150.0f - m_WorldOffset, -120.0f });
    if (m_FlagpoleLvl2) m_FlagpoleLvl2->SetPosition({ 6868.0f - m_WorldOffset, 6.0f });
    if (m_FlagLvl2 && !m_IsLevelClear) m_FlagLvl2->SetPosition({ 6845.0f - m_WorldOffset, 210.0f });

    // 🌟 修正：配合旗竿 6868.0f 的合理範圍 (6861.0f ~ 6876.0f)
    if (!m_IsLevelClear && marioWorldX >= 6861.0f && marioWorldX <= 6876.0f) {
        m_IsLevelClear = true;
        m_IsFlagSliding = true;

        int timeDigit = static_cast<int>(m_GameTime) % 10;
        if (timeDigit == 1 || timeDigit == 3 || timeDigit == 6) { m_PendingFireworks = timeDigit; }
        else { m_PendingFireworks = 0; }
        m_FireworkTimer = 0.0f;

        float hitY = m_Player->GetCharacter()->GetPosition().y;
        int flagScore = 100;
        if (hitY > -100) flagScore = 400;
        if (hitY > 0)    flagScore = 800;
        if (hitY > 100)  flagScore = 2000;
        if (hitY > 150)  flagScore = 5000;
        m_Score += flagScore;

        // 🌟 修正：分數特效的位置也要跟著旗竿改到 6868.0f
        auto flagScoreFx = std::make_shared<ScoreEffect>(flagScore, 6868.0f - m_WorldOffset + 30.0f, hitY);
        m_Root.AddChild(flagScoreFx->GetDrawable());
        m_ScoreEffects.push_back(flagScoreFx);

        // 🌟 修正：讓瑪利歐在 6868.0f 的旗竿上開始下滑
        m_Player->StartFlagSlide(6868.0f - m_WorldOffset);
        m_Player->GetCharacter()->SetPosition({ 6868.0f - m_WorldOffset - 10.0f, hitY });
    }

    // 🌟 修正：下滑前或定住時，旗子 X 軸要鎖定在 6845.0f
    if (m_IsLevelClear && m_FlagLvl2 && !m_IsFlagSliding) {
        m_FlagLvl2->SetPosition({ 6845.0f - m_WorldOffset, m_FlagLvl2->GetPosition().y });
    }

    // 往下滑動中
    if (m_FlagLvl2 && m_IsFlagSliding) {
        bool flagReachedBottom = false;
        bool marioReachedBottom = false;

        glm::vec2 flagPos = m_FlagLvl2->GetPosition();
        if (flagPos.y > -185.0f) {
            // 🌟 修正：下滑時，旗子的 X 軸要固定在 6845.0f，不可以寫舊的 6984.0f！
            m_FlagLvl2->SetPosition({ 6845.0f - m_WorldOffset, flagPos.y - (300.0f * dt) });
        }
        else {
            m_FlagLvl2->SetPosition({ 6845.0f - m_WorldOffset, -185.0f }); // 確保定死在底部
            flagReachedBottom = true;
        }

        glm::vec2 marioCharPos = m_Player->GetCharacter()->GetPosition();
        float groundY = (m_Player->IsBig() || m_Player->IsFire()) ? -216.0f : -240.0f;

        if (marioCharPos.y > groundY) {
            // 🌟 修正：瑪利歐下滑時的 X 軸要固定在 6868.0f
            m_Player->GetCharacter()->SetPosition({ 6868.0f - m_WorldOffset - 10.0f, marioCharPos.y - (300.0f * dt) });
        }
        else {
            m_Player->GetCharacter()->SetPosition({ 6868.0f - m_WorldOffset - 10.0f, groundY });
            marioReachedBottom = true;
        }

        if (flagReachedBottom && marioReachedBottom) {
            m_IsFlagSliding = false;
            m_ClearGroundY = groundY;

            std::vector<std::string> runAnims;
            if (m_Player->IsFire()) { runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/fire/big/run3.png" }; }
            else if (m_Player->IsBig()) { runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/big/run3.png" }; }
            else { runAnims = { GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run1.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run2.png", GA_RESOURCE_DIR"/Image/Character/mario/normal/small/run3.png" }; }
            m_Player->GetCharacter()->SetAnimation(runAnims, 100);
            m_Player->GetCharacter()->Play();
        }
    }

    // 走向城堡與放煙火
    if (m_IsLevelClear && !m_IsFlagSliding) {
        if (m_WorldOffset < 7114.0f) m_WorldOffset += 150.0f * dt; // 鏡頭限制在 7114

        float doorWorldX = 7150.0f;
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

                        std::vector<std::string> boomImgs = { GA_RESOURCE_DIR"/Image/Items/fireball/boom1.png", GA_RESOURCE_DIR"/Image/Items/fireball/boom2.png", GA_RESOURCE_DIR"/Image/Items/fireball/boom3.png" };
                        auto fw = std::make_shared<AnimatedCharacter>(boomImgs);
                        fw->SetAnimation(boomImgs, 150);
                        fw->m_Transform.scale = { 4.0f, 4.0f };

                        float offsetX = (m_PendingFireworks % 2 == 0) ? 60.0f : -40.0f;
                        float fwY = 50.0f + (m_PendingFireworks * 40.0f);
                        float fwWorldX = 7300.0f + offsetX;

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
                else if (m_ActiveFireworks.empty()) {
                    m_LevelClearTimer += dt;
                    if (m_LevelClearTimer >= 2.0f) {
                        // 🌟 過關後，自動跳到第 3 關！
                        m_CurrentLevel = 3;
                        m_ShowLifeScreen = true;
                        m_LifeScreenTimer = 2.5f;
                        SetGameElementsVisible(false);
                    }
                }
            }
        }
    }
}