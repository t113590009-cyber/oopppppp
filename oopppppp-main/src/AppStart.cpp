#include "App.hpp"
#include "Util/Logger.hpp"
#include "Coin.hpp"
#include "MovingBlock.hpp"
#include "FireBar.hpp" // 🌟 記得引入火柱

void App::Start() {
    LOG_TRACE("Start");

    m_Menu = std::make_unique<Menu>();
    m_Player = std::make_unique<Player>();
    m_Map = std::make_unique<Map>();
    m_Map->Init(m_Root);

    m_Flagpole = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/elv_win.png");
    m_Flagpole->m_Transform.scale = { 3.0f, 3.0f };
    m_Flagpole->SetZIndex(1);
    m_Flagpole->SetPosition({ 9147.0f, 6.0f });
    m_Flagpole->SetVisible(false);
    m_Root.AddChild(m_Flagpole);

    m_Flag = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/flag_win.png");
    m_Flag->m_Transform.scale = { 3.0f, 3.0f };
    m_Flag->SetZIndex(2);
    m_Flag->SetPosition({ 9120.0f, 180.0f });
    m_Flag->SetVisible(false);
    m_Root.AddChild(m_Flag);

    m_Castle = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Background/stage1-1/castle1_1_win.png");
    m_Castle->m_Transform.scale = { 3.0f, 3.0f };
    m_Castle->SetZIndex(2);
    m_Castle->SetPosition({ 9436.0f, -145.0f });
    m_Castle->SetVisible(false);
    m_Root.AddChild(m_Castle);

    m_FlagpoleLvl2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/elv_win.png");
    m_FlagpoleLvl2->m_Transform.scale = { 3.0f, 3.0f };
    m_FlagpoleLvl2->SetZIndex(1);
    m_FlagpoleLvl2->SetVisible(false);
    m_Root.AddChild(m_FlagpoleLvl2);

    m_FlagLvl2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/flag_win.png");
    m_FlagLvl2->m_Transform.scale = { 3.0f, 3.0f };
    m_FlagLvl2->SetZIndex(2);
    m_FlagLvl2->SetVisible(false);
    m_Root.AddChild(m_FlagLvl2);

    m_CastleLvl2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Background/stage1-2/castle1-2win.png");
    m_CastleLvl2->m_Transform.scale = { 3.0f, 3.0f };
    m_CastleLvl2->SetZIndex(2);
    m_CastleLvl2->SetVisible(false);
    m_Root.AddChild(m_CastleLvl2);

    auto title = m_Menu->GetTitle();
    auto selector = m_Menu->GetSelector();
    title->m_Transform.scale = { 3.0f, 3.0f };
    selector->m_Transform.scale = { 3.0f, 3.0f };
    title->SetZIndex(0);
    selector->SetZIndex(1);
    title->SetVisible(true);
    selector->SetVisible(true);
    m_Root.AddChild(title);
    m_Root.AddChild(selector);

    auto leftBar = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/black_box.png");
    leftBar->m_Transform.scale = { 3.0f, 3.0f };
    leftBar->SetZIndex(20);
    leftBar->SetPosition({ -550.0f, 0.0f });
    m_Root.AddChild(leftBar);

    auto rightBar = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/black_box.png");
    rightBar->m_Transform.scale = { 3.0f, 3.0f };
    rightBar->SetZIndex(20);
    rightBar->SetPosition({ 550.0f, 0.0f });
    m_Root.AddChild(rightBar);

    m_FailScreen = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/fail.png");
    m_FailScreen->m_Transform.scale = { 3.0f, 3.0f };
    m_FailScreen->SetZIndex(100);
    m_FailScreen->SetPosition({ 0.0f, 0.0f });
    m_FailScreen->SetVisible(false);
    m_Root.AddChild(m_FailScreen);

    LoadLevelObjects();

    m_Root.AddChild(m_Player->GetCharacter());

    m_TopUI = std::make_shared<TopUI>();
    for (auto& uiElem : m_TopUI->GetDrawables()) {
        m_Root.AddChild(uiElem);
    }
    m_TopUI->SetVisible(false);

    m_CurrentState = State::UPDATE;
}

