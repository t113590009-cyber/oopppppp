#include "ScoreEffect.hpp"

ScoreEffect::ScoreEffect(int scoreValue, float startWorldX, float startWorldY) {
    m_WorldX = startWorldX;
    m_WorldY = startWorldY;
    m_Timer = 1.0f;       // 🌟 讓分數在畫面上停留 1 秒鐘
    m_FloatSpeed = 80.0f; // 🌟 每秒往上飄 80 像素
    m_IsDone = false;

    // 🌟 根據傳入的數字，決定要抓哪一張圖片
    std::string imagePath = GA_RESOURCE_DIR"/Image/Score/earn/";
    if (scoreValue == 100) imagePath += "100.png";
    else if (scoreValue == 200) imagePath += "200.png";
    else if (scoreValue == 400) imagePath += "400.png";
    else if (scoreValue == 500) imagePath += "500.png";
    else if (scoreValue == 800) imagePath += "800.png";
    else if (scoreValue == 1000) imagePath += "1000.png";
    else if (scoreValue == 2000) imagePath += "2000.png";
    else if (scoreValue == 4000) imagePath += "4000.png";
    else if (scoreValue == 5000) imagePath += "5000.png";
    else if (scoreValue == 8000) imagePath += "8000.png";
    else imagePath += "1up.png"; // 如果傳入 -1 或其他奇怪數字，就給 1UP

    m_Visual = std::make_shared<Character>(imagePath);
    m_Visual->m_Transform.scale = { 3.0f, 3.0f };
    m_Visual->SetZIndex(20); // 放在最前面，不要被磚塊擋住
    m_Visual->SetPosition({ m_WorldX, m_WorldY });
}

void ScoreEffect::Update(float deltaTime, float worldOffset) {
    if (m_IsDone) return;

    m_Timer -= deltaTime;
    if (m_Timer <= 0.0f) {
        m_IsDone = true; // 時間到，標記為可以刪除
        return;
    }

    // 🌟 讓分數緩緩往上飄
    m_WorldY += m_FloatSpeed * deltaTime;

    // 🌟 更新在畫面上的位置 (扣掉鏡頭偏移)
    m_Visual->SetPosition({ m_WorldX - worldOffset, m_WorldY });
}