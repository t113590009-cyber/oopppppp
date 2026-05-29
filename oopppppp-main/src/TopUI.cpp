#include "TopUI.hpp"
#include "Util/Logger.hpp"

TopUI::TopUI() {
    for (char c = '0'; c <= '9'; ++c) {
        std::string path = GA_RESOURCE_DIR"/Image/UI/top/" + std::string(1, c) + ".png";
        m_NumberImages[c] = std::make_shared<Util::Image>(path);
    }

    m_TopText = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/top.png");
    m_TopText->m_Transform.scale = { 3.0f, 3.0f };
    m_TopText->SetZIndex(100);
    m_TopText->SetPosition({ 0.0f, 312.0f });

    CreateDigits(m_ScoreDigits, 7, -324.0f, 276.0f);
    CreateDigits(m_CoinDigits, 2, -60.0f, 276.0f);
    CreateDigits(m_TimeDigits, 3, 252.0f, 276.0f);

    // 🌟 初始化 WORLD 底下的兩個數字
    for (int i = 0; i < 2; ++i) {
        auto digit = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/1.png");
        digit->m_Transform.scale = { 3.0f, 3.0f };
        digit->SetZIndex(100);
        m_WorldDigits.push_back(digit);
    }
    m_WorldDigits[0]->SetPosition({ 84.0f, 276.0f });  // 橫槓左邊 (大關卡 1)
    m_WorldDigits[1]->SetPosition({ 132.0f, 276.0f }); // 橫槓右邊 (小關卡)
}

void TopUI::CreateDigits(std::vector<std::shared_ptr<Character>>& digits, int count, float startX, float startY) {
    for (int i = 0; i < count; ++i) {
        auto digit = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/0.png");
        digit->m_Transform.scale = { 3.0f, 3.0f };
        digit->SetZIndex(100);
        digit->SetPosition({ startX + (i * 24.0f), startY });
        digits.push_back(digit);
    }
}

void TopUI::Update(int score, int coins, int time, int level) {
    if (score != m_LastScore) {
        UpdateDigits(score, m_ScoreDigits);
        m_LastScore = score;
    }
    if (coins != m_LastCoins) {
        UpdateDigits(coins, m_CoinDigits);
        m_LastCoins = coins;
    }
    if (time != m_LastTime) {
        UpdateDigits(time, m_TimeDigits);
        m_LastTime = time;
    }

    // 🌟 關卡數字更新
    if (level != m_LastLevel) {
        std::string lvlStr = std::to_string(level);
        char stageChar = lvlStr[0]; // 抓出關卡尾數

        if (m_NumberImages.count('1') && m_NumberImages['1']) {
            m_WorldDigits[0]->SetDrawable(m_NumberImages['1']); // 大關卡永遠是 1
            m_WorldDigits[0]->m_Transform.scale = { 3.0f, 3.0f };
        }
        if (m_NumberImages.count(stageChar) && m_NumberImages[stageChar]) {
            m_WorldDigits[1]->SetDrawable(m_NumberImages[stageChar]);
            m_WorldDigits[1]->m_Transform.scale = { 3.0f, 3.0f };
        }
        m_LastLevel = level;
    }
}

void TopUI::UpdateDigits(int value, std::vector<std::shared_ptr<Character>>& digits) {
    std::string valStr = std::to_string(value);
    int padding = static_cast<int>(digits.size()) - static_cast<int>(valStr.length());

    if (padding < 0) {
        valStr = std::string(digits.size(), '9');
        padding = 0;
    }

    std::string paddedStr = std::string(padding, '0') + valStr;

    for (size_t i = 0; i < digits.size(); ++i) {
        char digitChar = paddedStr[i];
        if (m_NumberImages.count(digitChar) && m_NumberImages[digitChar]) {
            digits[i]->SetDrawable(m_NumberImages[digitChar]);
            digits[i]->m_Transform.scale = { 3.0f, 3.0f };
        }
    }
}

std::vector<std::shared_ptr<Character>> TopUI::GetDrawables() const {
    std::vector<std::shared_ptr<Character>> all;
    all.push_back(m_TopText);
    for (auto& d : m_ScoreDigits) all.push_back(d);
    for (auto& d : m_CoinDigits) all.push_back(d);
    for (auto& d : m_TimeDigits) all.push_back(d);
    for (auto& d : m_WorldDigits) all.push_back(d); // 🌟 畫出關卡數字
    return all;
}

void TopUI::SetVisible(bool visible) {
    m_TopText->SetVisible(visible);
    for (auto& d : m_ScoreDigits) d->SetVisible(visible);
    for (auto& d : m_CoinDigits) d->SetVisible(visible);
    for (auto& d : m_TimeDigits) d->SetVisible(visible);
    for (auto& d : m_WorldDigits) d->SetVisible(visible); // 🌟 顯示/隱藏關卡數字
}