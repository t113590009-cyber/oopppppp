#pragma once

#include "AnimatedCharacter.hpp"
#include <memory>
#include <vector>
#include <string>

#ifndef NES_SCALE
#define NES_SCALE 3.0f
#endif

struct HammerRect {
    float x;
    float y;
    float width;
    float height;
};

class HammerProp {
public:
    // 傳入世界座標位置
    HammerProp(float worldX, float worldY);

    // 🌟 保持原樣不變，不增加新參數！
    void Update(float deltaTime);

    // 🌟 新增：讓外部把 m_WorldOffset 傳給鐵鎚，用來更正它的渲染位置
    void RenderWithCamera(float worldOffset);

    HammerRect GetHitbox() const;

    void Touch();
    bool IsDestroyed() const { return m_IsDestroyed; }
    std::shared_ptr<AnimatedCharacter> GetCharacter() const { return m_Animation; }

private:
    std::shared_ptr<AnimatedCharacter> m_Animation;
    float m_WorldX;
    float m_WorldY;
    bool m_IsDestroyed = false;

    std::vector<std::string> m_ImagePaths;
    float m_AnimTimer = 0.0f;
    int m_FrameIndex = 0;
    float m_FrameDuration = 0.15f;
};