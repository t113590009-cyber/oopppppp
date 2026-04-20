#ifndef MUSHROOM_HPP
#define MUSHROOM_HPP

#include "Item.hpp"

class Mushroom : public Item {
public:
    enum class State { SPAWNING, MOVING }; // 🌟 兩種狀態：鑽出方塊中 / 移動中

    Mushroom(float spawnWorldX, float spawnWorldY);

    void Update(float deltaTime, float worldOffset, const std::vector<Rect>& obstacles) override;
    void ApplyEffect(Player* player) override;
    Rect GetRect(float worldOffset) const override;

private:
    State m_State = State::SPAWNING; // 🌟 剛出生的時候預設是「鑽出中」
    float m_SpawnStartY = 0.0f;      // 記錄方塊的高度，才知道要鑽多高

    float m_SpeedX = 2.5f;
    float m_VelocityY = 0.0f;
    const float GRAVITY = -15.0f;
};

#endif // MUSHROOM_HPP