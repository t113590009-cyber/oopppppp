#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Character.hpp"
#include "Util/Renderer.hpp"
#include <vector>
#include <memory>
#include <string>

class Map {
public:
    Map() = default;

    // ➡️ 初始化：在遊戲啟動時「一次性」載入所有關卡資源，拒絕重複載入造成破圖
    void Init(Util::Renderer& renderer) {
        float globalScale = 3.0f;

        // --- ⬇️ 1. 載入 1-1 的地表背景 ⬇️ ---
        float currentX1 = -380.0f;
        for (int i = 1; i <= 14; ++i) {
            std::string path = GA_RESOURCE_DIR"/Image/Background/stage1-1/1-1-" + std::to_string(i) + ".png";
            auto stage = std::make_shared<Character>(path);

            stage->m_Transform.scale = { globalScale, globalScale };
            stage->SetZIndex(0); // 背景在最底層
            stage->SetVisible(false);

            float originalWidth = (i == 14) ? 48.0f : 256.0f;
            float scaledWidth = originalWidth * globalScale;

            stage->SetPosition({ currentX1 + (scaledWidth / 2.0f), 0.0f });
            m_OriginalXCoords.push_back(currentX1 + (scaledWidth / 2.0f));

            currentX1 += scaledWidth;
            renderer.AddChild(stage);
            m_BackgroundStages.push_back(stage);
        }

        // --- ⬇️ 2. 載入 1-1 的地下室背景與水管 ⬇️ ---
        m_UndergroundBackground = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Background/stage1-1/1-1-under.png");
        m_UndergroundBackground->m_Transform.scale = { globalScale, globalScale };
        m_UndergroundBackground->SetZIndex(0);
        m_UndergroundBackground->SetVisible(false);
        m_UndergroundOriginalX = 15384.0f;
        m_UndergroundBackground->SetPosition({ m_UndergroundOriginalX, 0.0f });
        renderer.AddChild(m_UndergroundBackground);

        m_UndergroundPipe = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Environment/stage1-1/water pipe under.png");
        m_UndergroundPipe->m_Transform.scale = { globalScale, globalScale };
        m_UndergroundPipe->SetZIndex(15);
        m_UndergroundPipe->SetVisible(false);
        m_UndergroundPipeOriginalX = 15696.0f;
        m_UndergroundPipe->SetPosition({ m_UndergroundPipeOriginalX, 0.0f });
        renderer.AddChild(m_UndergroundPipe);

        // --- ⬇️ 3. 載入 1-2 的背景圖片邏輯 ⬇️ ---
        float currentX2 = -450.0f; // 1-2 從城堡門口 (-450) 開始排列（可依需求微調此數值）
        for (int i = 1; i <= 11; ++i) {
            std::string path2 = GA_RESOURCE_DIR"/Image/Background/stage1-2/1-2-" + std::to_string(i) + ".png";
            auto stage2 = std::make_shared<Character>(path2);

            stage2->m_Transform.scale = { globalScale, globalScale };
            stage2->SetZIndex(0);
            stage2->SetVisible(false);

            // ✨ 核心修正：判斷第 11 張（最後一張）特殊寬度，防止圖片邊緣出現留白黑條
            float originalWidth2 = (i == 11) ? 48.0f : 256.0f;
            float scaledWidth2 = originalWidth2 * globalScale;

            stage2->SetPosition({ currentX2 + (scaledWidth2 / 2.0f), 0.0f });
            m_Level2OriginalXCoords.push_back(currentX2 + (scaledWidth2 / 2.0f));

            currentX2 += scaledWidth2;
            renderer.AddChild(stage2);
            m_Level2Backgrounds.push_back(stage2);
        }
    }

    // ➡️ 更新：根據 worldOffset 移動所有背景（✨ 關鍵修正：兩關都要隨著鏡頭移動！）
    void Update(float worldOffset) {
        // 更新 1-1 地表背景位置
        for (size_t i = 0; i < m_BackgroundStages.size(); ++i) {
            m_BackgroundStages[i]->SetPosition({ m_OriginalXCoords[i] - worldOffset, 0.0f });
        }

        // ✨ 核心修正：讓第二關背景也能跟著鏡頭捲動！
        for (size_t i = 0; i < m_Level2Backgrounds.size(); ++i) {
            m_Level2Backgrounds[i]->SetPosition({ m_Level2OriginalXCoords[i] - worldOffset, 0.0f });
        }

        // 更新 1-1 地下室背景與水管
        if (m_UndergroundBackground) {
            m_UndergroundBackground->SetPosition({ m_UndergroundOriginalX - worldOffset, 0.0f });
        }
        if (m_UndergroundPipe) {
            m_UndergroundPipe->SetPosition({ m_UndergroundPipeOriginalX - worldOffset, 0.0f });
        }
    }

    // ➡️ 顯示狀態切換（防呆相容原本選單舊 code）
    void SetVisible(bool visible) {
        if (!visible) {
            HideAll();
        } else {
            ShowLevel(m_CurrentLevel);
        }
    }

    // ➡️ 大掃除：隱藏所有關卡背景
    void HideAll() {
        for (auto& stage : m_BackgroundStages) {
            if (stage) stage->SetVisible(false);
        }
        if (m_UndergroundBackground) m_UndergroundBackground->SetVisible(false);
        if (m_UndergroundPipe) m_UndergroundPipe->SetVisible(false);

        for (auto& stage : m_Level2Backgrounds) {
            if (stage) stage->SetVisible(false);
        }
    }

    // ➡️ 根據關卡編號顯示對應地圖（切換關卡的核心控制）
    void ShowLevel(int level) {
        m_CurrentLevel = level;
        HideAll(); // 先全部大掃除隱藏起來！

        if (level == 1) {
            // 顯示 1-1 地表
            for (auto& stage : m_BackgroundStages) {
                if (stage) stage->SetVisible(true);
            }
            // 顯示 1-1 地下室
            if (m_UndergroundBackground) m_UndergroundBackground->SetVisible(true);
            if (m_UndergroundPipe) m_UndergroundPipe->SetVisible(true);
        }
        else if (level == 2) {
            // 顯示 1-2 背景
            for (auto& stage : m_Level2Backgrounds) {
                if (stage) stage->SetVisible(true);
            }
        }
    }

private:
    int m_CurrentLevel = 1; // 紀錄當前關卡

    // 1-1 專用變數
    std::vector<std::shared_ptr<Character>> m_BackgroundStages;
    std::vector<float> m_OriginalXCoords;

    // 地下室變數
    std::shared_ptr<Character> m_UndergroundBackground;
    float m_UndergroundOriginalX = 0.0f;
    std::shared_ptr<Character> m_UndergroundPipe;
    float m_UndergroundPipeOriginalX = 0.0f;

    // 1-2 專用變數
    std::vector<float> m_Level2OriginalXCoords;
    std::vector<std::shared_ptr<Character>> m_Level2Backgrounds;
};

#endif // MAP_HPP