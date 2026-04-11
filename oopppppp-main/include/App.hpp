#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" 
#include "Util/Renderer.hpp"
#include "CollisionHandler.hpp" 
#include "Player.hpp"           
#include "Map.hpp"              
#include "Menu.hpp"             
#include "Block.hpp"            
#include "Goomba.hpp"           
// 🌟 補上這兩個道具系統的標頭檔
#include "Item.hpp"             
#include "Mushroom.hpp"         

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

    float m_WorldOffset = 0.0f;

    // ==========================================
    // 🍄 栗子球管理系統
    // ==========================================
    std::vector<std::unique_ptr<Goomba>> m_Goombas;
    int m_SpawnPhase = 0;

    // ==========================================
    // 🌟 道具管理系統 (補上這個百寶袋！)
    // ==========================================
    std::vector<std::shared_ptr<Item>> m_Items;     // 用來存放畫面上所有的道具
};

#endif // APP_HPP