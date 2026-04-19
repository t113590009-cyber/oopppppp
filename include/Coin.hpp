#ifndef COIN_HPP
#define COIN_HPP

#include "Item.hpp"
#include <vector>
#include <string>

class Coin : public Item {
public:
    // type: 0 = 從方塊彈出 (BOX), 1 = 在地圖上懸浮 (OVERWORLD)
    Coin(float startWorldX, float startWorldY, int type = 0);

    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) override;
    void ApplyEffect(Player* player) override;
    Rect GetRect(float worldOffset) const override;

private:
    int m_Type;
    float m_SpawnStartY;
    float m_VelocityY = 0.0f;

    // 動畫控制
    float m_AnimTimer = 0.0f;
    int m_AnimFrame = 0;
    std::vector<std::string> m_Images;
};

#endif