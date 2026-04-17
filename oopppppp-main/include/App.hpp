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

// 🌟 補上道具系統的標頭檔
#include "Item.hpp"
#include "Mushroom.hpp"
#include "ScoreEffect.hpp"

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

private:
    // --- 遊戲狀態管理 ---
    State m_CurrentState = State::START;
    Util::Renderer m_Root;

    // ==========================================
    // 🌟 遊戲數值與 UI 管理
    // ==========================================
    int m_Score = 0;           // 總分
    int m_Coins = 0;           // 金幣數
    int m_ComboCount = 0;      // 🌟 踩踏連擊計數器
    int m_Lives = 3;           // 🌟 瑪利歐的生命數
    float m_GameTime = 400.0f; // 瑪利歐經典的 400 秒倒數
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

    // ==========================================
    // 🍄 栗子球管理系統
    // ==========================================
    std::vector<std::unique_ptr<Goomba>> m_Goombas;
    int m_SpawnPhase = 0;

    // ==========================================
    // 🌟 道具管理系統
    // ==========================================
    std::vector<std::shared_ptr<Item>> m_Items; // 存放畫面上所有的道具 (蘑菇/星星/金幣)

    // 分數特效系統
    std::vector<std::shared_ptr<ScoreEffect>> m_ScoreEffects;

    // ==========================================
    // 💀 失敗畫面 UI 與延遲計時器
    // ==========================================
    std::shared_ptr<Character> m_FailScreen;
    float m_DeathTimer = 0.0f; // <--- 死亡延遲判定計時器
};

#endif // APP_HPP