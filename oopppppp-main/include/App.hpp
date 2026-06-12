#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Renderer.hpp"
#include "CollisionHandler.hpp" 
#include "Player.hpp"           
#include "Map.hpp"              
#include "Menu.hpp"             
#include "TopUI.hpp"            
#include "Block.hpp"            
#include "Goomba.hpp"           
#include "Koopatroopa.hpp"
#include "Fireball.hpp"
#include "Item.hpp"
#include "Mushroom.hpp"
#include "ScoreEffect.hpp"
#include "AnimatedCharacter.hpp"
#include "Bowser.hpp"
#include "BowserFire.hpp"

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

    // 🌟 極度重要：就是漏了這行！必須宣告這個函式，CPP 檔才找得到它！
    void SetGameElementsVisible(bool visible);

    State m_CurrentState = State::START;
    Util::Renderer m_Root;

    int m_Score = 0;
    int m_Coins = 0;
    int m_ComboCount = 0;
    int m_Lives = 3;
    float m_GameTime = 400.0f;
    float m_DeathTimer = 0.0f;

    // 🌟 生命值黑畫面過場系統
    bool m_ShowLifeScreen = false;
    float m_LifeScreenTimer = 0.0f;
    std::shared_ptr<Character> m_LifeBg;
    std::shared_ptr<Character> m_LifeWorldNum;
    std::shared_ptr<Character> m_LifeStageNum;
    std::shared_ptr<Character> m_LifeCountNum;

    int m_PendingFireworks = 0;
    float m_FireworkTimer = 0.0f;
    std::vector<std::shared_ptr<AnimatedCharacter>> m_ActiveFireworks;
    std::vector<float> m_FireworkWorldX;
    float m_ClearGroundY = -264.0f;

    void ResetLevel(bool keepPosition = false); // 🌟 新增預設參數，預設為 false (回到原點)
    void LoadLevelObjects();
    std::shared_ptr<TopUI> m_TopUI;

    std::unique_ptr<Menu> m_Menu;
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;
    std::shared_ptr<Character> m_Castle;

    std::shared_ptr<Character> m_Flagpole;
    std::shared_ptr<Character> m_Flag;

    std::vector<std::shared_ptr<Block>> m_Blocks;
    CollisionHandler m_Collision;
    float m_WorldOffset = 0.0f;

    std::vector<std::unique_ptr<Goomba>> m_Goombas;
    int m_SpawnPhase = 0;
    std::vector<std::shared_ptr<Koopatroopa>> m_Koopatroopas;
    std::vector<std::shared_ptr<Bowser>> m_Bowsers;
    std::vector<std::shared_ptr<BowserFire>> m_BowserFires;

    std::vector<std::shared_ptr<Item>> m_Items;
    std::vector<std::shared_ptr<Fireball>> m_Fireballs;
    std::vector<std::shared_ptr<ScoreEffect>> m_ScoreEffects;

    std::shared_ptr<Character> m_FailScreen;

    bool m_IsLevelClear = false;
    bool m_IsFlagSliding = false;
    float m_LevelClearTimer = 0.0f;

    int m_CurrentLevel = 1;
    bool m_IsExitingCastle = false;

    bool m_CheatEnabled = false;
    void LoadLevel2Objects();
    void LoadLevel3Objects();
    void SwitchLevel(int nextLevel);

    void AddBlock(Block::Type type, int gridX, float gridY, Block::ItemType item = Block::ItemType::NONE);

    // 同學拆分出去的函式
    void UpdateBlocksAndItems(float dt, const std::vector<Rect>& allObstacles);
    void UpdateEnemiesAndFireballs(float dt, const std::vector<Rect>& allObstacles, const glm::vec2& pPos);
    void HandleLevel1ClearAnimation(float dt, float marioWorldX);
};

#endif // APP_HPP