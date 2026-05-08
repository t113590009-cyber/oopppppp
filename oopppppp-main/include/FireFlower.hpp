#ifndef FIREFLOWER_HPP
#define FIREFLOWER_HPP

#include "Item.hpp"
#include <vector>
#include <string>

class FireFlower : public Item {
public:
    FireFlower(float worldX, float worldY);

    void Update(float dt, float worldOffset, const std::vector<Rect>& obstacles) override;
    void ApplyEffect(Player* player) override;
    Rect GetRect(float worldOffset) const override;

private:
    float m_WorldX; // 🌟 新增：記住它在真實世界的位置
    float m_WorldY;
    float m_SpawnTimer = 0.0f;
    float m_TargetY;
    bool m_IsSpawning = true;
};

#endif // FIREFLOWER_HPP