#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "AnimatedCharacter.hpp" 
#include "CollisionHandler.hpp" 
#include <memory>
#include <vector> 
#include <string>

class Block {
public:
    enum class Type { QUESTION, BRICK, BRICK_FRAGILE, BRICK_ITEM, PIPE_A, PIPE_B };
    enum class State { NORMAL, BOUNCING, EMPTY, DESTROYED };

    // 🌟 1. 新增：定義方塊肚子裡可以裝的道具種類
    enum class ItemType {
        NONE,       // 空的 (只會彈一下或碎掉)
        MUSHROOM,   // 裝蘑菇
        STAR        // 裝無敵星星
        // COIN     // 未來如果要擴充可以加在這裡
    };

    Block(Type type, glm::vec2 startPos, int interval = 150);

    void Update(float deltaTime, float worldOffset);

    // 接收 bool，讓大隻瑪利歐可以碎磚
    void Hit(bool isBigMario = false);

    Rect GetHitbox() const;
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Visual; }

    bool IsEmpty() const { return m_State == State::EMPTY; }
    bool IsDestroyed() const { return m_State == State::DESTROYED; }

    // 取得方塊座標 (給蘑菇/星星出生用)
    glm::vec2 GetPosition() const { return m_CurrentPos; }

    // ==========================================
    // 🎁 2. 新增：道具系統介面
    // ==========================================

    // 讀取/設定內容物
    ItemType GetItemType() const { return m_ItemType; }
    void SetItemType(ItemType type) {
        m_ItemType = type;
        m_HasItem = (type != ItemType::NONE); // 設定了非 NONE 的道具，就代表口袋裡有東西
    }

    // 舉手開關：告訴 App 這個方塊「剛剛」噴出道具了
    bool HasJustSpawnedItem() {
        if (m_JustSpawnedItem) {
            m_JustSpawnedItem = false; // 讀取完就把手放下
            return true;
        }
        return false;
    }

private:
    Type m_Type;
    State m_State;

    glm::vec2 m_OriginalPos;
    glm::vec2 m_CurrentPos;
    float m_VelocityY;

    std::shared_ptr<AnimatedCharacter> m_Visual;

    std::vector<std::string> m_NormalImages;
    std::vector<std::string> m_EmptyImages;

    int m_BlinkInterval;

    // --- 道具系統變數 ---
    bool m_HasItem = false;         // 口袋裡有沒有藏東西？
    bool m_JustSpawnedItem = false; // 剛剛是不是噴出東西了？

    // 🌟 3. 新增：紀錄肚子裡裝什麼，預設為 NONE
    ItemType m_ItemType = ItemType::NONE;
};

#endif // BLOCK_HPP