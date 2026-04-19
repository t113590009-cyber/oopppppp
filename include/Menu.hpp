#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include "Character.hpp"
#include <memory>
#include <cmath>

class Menu {
public:
    Menu();

    // ➡️ 更新閃爍邏輯與偵測 Enter 鍵
    void Update(float deltaTime);

    // ➡️ 讓 App 知道是否該進入遊戲
    bool IsStartPressed() const;

    // ➡️ 設定顯示或隱藏
    void SetVisible(bool visible);
    bool GetVisibility() const { return m_Visible; }

    // 👇 這裡就是我們補上的兩個關鍵函式！讓 App 拿得到圖片！
    std::shared_ptr<Character> GetTitle() const { return m_TitleBackground; }
    std::shared_ptr<Character> GetSelector() const { return m_Selector; }

private:
    std::shared_ptr<Character> m_TitleBackground;
    std::shared_ptr<Character> m_Selector;
    float m_MenuTimer = 0.0f;
    bool m_Visible = true;
    bool m_StartPressed = false;
};

#endif // MENU_HPP