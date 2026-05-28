#ifndef TOPUI_HPP
#define TOPUI_HPP

#include "Character.hpp"
#include "Util/Image.hpp" 
#include <vector>
#include <memory>
#include <string>
#include <map>            

class TopUI {
public:
    TopUI();

    // 🌟 加上 level 參數，讓 UI 知道現在是第幾關
    void Update(int score, int coins, int time, int level = 1);

    void SetVisible(bool visible);
    std::vector<std::shared_ptr<Character>> GetDrawables() const;

private:
    std::shared_ptr<Character> m_TopText;
    std::vector<std::shared_ptr<Character>> m_ScoreDigits;
    std::vector<std::shared_ptr<Character>> m_CoinDigits;
    std::vector<std::shared_ptr<Character>> m_TimeDigits;
    std::vector<std::shared_ptr<Character>> m_WorldDigits; // 🌟 裝 WORLD 1 - 1 數字的容器

    void CreateDigits(std::vector<std::shared_ptr<Character>>& digits, int count, float startX, float startY);
    void UpdateDigits(int value, std::vector<std::shared_ptr<Character>>& digits);

    int m_LastScore = -1;
    int m_LastCoins = -1;
    int m_LastTime = -1;
    int m_LastLevel = -1; // 🌟 紀錄上次的關卡，沒換關就不重繪

    std::map<char, std::shared_ptr<Util::Image>> m_NumberImages;
};

#endif // TOPUI_HPP