void App::LoadLevelObjects() {
    auto AddFixedCoin = [&](float absoluteX, float absoluteY) {
        auto coin = std::make_shared<Coin>(absoluteX, absoluteY, 1);
        m_Root.AddChild(coin);
        m_Items.push_back(coin);
        };

    float ugStartX = 15192.0f + 24.0f;
    float ugBaseY = -96.0f;

    for (int i = 0; i < 7; ++i) AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY);
    for (int i = 0; i < 7; ++i) AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY + 96.0f);
    for (int i = 1; i <= 5; ++i) AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY + 192.0f);

    const float ROW_1_Y = -72.0f;
    const float ROW_2_Y = 120.0f;

    m_Collision.AddObstacle(-380.0f, -360.0f, 3312.0f, 96.0f);
    m_Collision.AddObstacle(3028.0f, -360.0f, 720.0f, 96.0f);
    m_Collision.AddObstacle(3892.0f, -360.0f, 3072.0f, 96.0f);
    m_Collision.AddObstacle(7060.0f, -360.0f, 2688.0f, 96.0f);
    m_Collision.AddObstacle(964.0f, -264.0f, 96.0f, 96.0f);
    m_Collision.AddObstacle(1444.0f, -264.0f, 96.0f, 144.0f);
    m_Collision.AddObstacle(1828.0f, -264.0f, 96.0f, 192.0f);
    m_Collision.AddObstacle(8212.0f, -264.0f, 96.0f, 96.0f);
    m_Collision.AddObstacle(2356.0f, -264.0f, 96.0f, 192.0f);
    m_Collision.AddObstacle(7444.0f, -264.0f, 96.0f, 96.0f);
    m_Collision.AddObstacle(6052.0f, -264.0f, 192.0f, 48.0f);
    m_Collision.AddObstacle(6100.0f, -216.0f, 144.0f, 48.0f);
    m_Collision.AddObstacle(6148.0f, -168.0f, 96.0f, 48.0f);
    m_Collision.AddObstacle(6196.0f, -120.0f, 48.0f, 48.0f);
    m_Collision.AddObstacle(6340.0f, -264.0f, 192.0f, 48.0f);
    m_Collision.AddObstacle(6340.0f, -216.0f, 144.0f, 48.0f);
    m_Collision.AddObstacle(6340.0f, -168.0f, 96.0f, 48.0f);
    m_Collision.AddObstacle(6340.0f, -120.0f, 48.0f, 48.0f);
    m_Collision.AddObstacle(6724.0f, -264.0f, 216.0f, 48.0f);
    m_Collision.AddObstacle(6772.0f, -216.0f, 192.0f, 48.0f);
    m_Collision.AddObstacle(6820.0f, -168.0f, 144.0f, 48.0f);
    m_Collision.AddObstacle(6868.0f, -120.0f, 96.0f, 48.0f);
    m_Collision.AddObstacle(7060.0f, -264.0f, 192.0f, 48.0f);
    m_Collision.AddObstacle(7060.0f, -216.0f, 144.0f, 48.0f);
    m_Collision.AddObstacle(7060.0f, -168.0f, 96.0f, 48.0f);
    m_Collision.AddObstacle(7060.0f, -120.0f, 48.0f, 48.0f);
    m_Collision.AddObstacle(8308.0f, -264.0f, 432.0f, 48.0f);
    m_Collision.AddObstacle(8356.0f, -216.0f, 384.0f, 48.0f);
    m_Collision.AddObstacle(8404.0f, -168.0f, 336.0f, 48.0f);
    m_Collision.AddObstacle(8452.0f, -120.0f, 288.0f, 48.0f);
    m_Collision.AddObstacle(8500.0f, -72.0f, 240.0f, 48.0f);
    m_Collision.AddObstacle(8548.0f, -24.0f, 192.0f, 48.0f);
    m_Collision.AddObstacle(8596.0f, 24.0f, 144.0f, 48.0f);
    m_Collision.AddObstacle(8644.0f, 72.0f, 96.0f, 48.0f);
    m_Collision.AddObstacle(15000.0f, -360.0f, 768.0f, 96.0f);
    m_Collision.AddObstacle(15000.0f, -360.0f, 48.0f, 672.0f);
    m_Collision.AddObstacle(15624.0f, -264.0f, 144.0f, 96.0f);
    m_Collision.AddObstacle(15720.0f, -264.0f, 48.0f, 432.0f);
    m_Collision.AddObstacle(15192.0f, -264.0f, 336.0f, 144.0f);

    auto entrancePipe = std::make_shared<Block>(Block::Type::PIPE_A, glm::vec2(2400.0f, -168.0f));
    m_Blocks.push_back(entrancePipe);

    auto exitPipe = std::make_shared<Block>(Block::Type::PIPE_B, glm::vec2(7492.0f, -216.0f));
    m_Blocks.push_back(exitPipe);

    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) {
            block->GetCharacter()->SetVisible(false);
            m_Root.AddChild(block->GetCharacter());
        }
    }

    AddBlock(Block::Type::QUESTION, 16, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 20, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 21, ROW_1_Y, Block::ItemType::MUSHROOM);
    AddBlock(Block::Type::BRICK_FRAGILE, 22, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 23, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 24, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 22, ROW_2_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 77, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 78, ROW_1_Y, Block::ItemType::MUSHROOM);
    AddBlock(Block::Type::BRICK_FRAGILE, 79, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 80, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 81, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 82, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 83, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 84, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 85, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 86, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 87, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 91, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 92, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 93, ROW_2_Y);
    AddBlock(Block::Type::QUESTION, 94, ROW_2_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 94, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 100, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 101, ROW_1_Y, Block::ItemType::STAR);
    AddBlock(Block::Type::QUESTION, 106, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::QUESTION, 109, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::QUESTION, 112, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::QUESTION, 109, ROW_2_Y, Block::ItemType::MUSHROOM);
    AddBlock(Block::Type::BRICK_FRAGILE, 118, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 121, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 122, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 123, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 129, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 130, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 128, ROW_2_Y);
    AddBlock(Block::Type::QUESTION, 129, ROW_2_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::QUESTION, 130, ROW_2_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 131, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 168, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 169, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 170, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 171, ROW_1_Y);
}

