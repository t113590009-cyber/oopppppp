#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp" 

class Character : public Util::GameObject {
public:
    // ➡️ 1. 實作建構子：將圖片路徑存起來，並設定給 m_Drawable
    explicit Character(const std::string& ImagePath) {
        m_ImagePath = ImagePath;
        this->m_Drawable = std::make_shared<Util::Image>(ImagePath);

        // 👇 就是這兩行必殺技！強制解鎖底層的隱形限制與厚度！
        this->m_Visible = true;
        this->m_ZIndex = 100;

        ResetPosition();
    }

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    // ➡️ 2. 實作 SetImage：更新圖片路徑，並重新設定 m_Drawable
    void SetImage(const std::string& ImagePath) {
        m_ImagePath = ImagePath;
        this->m_Drawable = std::make_shared<Util::Image>(ImagePath);
    }

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    // TODO: Implement the collision detection
    [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const {
        (void)other;
        return false;
    }

private:
    void ResetPosition() { m_Transform.translation = { 0, 0 }; }

    std::string m_ImagePath;
};

#endif // CHARACTER_HPP