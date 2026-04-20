#include "TopUI.hpp"
#include "Util/Image.hpp"

TopUI::TopUI() {
    // 1. 載入靜態的 top.png (包含 MARIO, WORLD, TIME 字樣)
    m_TopText = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/top.png");
    m_TopText->m_Transform.scale = { 3.0f, 3.0f };
    m_TopText->SetZIndex(100); // 🌟 Z 軸設超大，確保永遠在最上層不會被雲朵擋住

    // ⚠️ 這裡的 Y 座標 (320.0f) 是螢幕最上方，X 座標 (0.0f) 是正中央。
    // 請依據你遊戲視窗的實際大小來微調這些座標！
    m_TopText->SetPosition({ 0.0f, 312.0f });

    // 2. 初始化分數 (6位)、金幣 (2位)、時間 (3位) 的圖片佔位符
    // ⚠️ 以下的 startX 和 startY 也需要你配合 top.png 的字體位置進行微調對齊！
    CreateDigits(m_ScoreDigits, 7, -324.0f, 276.0f); // 分數位置
    CreateDigits(m_CoinDigits, 2, -60.0f, 276.0f);  // 金幣位置
    CreateDigits(m_TimeDigits, 3, 252.0f, 276.0f);   // 時間位置
}

void TopUI::CreateDigits(std::vector<std::shared_ptr<Character>>& digits, int count, float startX, float startY) {
    for (int i = 0; i < count; ++i) {
        auto digit = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/top/0.png");
        digit->m_Transform.scale = { 3.0f, 3.0f };
        digit->SetZIndex(100);
        // 每個數字往右排開，間隔大約 24 像素 (可依你的圖片寬度調整)
        digit->SetPosition({ startX + (i * 24.0f), startY });
        digits.push_back(digit);
    }
}

void TopUI::Update(int score, int coins, int time) {
    UpdateDigits(score, m_ScoreDigits);
    UpdateDigits(coins, m_CoinDigits);
    UpdateDigits(time, m_TimeDigits);
}

void TopUI::UpdateDigits(int value, std::vector<std::shared_ptr<Character>>& digits) {
    // 將數字轉成字串，並在前面補 0 直到符合位數 (例如 200 -> "000200")
    std::string valStr = std::to_string(value);
    int padding = digits.size() - valStr.length();

    // 防呆機制：如果分數爆表超過位數，就鎖在最大值 (例如 999999)
    if (padding < 0) {
        valStr = std::string(digits.size(), '9');
        padding = 0;
    }

    std::string paddedStr = std::string(padding, '0') + valStr;

    // 替換每一位數對應的圖片
    for (size_t i = 0; i < digits.size(); ++i) {
        char digitChar = paddedStr[i];
        std::string imgPath = GA_RESOURCE_DIR"/Image/UI/top/" + std::string(1, digitChar) + ".png";

        // 🌟 換圖片 (引擎會把縮放偷偷重置回 1 倍)
        digits[i]->SetDrawable(std::make_shared<Util::Image>(imgPath));

        // 🌟 就是少這行！馬上強制把它放大回 3 倍！
        digits[i]->m_Transform.scale = { 3.0f, 3.0f };
    }
}

std::vector<std::shared_ptr<Character>> TopUI::GetDrawables() const {
    std::vector<std::shared_ptr<Character>> all;
    all.push_back(m_TopText);
    for (auto& d : m_ScoreDigits) all.push_back(d);
    for (auto& d : m_CoinDigits) all.push_back(d);
    for (auto& d : m_TimeDigits) all.push_back(d);
    return all;
}

void TopUI::SetVisible(bool visible) {
    m_TopText->SetVisible(visible);
    for (auto& d : m_ScoreDigits) d->SetVisible(visible);
    for (auto& d : m_CoinDigits) d->SetVisible(visible);
    for (auto& d : m_TimeDigits) d->SetVisible(visible);
}