#ifndef MOVINGBLOCK_HPP
#define MOVINGBLOCK_HPP

#include "Block.hpp"
#include <string>

class MovingBlock : public Block {
public:
    // 定義移動的軸向：水平 或 垂直
    enum class Axis { HORIZONTAL, VERTICAL };

    // 建構子：傳入圖片路徑、起始 X、起始 Y、移動軸向、移動範圍、移動速度
    MovingBlock(const std::string& imagePath, float startX, float startY, Axis axis, float range, float speed);

    // ==========================================
    // 🌟 關鍵修復：宣告覆寫 (override) 這三個函式！
    // ==========================================
    void Update(float dt, float worldOffset) override;
    void Hit(bool isBigMario = false) override;
    Rect GetHitbox() const override;

    // 讓 Player.cpp 能夠取得平台速度的接口 (履帶效應的核心)
    float GetVelocityX() const { return m_VelocityX; }
    float GetVelocityY() const { return m_VelocityY; }

private:
    Axis m_Axis;           // 記錄是水平還是垂直移動
    float m_Range;         // 移動範圍 (例如 150 像素)
    float m_Speed;         // 移動速度
    float m_StartX;        // 原始出發點 X
    float m_StartY;        // 原始出發點 Y
    float m_VelocityX;     // 當前 X 軸速度
    float m_VelocityY;     // 當前 Y 軸速度
    int m_Direction;       // 當前移動方向 (1 或 -1)
};

#endif // MOVINGBLOCK_HPP