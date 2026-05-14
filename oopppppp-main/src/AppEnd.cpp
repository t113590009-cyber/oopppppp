#include "App.hpp"
#include "Util/Logger.hpp"

void App::End() {
    LOG_TRACE("End");

    // 1. 隱藏玩家與地圖
    if (m_Player && m_Player->GetCharacter()) m_Player->GetCharacter()->SetVisible(false);
    if (m_Map) m_Map->SetVisible(false);

    // 2. 顯示 GameOver 圖片
    if (m_FailScreen) {
        m_FailScreen->SetVisible(true);
        m_FailScreen->SetPosition({0.0f, 0.0f}); // 確保在畫面中間
    }

    // 3. 務必呼叫一次 Update 讓渲染器知道要畫出這張圖
    m_Root.Update();
}