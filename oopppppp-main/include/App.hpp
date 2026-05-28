#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "CollisionHandler.hpp" // 裡面已經有 struct Rect 了
#include "Player.hpp"           // 新建立的角色類別
#include "Map.hpp"              // 新建立的地圖類別
#include "Menu.hpp"             // 新建立的選單類別
#include "TopUI.hpp"            // 🌟 頂部計分板 UI
#include "Block.hpp"            // 🛡️ 磚塊系統
#include "Goomba.hpp"           // ➕ 栗子球系統
#include "Koopatroopa.hpp"
#include "Fireball.hpp"

// 🌟 補上道具系統的標頭檔
#include "Item.hpp"
#include "Mushroom.hpp"
#include "ScoreEffect.hpp"

// 🌟 極重要：必須引入這個，煙火動畫才能正常讀取！
#include "AnimatedCharacter.hpp" 

#include <memory>
#include <vector>

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    void Start();
    void Update();
    void End();

private:
    void ValidTask();

    // --- 遊戲狀態管理 ---
    State m_CurrentState = State::START;
    Util::Renderer m_Root;

    // 🌟 遊戲數值與 UI 管理
    int m_Score = 0;           // 總分
    int m_Coins = 0;           // 金幣數
    int m_ComboCount = 0;      // 🌟 踩踏連擊計數器
    int m_Lives = 3;           // 🌟 瑪利歐的生命數
    float m_GameTime = 400.0f; // 瑪利歐經典的 400 秒倒數
    float m_DeathTimer = 0.0f; // 💀 控制死亡動畫播多久的計時器

    // ==========================================
    // 🎆 我們新增的煙火與過關動畫專用變數 (合併加回)
    // ==========================================
    int m_PendingFireworks = 0;
    float m_FireworkTimer = 0.0f;
    std::vector<std::shared_ptr<AnimatedCharacter>> m_ActiveFireworks;
    std::vector<float> m_FireworkWorldX; // 記錄煙火絕對世界座標
    float m_ClearGroundY = -264.0f;      // 記錄過關落地高度，防止浮空遁地
    // ==========================================

    void ResetLevel();         // 🔄 重置關卡用的函式
    void LoadLevelObjects();   // 重新生成所有磚塊跟怪物
    std::shared_ptr<TopUI> m_TopUI;

    // --- ➡️ 重構後的管理者對象 ---
    std::unique_ptr<Menu> m_Menu;
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;
    std::shared_ptr<Character> m_Castle;

    std::shared_ptr<Character> m_Flagpole; // 🚩 新增：旗桿掛架 (單純圖片)
    std::shared_ptr<Character> m_Flag;     // 🏳️ 新增：旗子掛架 (單純圖片)

    // 🛡️ 磚塊陣列
    std::vector<std::shared_ptr<Block>> m_Blocks;

    // 碰撞系統
    CollisionHandler m_Collision;

    // 全域捲動位移
    float m_WorldOffset = 0.0f;

    // 🍄 栗子球管理系統
    std::vector<std::unique_ptr<Goomba>> m_Goombas;
    int m_SpawnPhase = 0;

    // 🐢 烏龜陣列
    std::vector<std::shared_ptr<Koopatroopa>> m_Koopatroopas;

    // 🌟 道具與特效管理系統
    std::vector<std::shared_ptr<Item>> m_Items; // 存放畫面上所有的道具 (蘑菇/星星/金幣)
    std::vector<std::shared_ptr<Fireball>> m_Fireballs;
    std::vector<std::shared_ptr<ScoreEffect>> m_ScoreEffects; // 分數特效系統

    // 💀 失敗畫面 UI 與延遲計時器
    std::shared_ptr<Character> m_FailScreen;

    // 🚩 過關動畫控制系統
    bool m_IsLevelClear = false;      // 是否已經碰到旗桿
    bool m_IsFlagSliding = false;     // 是否正在往下滑
    bool m_IsWalkingToCastle = false; // 是否正在走向城堡
    float m_LevelClearTimer = 0.0f;   // 用來控制落地後的停頓或轉圈時間

    // ==========================================
    // 🗺️ 同學新增的關卡與地圖邏輯
    // ==========================================
    int m_CurrentLevel = 1;         // 🌟 記錄目前是第幾關
    bool m_IsExitingCastle = false; // 🌟 記錄第二關開場時，是否正在從城堡走出來

    void LoadLevel2Objects();       // 🍄 第二關 (1-2) 專用的物件鋪設
    void LoadLevel3Objects();       // ✨ 新增：第三關 (1-3) 專用的物件鋪設

    void AddBlock(Block::Type type, int gridX, float gridY, Block::ItemType item = Block::ItemType::NONE);
};

#endif // APP_HPP