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

    // ➡️ 初始化：載入 14 張圖並排好位置
    void Init(Util::Renderer& renderer) {
        float currentX = -380.0f;
        float globalScale = 3.0f;

        for (int i = 1; i <= 14; ++i) {
            std::string path = GA_RESOURCE_DIR"/Image/Background/stage1-1/1-1-" + std::to_string(i) + ".png";
            auto stage = std::make_shared<Character>(path);

            stage->m_Transform.scale = { globalScale, globalScale };
            stage->SetZIndex(0); // 背景在最底層
            stage->SetVisible(false);

            // 手動寬度對齊邏輯
            float originalWidth = (i == 14) ? 48.0f : 256.0f;
            float scaledWidth = originalWidth * globalScale;

            stage->SetPosition({ currentX + (scaledWidth / 2.0f), 0.0f });
            m_OriginalXCoords.push_back(currentX + (scaledWidth / 2.0f));

            currentX += scaledWidth; // 累加寬度，拼下一張圖
            renderer.AddChild(stage);
            m_BackgroundStages.push_back(stage);
        }

        // 🌟 新增：載入地下室背景 (1-1-under.png)
        m_UndergroundBackground = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Background/stage1-1/1-1-under.png"); // 請確認路徑正確！
        m_UndergroundBackground->m_Transform.scale = { 3.0f, 3.0f };
        m_UndergroundBackground->SetZIndex(0); // 放在與地表相同的背景層
        m_UndergroundBackground->SetVisible(false);

        // 🌟 修正：設定初始絕對位置。先設在 15384，Y=0，
        m_UndergroundOriginalX = 15384.0f;
        m_UndergroundBackground->SetPosition({ m_UndergroundOriginalX, 0.0f });

        renderer.AddChild(m_UndergroundBackground);

        // ==========================================
        // 🌟 新增：載入地下室水管圖片 (water pipe under.png)
        // ==========================================
        // 依照你前面的報錯畫面，這張圖放在 Environment 資料夾
        m_UndergroundPipe = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Environment/stage1-1/water pipe under.png");
        m_UndergroundPipe->m_Transform.scale = { 3.0f, 3.0f };
        m_UndergroundPipe->SetZIndex(15); // Z=15，比瑪利歐的 10 還高，確保能遮住他
        m_UndergroundPipe->SetVisible(false);

        // 設定地下室水管的絕對 X 座標 (大約在你設的 15624 物理牆壁附近，可自行微調)
        m_UndergroundPipeOriginalX = 15696.0f;
        m_UndergroundPipe->SetPosition({ m_UndergroundPipeOriginalX, 0.0f });

        renderer.AddChild(m_UndergroundPipe);
    }

    // ➡️ 更新：根據 worldOffset 移動所有背景
    void Update(float worldOffset) {
        // 更新地表背景
        for (int i = 0; i < m_BackgroundStages.size(); ++i) {
            m_BackgroundStages[i]->SetPosition({ m_OriginalXCoords[i] - worldOffset, 0.0f });
        }

        // 🌟 修正：更新地下室背景！當鏡頭切換到 15400 時，它才會出現在畫面中。
        if (m_UndergroundBackground) {
            m_UndergroundBackground->SetPosition({ m_UndergroundOriginalX - worldOffset, 0.0f }); // 這裡也可以微調 Y 值，例如 50.0f 或 -50.0f
        }

        // 🌟 新增：更新地下室水管，讓它跟著鏡頭一起移動！
        if (m_UndergroundPipe) {
            m_UndergroundPipe->SetPosition({ m_UndergroundPipeOriginalX - worldOffset, 0.0f });
        }
    }

    void SetVisible(bool visible) {
        for (auto& stage : m_BackgroundStages) stage->SetVisible(visible);

        // 🌟 確保遊戲開始時，地下室的圖也被設為顯示狀態
        if (m_UndergroundBackground) {
            m_UndergroundBackground->SetVisible(visible);
        }
        if (m_UndergroundPipe) {
            m_UndergroundPipe->SetVisible(visible);
        }
    }

private:
    std::vector<std::shared_ptr<Character>> m_BackgroundStages;
    std::vector<float> m_OriginalXCoords; // 儲存初始位置以供捲動參考

    std::shared_ptr<Character> m_UndergroundBackground;
    float m_UndergroundOriginalX = 0.0f; // 紀錄地下室的初始絕對 X 座標

    // 🌟 新增：地下室水管專用的變數
    std::shared_ptr<Character> m_UndergroundPipe;
    float m_UndergroundPipeOriginalX = 0.0f;
};

#endif // MAP_HPP