void App::LoadLevel2Objects() {
    if (m_Map) {
        m_Map->SetVisible(true);
    }

    auto AddBlockByGrid = [this](Block::Type type, int imgNum, int col, int row, Block::ItemType item = Block::ItemType::NONE) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (static_cast<float>(col) * 48.0f) + 24.0f;
        float worldY = -360.0f + (static_cast<float>(row) * 48.0f);

        auto block = std::make_shared<Block>(type, glm::vec2{ worldX, worldY });
        block->SetItemType(item);
        m_Blocks.push_back(block);
        m_Root.AddChild(block->GetCharacter());
        };

    auto SpawnBricksByGrid = [&](int imgNum, int startCol, int endCol, int row) {
        for (int col = startCol; col <= endCol; ++col) {
            AddBlockByGrid(Block::Type::BRICK_FRAGILE, imgNum, col, row);
        }
        };

    auto AddObstacleByGrid = [this](int startImg, int startCol, int endImg, int endCol, int startRow, int endRow) {
        float totalStartX = -450.0f + static_cast<float>(startImg - 1) * 768.0f + (static_cast<float>(startCol) * 48.0f);
        float totalEndX = -450.0f + static_cast<float>(endImg - 1) * 768.0f + (static_cast<float>(endCol) * 48.0f) + 48.0f;

        int colsCount = std::round((totalEndX - totalStartX) / 48.0f);

        for (int i = 0; i < colsCount; ++i) {
            float currentX = totalStartX + (static_cast<float>(i) * 48.0f);

            for (int r = startRow; r <= endRow; ++r) {
                float currentY = -360.0f + static_cast<float>(r) * 48.0f;
                m_Collision.AddObstacle(currentX, currentY, 48.0f, 48.0f);
            }
        }
        };

    auto AddMovingBlockByGrid = [this](const std::string& imgPath, int imgNum, float col, float row, MovingBlock::Axis axis, float range, float speed) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (col * 48.0f) + 24.0f;
        float worldY = -360.0f + (row * 48.0f);

        auto movingBlock = std::make_shared<MovingBlock>(imgPath, worldX, worldY, axis, range, speed);
        m_Blocks.push_back(movingBlock);
        m_Root.AddChild(movingBlock->GetCharacter());
        };

    auto AddCoinByGrid = [this](int imgNum, float col, float row) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float absoluteX = leftEdge + (col * 48.0f) + 24.0f;
        float absoluteY = -360.0f + (row * 48.0f);

        auto coin = std::make_shared<Coin>(absoluteX, absoluteY, 1);
        m_Root.AddChild(coin);
        m_Items.push_back(coin);
        };

    auto SpawnCoinsByGrid = [&](int imgNum, float startCol, float endCol, float row) {
        for (float col = startCol; col <= endCol; col += 1.0f) {
            AddCoinByGrid(imgNum, col, row);
        }
        };

    AddObstacleByGrid(1, 0, 1, 15, 0, 1);
    AddObstacleByGrid(2, 2, 2, 5, 2, 2);
    AddObstacleByGrid(2, 3, 2, 4, 0, 1);
    AddObstacleByGrid(2, 8, 2, 15, 5, 5);
    AddObstacleByGrid(2, 9, 2, 14, 0, 4);
    AddObstacleByGrid(2, 10, 2, 14, 9, 9);
    AddObstacleByGrid(2, 11, 2, 13, 6, 8);
    AddObstacleByGrid(3, 0, 3, 2, 2, 2);
    AddObstacleByGrid(3, 1, 3, 1, 0, 1);
    AddObstacleByGrid(3, 3, 3, 7, 6, 6);
    AddObstacleByGrid(3, 4, 3, 6, 0, 5);
    AddObstacleByGrid(3, 8, 3, 14, 10, 10);
    AddObstacleByGrid(3, 9, 3, 13, 0, 9);
    AddObstacleByGrid(4, 2, 4, 5, 1, 1);
    AddObstacleByGrid(4, 3, 4, 4, 0, 0);
    AddObstacleByGrid(4, 12, 4, 15, 9, 9);
    AddObstacleByGrid(4, 13, 4, 14, 2, 8);
    AddObstacleByGrid(4, 11, 4, 15, 1, 1);
    AddObstacleByGrid(4, 12, 4, 14, 0, 0);
    AddObstacleByGrid(5, 1, 5, 5, 1, 1);
    AddObstacleByGrid(5, 2, 5, 4, 0, 0);
    AddObstacleByGrid(5, 6, 5, 8, 5, 5);
    AddObstacleByGrid(5, 7, 5, 7, 0, 4);
    AddObstacleByGrid(5, 12, 5, 15, 8, 8);
    AddObstacleByGrid(6, 0, 6, 1, 8, 8);
    AddObstacleByGrid(5, 13, 5, 15, 0, 7);
    AddObstacleByGrid(6, 0, 6, 0, 0, 7);
    AddObstacleByGrid(7, 2, 7, 5, 3, 3);
    AddObstacleByGrid(7, 3, 7, 4, 0, 2);
    AddObstacleByGrid(7, 8, 7, 15, 7, 7);
    AddObstacleByGrid(7, 9, 7, 14, 0, 6);
    AddObstacleByGrid(8, 1, 8, 3, 1, 1);
    AddObstacleByGrid(8, 2, 8, 2, 0, 0);
    AddObstacleByGrid(8, 4, 8, 7, 5, 5);
    AddObstacleByGrid(8, 5, 8, 6, 0, 4);
    AddObstacleByGrid(8, 10, 8, 13, 5, 5);
    AddObstacleByGrid(8, 11, 8, 12, 0, 4);
    AddObstacleByGrid(9, 1, 9, 15, 0, 1);
    AddObstacleByGrid(9, 10, 9, 15, 2, 5);
    AddObstacleByGrid(9, 12, 9, 15, 6, 7);
    AddObstacleByGrid(9, 14, 9, 15, 8, 9);
    AddObstacleByGrid(10, 0, 10, 15, 0, 1);
    AddObstacleByGrid(11, 0, 11, 3, 0, 1);

    if (m_CheatEnabled) {
        AddObstacleByGrid(1, 0, 11, 15, 0, 1);
    }

    AddBlockByGrid(Block::Type::QUESTION, 4, 11, 6, Block::ItemType::MUSHROOM);

    AddCoinByGrid(2, 11, 10.5);
    AddCoinByGrid(2, 12, 10.5);
    AddCoinByGrid(2, 13, 10.5);
    AddCoinByGrid(3, 1, 3.5);
    AddCoinByGrid(3, 6, 12);
    AddCoinByGrid(3, 7, 12);
    AddCoinByGrid(4, 12, 10.5);
    AddCoinByGrid(4, 13, 10.5);
    AddCoinByGrid(4, 14, 10.5);
    AddCoinByGrid(4, 15, 10.5);
    AddCoinByGrid(6, 4, 9.5);
    AddCoinByGrid(6, 5, 9.5);
    AddCoinByGrid(6, 13, 10.5);
    AddCoinByGrid(6, 14, 10.5);
    AddCoinByGrid(7, 1, 10.5);
    AddCoinByGrid(7, 2, 10.5);
    AddCoinByGrid(8, 1, 2.5);
    AddCoinByGrid(8, 2, 2.5);
    AddCoinByGrid(8, 3, 2.5);
    AddCoinByGrid(8, 8, 9.5);
    AddCoinByGrid(8, 9, 9.5);

    std::string platformImg = GA_RESOURCE_DIR"/Image/Items/road.png";
    AddMovingBlockByGrid(platformImg, 4, 8.0f, 5.0f, MovingBlock::Axis::VERTICAL, 155.0f, 100.0f);
    AddMovingBlockByGrid(platformImg, 6, 6.0f, 5.0f, MovingBlock::Axis::HORIZONTAL, 160.0f, 110.0f);
    AddMovingBlockByGrid(platformImg, 6, 12.0f, 6.f, MovingBlock::Axis::HORIZONTAL, 160.0f, 130.0f);
    AddMovingBlockByGrid(platformImg, 9, 4.0f, 7.0f, MovingBlock::Axis::HORIZONTAL, 150.0f, 110.0f);
}

