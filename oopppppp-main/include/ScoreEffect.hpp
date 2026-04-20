#ifndef SCOREEFFECT_HPP
#define SCOREEFFECT_HPP

#include "Character.hpp" 
#include <memory>
#include <string>

class ScoreEffect {
public:
    // 傳入要顯示的分數 (例如 100, 200, 或者傳入 -1 代表 1UP)
    ScoreEffect(int scoreValue, float startWorldX, float startWorldY);

    // 更新飄浮與計時
    void Update(float deltaTime, float worldOffset);

    // 檢查是否顯示完畢
    bool IsDone() const { return m_IsDone; }

    // 給 App 加入 m_Root 畫出來用
    std::shared_ptr<Character> GetDrawable() const { return m_Visual; }

private:
    std::shared_ptr<Character> m_Visual;
    float m_WorldX;
    float m_WorldY;
    float m_Timer;       // 存活時間計時器
    float m_FloatSpeed;  // 往上飄的速度
    bool m_IsDone;       // 是否該被刪除？
};

#endif