#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "CollisionHandler.hpp" // 裡面已經有 struct Rect 了
#include "Player.hpp"           // 新建立的角色類別
#include "Map.hpp"              // 新建立的地圖類別
#include "Menu.hpp"             // 新建立的選單類別
#include "Block.hpp"            // 🛡️ 保衛戰：保留你的磚塊系統
#include "Goomba.hpp"           // ➕ 朋友新增：栗子球標頭檔

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

    // --- ➡️ 重構後的管理者對象 ---
    std::unique_ptr<Menu> m_Menu;
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;
    std::shared_ptr<Character> m_Castle;

    // 🛡️ 保衛戰：保留你的磚塊陣列
    std::vector<std::shared_ptr<Block>> m_Blocks;

    // 碰撞系統
    CollisionHandler m_Collision;

    // 如果你有全域的捲動位移，可以考慮留在這或移入 Map
    float m_WorldOffset = 0.0f;

    // ==========================================
    // 🍄 朋友新增：栗子球管理系統
    // ==========================================
    std::vector<std::unique_ptr<Goomba>> m_Goombas;
    int m_SpawnPhase = 0;

    // 🌟 失敗畫面 UI 與延遲計時器
    std::shared_ptr<Character> m_FailScreen;
    float m_DeathTimer = 0.0f; // <--- 新增這個計時器
};

#endif // APP_HPP