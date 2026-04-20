#ifndef ANIMATED_CHARACTER_HPP
#define ANIMATED_CHARACTER_HPP

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class AnimatedCharacter : public Util::GameObject {
public:
    explicit AnimatedCharacter(const std::vector<std::string>& AnimationPaths) {
        // ➡️ 直接賦值，不宣告任何新變數！(給定 5 個參數來測試真正的建構子)
        this->m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, true, 50, true, 0);
    }

    void SetPosition(const glm::vec2& pos) { this->m_Transform.translation = pos; }
    glm::vec2 GetPosition() const { return this->m_Transform.translation; }

    // 🌟 升級版：接收 interval 參數控制動畫速度 (預設 100)
    void SetAnimation(const std::vector<std::string>& paths, int interval = 100) {
        auto anim = std::make_shared<Util::Animation>(paths, true, interval, true, 0);
        anim->Play();
        this->m_Drawable = anim;
    }

    void Play() {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        if (a) a->Play();
    }

    // 🌟 新增功能：讓動畫可以暫停 (變成死磚時使用)
    void Pause() {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        if (a) a->Pause();
    }

    [[nodiscard]] bool IsLooping() const {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        return a ? a->GetLooping() : false;
    }

    [[nodiscard]] bool IsPlaying() const {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        return a ? (a->GetState() == Util::Animation::State::PLAY) : false;
    }

    void SetLooping(bool looping) {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        if (a) a->SetLooping(looping);
    }

    [[nodiscard]] bool IfAnimationEnds() const {
        auto a = std::dynamic_pointer_cast<Util::Animation>(this->m_Drawable);
        return a ? (a->GetState() == Util::Animation::State::ENDED) : false;
    }
};

#endif // ANIMATED_CHARACTER_HPP