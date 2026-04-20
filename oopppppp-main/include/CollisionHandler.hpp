#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

#include <vector>
#include <glm/vec2.hpp>

// 定義矩形區域
struct Rect {
    float x, y, width, height;
};

class CollisionHandler {
public:
    CollisionHandler() = default;

    // ➡️ 增加障礙物（水管、方塊、地面）
    void AddObstacle(float x, float y, float w, float h) {
        m_Obstacles.push_back({ x, y, w, h });
    }

    // ➡️ 取得所有障礙物（給 Update 迴圈用）
    const std::vector<Rect>& GetObstacles() const { return m_Obstacles; }

    // ➡️ 核心碰撞演算法 (AABB)
    static bool CheckCollision(Rect a, Rect b) {
        return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    }

    float GetGroundHeight(Rect marioFeet, float defaultGroundY) const {
        float groundY = defaultGroundY;
        for (const auto& obs : m_Obstacles) {
            if (CheckCollision(marioFeet, obs)) {
                // 如果踩到東西，回傳該物體的頂部高度
                return obs.y + obs.height;
            }
        }
        return groundY;
    }

    // ➡️ 清除所有障礙物（換關卡時用）
    void Clear() { m_Obstacles.clear(); }

private:
    std::vector<Rect> m_Obstacles;
};

#endif