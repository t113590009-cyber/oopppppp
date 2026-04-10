#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "AnimatedCharacter.hpp" // 🌟 升級為動畫角色！
#include "CollisionHandler.hpp" 
#include <memory>
#include <vector> // 🌟 為了裝多張圖片，引入 vector
#include <string>

class Block {
public:
    // 🌟 1. 擴充方塊種類：新增 脆磚(FRAGILE) 與 道具磚(ITEM)
    enum class Type { QUESTION, BRICK, BRICK_FRAGILE, BRICK_ITEM, PIPE_A, PIPE_B};

    // 🌟 2. 擴充方塊狀態：新增 被破壞(DESTROYED)
    enum class State { NORMAL, BOUNCING, EMPTY, DESTROYED };

    Block(Type type, glm::vec2 startPos, int interval = 150);

    void Update(float deltaTime, float worldOffset);
    void Hit();
    Rect GetHitbox() const;

    // 🌟 回傳型態為 AnimatedCharacter
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Visual; }

    // 狀態判斷工具
    bool IsEmpty() const { return m_State == State::EMPTY; }

    // 🌟 3. 新增判斷：如果磚塊碎了，物理引擎就不該再撞到它
    bool IsDestroyed() const { return m_State == State::DESTROYED; }

private:
    Type m_Type;
    State m_State;

    glm::vec2 m_OriginalPos;
    glm::vec2 m_CurrentPos;
    float m_VelocityY;

    std::shared_ptr<AnimatedCharacter> m_Visual; // 🌟 動畫實體

    // 🌟 準備背包
    std::vector<std::string> m_NormalImages;
    std::vector<std::string> m_EmptyImages;

    // 速度變數
    int m_BlinkInterval;
};

#endif // BLOCK_HPP