void App::LoadLevel3Objects() {
    if (m_Map) {
        m_Map->SetVisible(true);
    }

    auto AddBlockByGrid = [this](Block::Type type, int imgNum, float col, float row, Block::ItemType item = Block::ItemType::NONE) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (col * 48.0f) + 24.0f;
        float worldY = -360.0f + (row * 48.0f);

        auto block = std::make_shared<Block>(type, glm::vec2{ worldX, worldY });
        block->SetItemType(item);
        m_Blocks.push_back(block);
        m_Root.AddChild(block->GetCharacter());
        };

    auto SpawnBricksByGrid = [&](int imgNum, float startCol, float endCol, float row) {
        for (float col = startCol; col <= endCol; col += 1.0f) {
            AddBlockByGrid(Block::Type::BRICK_FRAGILE, imgNum, col, row);
        }
        };

    auto AddObstacleByGrid = [this](int startImg, float startCol, int endImg, float endCol, float startRow, float endRow) {
        float totalStartX = -450.0f + static_cast<float>(startImg - 1) * 768.0f + (startCol * 48.0f);
        float totalEndX = -450.0f + static_cast<float>(endImg - 1) * 768.0f + (endCol * 48.0f) + 48.0f;

        int colsCount = std::round((totalEndX - totalStartX) / 48.0f);

        for (int i = 0; i < colsCount; ++i) {
            float currentX = totalStartX + (static_cast<float>(i) * 48.0f);
            for (float r = startRow; r <= endRow; r += 1.0f) {
                float currentY = -360.0f + r * 48.0f;
                m_Collision.AddObstacle(currentX, currentY, 48.0f, 48.0f);
            }
        }
        };

    auto AddMovingBlockByGrid = [this](const std::string& imgPath, int imgNum, float col, float row, MovingBlock::Axis axis, float range, float speed) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (col * 48.0f) + 24.0f;
        float worldY = -360.0f + (row * 48.0f);

        auto movingBlock = std::make_shared<MovingBlock>(imgPath, worldX, worldY, axis, range, speed);
        m_Blocks.push_back(movingBlock);
        m_Root.AddChild(movingBlock->GetCharacter());
        };

    // ==========================================
    // 🌟 新增：火柱輔助產生器
    // ==========================================
    auto AddFireBarByGrid = [this](int imgNum, float col, float row, int numFireballs, float speed, float initAngle = 0.0f) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (col * 48.0f) + 24.0f;
        // Y軸加上 24.0f 讓火柱旋轉中心對齊方塊的中心點
        float worldY = -360.0f + (row * 48.0f) + 24.0f;

        auto fireBar = std::make_shared<FireBar>(worldX, worldY, numFireballs, speed, initAngle);

        // 將火柱的每一顆火球加進渲染樹
        for (auto& fireballAnim : fireBar->GetDrawables()) {
            m_Root.AddChild(fireballAnim);
        }
        m_FireBars.push_back(fireBar);
    };

    auto AddCoinByGrid = [this](int imgNum, float col, float row) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float absoluteX = leftEdge + (col * 48.0f) + 24.0f;
        float absoluteY = -360.0f + (row * 48.0f);

        auto coin = std::make_shared<Coin>(absoluteX, absoluteY, 1);
        m_Root.AddChild(coin);
        m_Items.push_back(coin);
        };

    AddObstacleByGrid(1, 0, 1, 2, 0, 7);
    AddObstacleByGrid(1, 3, 1, 3, 0, 6);
    AddObstacleByGrid(1, 4, 1, 4, 0, 5);
    AddObstacleByGrid(1, 5, 1, 12, 0, 4);
    AddObstacleByGrid(1, 15, 1, 15, 0, 4);
    AddObstacleByGrid(1, 0, 1, 15, 10, 12);
    AddObstacleByGrid(2, 0, 2, 9, 0, 4);
    AddObstacleByGrid(2, 13, 2, 15, 0, 4);
    AddObstacleByGrid(2, 0, 2, 7, 10, 12);
    AddObstacleByGrid(2, 7, 2, 7, 9, 9);
    AddObstacleByGrid(2, 8, 2, 15, 12, 12);
    AddObstacleByGrid(3, 3, 3, 15, 0, 5);
    AddObstacleByGrid(3, 0, 3, 4, 12, 12);
    AddObstacleByGrid(3, 5, 3, 15, 9, 12);
    AddObstacleByGrid(4, 0, 4, 15, 0, 5);
    AddObstacleByGrid(4, 0, 4, 15, 9, 12);
    AddObstacleByGrid(5, 0, 5, 7, 0, 5);
    AddObstacleByGrid(5, 8, 5, 15, 0, 4);
    AddObstacleByGrid(5, 0, 5, 7, 9, 12);
    AddObstacleByGrid(5, 8, 5, 15, 12, 12);
    AddObstacleByGrid(6, 0, 6, 15, 0, 4);
    AddObstacleByGrid(6, 0, 6, 15, 12, 12);
    AddObstacleByGrid(6, 0, 6, 0, 11, 11);
    AddObstacleByGrid(6, 8, 6, 8, 11, 11);
    AddObstacleByGrid(7, 0, 7, 7, 0, 4);
    AddObstacleByGrid(7, 8, 7, 15, 0, 1);
    AddObstacleByGrid(7, 0, 7, 15, 12, 12);
    AddObstacleByGrid(7, 1, 7, 7, 10, 11);
    AddObstacleByGrid(8, 0, 8, 15, 0, 1);
    AddObstacleByGrid(8, 4, 8, 7, 2, 4);
    AddObstacleByGrid(8, 11, 8, 15, 2, 4);
    AddObstacleByGrid(8, 0, 8, 15, 12, 12);
    AddObstacleByGrid(8, 11, 8, 15, 10, 11);
    AddObstacleByGrid(9, 0, 9, 12, 4, 4);
    AddObstacleByGrid(9, 13, 9, 15, 0, 5);
    AddObstacleByGrid(9, 0, 9, 15, 12, 12);
    AddObstacleByGrid(9, 14, 9, 15, 9, 11);
    AddObstacleByGrid(10, 0, 10, 15, 0, 1);
    AddObstacleByGrid(10, 0, 10, 15, 12, 12);

    // 孤立半空的方塊
    AddObstacleByGrid(2, 7, 2, 7, 8, 8);
    AddObstacleByGrid(3, 5, 3, 5, 8, 8);
    AddObstacleByGrid(4, 1, 4, 1, 8, 8);
    AddObstacleByGrid(4, 12, 4, 12, 8, 8);
    AddObstacleByGrid(5, 3, 5, 3, 8, 8);
    AddObstacleByGrid(5, 12, 5, 12, 5, 5);
    AddObstacleByGrid(6, 0, 6, 0, 10, 10);
    AddObstacleByGrid(6, 4, 6, 4, 5, 5);
    AddObstacleByGrid(6, 8, 6, 8, 10, 10);
    AddObstacleByGrid(6, 12, 6, 12, 5, 5);


    // 🔥 綁定在半空方塊上的火柱大軍
    // 參數：(圖編號, X格, Y格, 顆數, 旋轉速度, 初始角度)
    AddFireBarByGrid(2, 7.0f, 8.0f, 6, -2.5f, 0.0f);     // 順時針
    AddFireBarByGrid(3, 5.0f, 8.0f, 6, 2.5f, 1.57f);     // 逆時針，初始朝上
    AddFireBarByGrid(4, 1.0f, 8.0f, 6, -3.0f, 3.14f);    // 順時針(快)，初始朝左
    AddFireBarByGrid(4, 12.0f, 8.0f, 6, 2.0f, 0.0f);     // 逆時針
    AddFireBarByGrid(5, 3.0f, 8.0f, 6, -2.5f, 1.0f);
    AddFireBarByGrid(6, 4.0f, 5.0f, 6, 2.5f, 0.0f);
    AddFireBarByGrid(6, 12.0f, 5.0f, 6, -2.5f, 3.14f);


    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 10.0f, 5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 13.0f, 5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 11.0f, 9.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 14.0f, 9.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 8, 0.0f, 5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 8, 1.0f, 9.0f, Block::ItemType::COIN);

    AddBlockByGrid(Block::Type::QUESTION, 2, 14.0f, 9.0f, Block::ItemType::MUSHROOM);

    std::string platformImg = GA_RESOURCE_DIR"/Image/Items/road.png";
    AddMovingBlockByGrid(platformImg, 9, 6.0f, 9.0f, MovingBlock::Axis::HORIZONTAL, 150.0f, 100.0f);

    m_Player->SetWorldPosition(-400.0f, 80.0f);
    m_Player->ResetStatus();
    m_Player->GetCharacter()->SetVisible(true);
}

