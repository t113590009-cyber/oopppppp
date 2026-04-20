#ifndef ITEM_HPP
#define ITEM_HPP

#include "AnimatedCharacter.hpp"
#include "CollisionHandler.hpp"
#include "Player.hpp" // 需要知道 Player 才能呼叫他的變大函式

class Item : public AnimatedCharacter {
public:
    // 初始化時要給它一組圖片
    Item(const std::vector<std::string>& animPaths) : AnimatedCharacter(animPaths) {}

    virtual ~Item() = default; // 虛擬解構子，確保兒子們被正確刪除

    // 🌟 所有道具共同的更新與物理邏輯
    virtual void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) = 0;

    // 🌟 核心：道具被吃到時會發生什麼事？ (由兒子各自實作)
    virtual void ApplyEffect(Player* player) = 0;

    // 取得碰撞箱
    virtual Rect GetRect(float worldOffset) const = 0;

    // 道具狀態
    bool IsDestroyed() const { return m_IsDestroyed; }
    void Destroy() { m_IsDestroyed = true; }

protected:
    bool m_IsDestroyed = false; // 記錄道具是否已經被吃掉
    float m_WorldX = 0.0f;
    float m_WorldY = 0.0f;
};

#endif // ITEM_HPP