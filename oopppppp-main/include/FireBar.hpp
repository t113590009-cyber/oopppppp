#ifndef FIREBAR_HPP
#define FIREBAR_HPP

#include "AnimatedCharacter.hpp"
#include "Player.hpp"
#include "CollisionHandler.hpp"
#include <vector>
#include <memory>

class FireBar {
public:
    // spawnWorldX, spawnWorldY: 旋轉的中心點 (通常對齊某個方塊的中心)
    // numFireballs: 火柱由幾顆火球組成 (經典長度通常是 6 顆)
    // speed: 旋轉速度 (負數=順時針轉，正數=逆時針轉)
    // initialAngle: 一開始的角度 (0.0f 是平平的朝右)
    FireBar(float spawnWorldX, float spawnWorldY, int numFireballs = 6, float speed = -2.5f, float initialAngle = 0.0f);

    void Update(float deltaTime, float worldOffset);
    void Interact(Player* player, float worldOffset);

    // 給 App 將每一顆火球加進 m_Root 畫出來
    std::vector<std::shared_ptr<AnimatedCharacter>> GetDrawables() const { return m_Fireballs; }

private:
    float m_WorldX;
    float m_WorldY;
    int m_NumFireballs;
    float m_Angle;
    float m_Speed;
    float m_Spacing = 24.0f; // 🌟 兩顆火球之間的間距 (可依據你的火球大小微調)

    std::vector<std::shared_ptr<AnimatedCharacter>> m_Fireballs;
};

#endif // FIREBAR_HPP