void App::SwitchLevel(int nextLevel) {
    for (auto& block : m_Blocks) {
        m_Root.RemoveChild(block->GetCharacter());
    }
    m_Blocks.clear();
    m_Collision.Clear();

    // 🌟 清除上一關殘留的火柱，避免跨關卡作亂
    for (auto& fb : m_FireBars) {
        for (auto& anim : fb->GetDrawables()) {
            m_Root.RemoveChild(anim);
        }
    }
    m_FireBars.clear();

    // 🌟 【新增：安全機制】切換關卡時，徹底拔除並清空鐵鎚物件 🌟
    if (m_Level3Hammer && m_Level3Hammer->GetCharacter()) {
        m_Root.RemoveChild(m_Level3Hammer->GetCharacter());
    }
    m_Level3Hammer = nullptr;

    if (nextLevel == 2) {
        LoadLevel2Objects();
    }
    else if (nextLevel == 3) {
        LoadLevel3Objects();
    }
}
void App::AddBlock(Block::Type type, int gridX, float gridY, Block::ItemType item) {
    float worldX = -360.0f + (static_cast<float>(gridX) * 48.0f);

    auto block = std::make_shared<Block>(type, glm::vec2{ worldX, gridY });
    block->SetItemType(item);

    m_Blocks.push_back(block);
    m_Root.AddChild(block->GetCharacter());
}