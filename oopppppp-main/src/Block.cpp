#include "Block.hpp"

Block::Block(Type type, glm::vec2 startPos, int interval) {
    m_Type = type;
    m_State = State::NORMAL;
    m_OriginalPos = startPos;
    m_CurrentPos = startPos;
    m_VelocityY = 0.0f;
    m_BlinkInterval = interval;

    // 🌟 1. 設定方塊一開始有沒有道具 (問號磚跟道具磚預設有道具)
    if (type == Type::QUESTION || type == Type::BRICK_ITEM) {
        m_HasItem = true;
    }
    else {
        m_HasItem = false;
    }
    m_JustSpawnedItem = false; // 剛出生還沒舉手

    // --- 載入圖片 ---
    if (type == Type::QUESTION) {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/question_1.png", GA_RESOURCE_DIR"/Image/Environment/stage1-1/question_2.png", GA_RESOURCE_DIR"/Image/Environment/stage1-1/question_3.png" };
        m_EmptyImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/question_afterhit.png" };
    }
    else if (type == Type::BRICK_ITEM) {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/brick.png" };
        m_EmptyImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/brick_e.png" };
    }
    else if (type == Type::BRICK_FRAGILE) {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/brick.png" };
        m_EmptyImages = {};
    }
    else if (type == Type::PIPE_A) {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/water pipe a.png" };
        m_EmptyImages = {};
    }
    else if (type == Type::PIPE_B) {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/water pipe b.png" };
        m_EmptyImages = {};
    }
    else {
        m_NormalImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/brick.png" };
        m_EmptyImages = { GA_RESOURCE_DIR"/Image/Environment/stage1-1/brick_e.png" };
    }

    m_Visual = std::make_shared<AnimatedCharacter>(m_NormalImages);
    m_Visual->SetPosition(m_CurrentPos);

    // --- 設定圖層 ---
    if (type == Type::PIPE_A || type == Type::PIPE_B) {
        m_Visual->SetZIndex(15);
    }
    else {
        m_Visual->SetZIndex(5);
    }

    m_Visual->m_Transform.scale = { 3.0f, 3.0f };
    m_Visual->SetVisible(false);

    if (type == Type::QUESTION) {
        m_Visual->SetAnimation(m_NormalImages, m_BlinkInterval);
    }
}

void Block::Update(float deltaTime, float worldOffset) {
    if (m_State == State::DESTROYED) return;

    if (m_State == State::BOUNCING) {
        m_CurrentPos.y += m_VelocityY * deltaTime * 60.0f;
        m_VelocityY -= 1.5f * (deltaTime * 60.0f);

        if (m_CurrentPos.y <= m_OriginalPos.y) {
            m_CurrentPos.y = m_OriginalPos.y;
            m_VelocityY = 0.0f;

            // 🌟 核心：如果有道具，觸發舉手開關！
            if (m_HasItem) {
                m_HasItem = false;        // 口袋空了
                m_JustSpawnedItem = true; // 舉手🙋‍♂️！通知 AppUpdate 生成蘑菇
                // 外觀變成 EMPTY 的邏輯，你的 Update 裡已經完美寫好了！
            }

            if (m_Type == Type::QUESTION || m_Type == Type::BRICK_ITEM) {
                m_State = State::EMPTY;
                m_Visual->SetAnimation(m_EmptyImages);
                m_Visual->Pause();
            }
            else {
                m_State = State::NORMAL;
            }
        }
    }
    m_Visual->SetPosition({ m_OriginalPos.x - worldOffset, m_CurrentPos.y });
}

// 🌟 2. 接收 isBigMario 參數，實作敲磚與噴道具邏輯
void Block::Hit(bool isBigMario) {
    // 水管防呆：水管頂不動！
    if (m_Type == Type::PIPE_A || m_Type == Type::PIPE_B) return;

    // 如果已經空了、碎了、或是正在彈，就不反應
    if (m_State == State::EMPTY || m_State == State::DESTROYED || m_State == State::BOUNCING) {
        return;
    }

    // 🌟 大隻瑪利歐敲碎脆磚 (直接破壞消失)
    if (m_Type == Type::BRICK_FRAGILE && isBigMario) {
        m_State = State::DESTROYED;
        m_Visual->SetVisible(false);
        return;
    }

    // 觸發彈跳
    m_State = State::BOUNCING;
    m_VelocityY = 9.0f; // 保持你原本完美的彈跳力度

    
}

Rect Block::GetHitbox() const {
    // 關鍵修正：水管「不要」產生內建碰撞框
    if (m_State == State::DESTROYED || m_Type == Type::PIPE_A || m_Type == Type::PIPE_B) {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    return { m_OriginalPos.x - 24.0f, m_OriginalPos.y - 24.0f, 48.0f, 48.0f };
}