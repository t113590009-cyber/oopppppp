#ifndef TOPUI_HPP
#define TOPUI_HPP

#include "Character.hpp"
#include <vector>
#include <memory>
#include <string>

class TopUI {
public:
    TopUI();

    // 每次 Update 時把最新的數字傳進來
    void Update(int score, int coins, int time);

    // 🌟 新增：控制整個 UI 顯示或隱藏的開關
    void SetVisible(bool visible);

    // 給 App 把所有 UI 圖片拿去畫的函式
    std::vector<std::shared_ptr<Character>> GetDrawables() const;

private:
    std::shared_ptr<Character> m_TopText;                  // MARIO WORLD TIME 的底圖
    std::vector<std::shared_ptr<Character>> m_ScoreDigits; // 分數的 6 個數字
    std::vector<std::shared_ptr<Character>> m_CoinDigits;  // 金幣的 2 個數字
    std::vector<std::shared_ptr<Character>> m_TimeDigits;  // 時間的 3 個數字

    // 輔助函式：建立特定數量的數字圖片
    void CreateDigits(std::vector<std::shared_ptr<Character>>& digits, int count, float startX, float startY);
    // 輔助函式：更新數字圖片
    void UpdateDigits(int value, std::vector<std::shared_ptr<Character>>& digits);
};

#endif