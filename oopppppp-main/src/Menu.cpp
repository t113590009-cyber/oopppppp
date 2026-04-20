#include "Menu.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Menu::Menu() {
    m_TitleBackground = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/mario_title.png");
    m_Selector = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/selector_icon.png");

    // 1. 強制放大
    m_TitleBackground->m_Transform.scale = { 3.0f, 3.0f };
    m_Selector->m_Transform.scale = { 3.0f, 3.0f };

    // 2. 圖層設定 (🌟 游標設為 100，保證絕對不會被標題擋住！)
    m_TitleBackground->SetZIndex(10);
    m_Selector->SetZIndex(20);

    // 3. 設定座標
    m_TitleBackground->SetPosition({ 0.0f, 0.0f });
    m_Selector->SetPosition({ 0.0f, 0.0f });

    // 4. 強制顯示
    m_TitleBackground->SetVisible(true);
    m_Selector->SetVisible(true);
}

void Menu::Update(float deltaTime) {
    if (!m_Visible) return;

    // 🌟 絕對不會被引擎騙的「自己記狀態」閃爍法
    static bool isShow = true;
    m_MenuTimer += deltaTime;
    if (m_MenuTimer >= 0.5f) {
        isShow = !isShow; // 自己把開關反轉 (亮變暗、暗變亮)
        m_Selector->SetVisible(isShow); // 強制設定給方塊
        m_MenuTimer = 0.0f; // 計時器歸零
    }

    // 偵測 Enter 鍵
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_StartPressed = true;
    }
}

bool Menu::IsStartPressed() const { return m_StartPressed; }

void Menu::SetVisible(bool visible) {
    m_Visible = visible;
    m_TitleBackground->SetVisible(visible);
    // 🌟 如果選單關閉了，確保游標也一起隱藏
    if (!visible) {
        m_Selector->SetVisible(false);
    }
}