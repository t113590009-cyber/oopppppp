#include "App.hpp"
#include "Util/Logger.hpp"
#include "Coin.hpp"
#include "MovingBlock.hpp" // 🌟 1. 新增：引入移動平台的標頭檔

void App::Start() {
    LOG_TRACE("Start");

    // 1. 載入所有基礎物件
    m_Menu = std::make_unique<Menu>();
    m_Player = std::make_unique<Player>();
    m_Map = std::make_unique<Map>();
    m_Map->Init(m_Root);

    // 🚩 旗桿、旗子與城堡初始化
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

    // 🌟 【第二關專屬】旗桿、旗子與城堡初始化
    m_FlagpoleLvl2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/elv_win.png");
    m_FlagpoleLvl2->m_Transform.scale = { 3.0f, 3.0f };
    m_FlagpoleLvl2->SetZIndex(1);
    m_FlagpoleLvl2->SetVisible(false); // 先隱藏
    m_Root.AddChild(m_FlagpoleLvl2);

    m_FlagLvl2 = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/flag_win.png");
    m_FlagLvl2->m_Transform.scale = { 3.0f, 3.0f };
    m_FlagLvl2->SetZIndex(2);
    m_FlagLvl2->SetVisible(false);
    m_Root.AddChild(m_FlagLvl2);

    // 使用你提供的第二關專屬城堡圖片！
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

    // 🌟 1. 將水管生成並存入 m_Blocks 容器中（這樣 ResetLevel 時才能徹底被大掃除）
    auto entrancePipe = std::make_shared<Block>(Block::Type::PIPE_A, glm::vec2(2400.0f, -168.0f));
    m_Blocks.push_back(entrancePipe);

    auto exitPipe = std::make_shared<Block>(Block::Type::PIPE_B, glm::vec2(7492.0f, -216.0f));
    m_Blocks.push_back(exitPipe);

    // 🌟 3. 核心關鍵：在這裡補上這個迴圈，確保「遊戲第一次剛啟動」時，水管、平台和所有方塊會立刻安全地被加進畫面中！
    for (auto& block : m_Blocks) {
        if (block && block->GetCharacter()) {
            block->GetCharacter()->SetVisible(false); // 剛開局在主選單時先隱形，點 Start 後才會秀出來
            m_Root.AddChild(block->GetCharacter());   // 正式加入渲染樹中
        }
    }


    AddBlock(Block::Type::QUESTION, 16, ROW_1_Y, Block::ItemType::COIN);
    AddBlock(Block::Type::BRICK_FRAGILE, 20, ROW_1_Y, Block::ItemType::MUSHROOM);
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

    // ⚙️ 【純格子對齊工具組】
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

    // 🧱 【純格子地圖配置區】
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

    // 🎁 【2-3 關】問號箱子配置
    AddBlockByGrid(Block::Type::QUESTION, 4, 11, 4, Block::ItemType::MUSHROOM);

    // 💰 【金幣配置專區】
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

    // 🎯 3. 移動平台動態配置專區
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

    // ⚙️ 【純格子對齊工具組】（已改為 float，支援帶有 .5f 的半格定位）

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

    // 🏆 終極版空氣牆工具 (自動切割成 48x48 完美方塊，已改為 float 支援半格)
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

    // 🌟 【新增工具 D：移動平台專用格子工具】(同樣支援 float 半格定位)
    auto AddMovingBlockByGrid = [this](const std::string& imgPath, int imgNum, float col, float row, MovingBlock::Axis axis, float range, float speed) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float worldX = leftEdge + (col * 48.0f) + 24.0f;
        float worldY = -360.0f + (row * 48.0f);

        auto movingBlock = std::make_shared<MovingBlock>(imgPath, worldX, worldY, axis, range, speed);
        m_Blocks.push_back(movingBlock);
        m_Root.AddChild(movingBlock->GetCharacter());
    };

    // 🪙 【新增工具 E：金幣專用格子生成器】(參數改為 float，完美支援半格定位)
    auto AddCoinByGrid = [this](int imgNum, float col, float row) {
        float leftEdge = -450.0f + static_cast<float>(imgNum - 1) * 768.0f;
        float absoluteX = leftEdge + (col * 48.0f) + 24.0f;
        float absoluteY = -360.0f + (row * 48.0f);

        auto coin = std::make_shared<Coin>(absoluteX, absoluteY, 1);
        m_Root.AddChild(coin);
        m_Items.push_back(coin);
    };

    // 🪙 【新增工具 F：整排金幣鋪設器】(同步支援 float 參數)
    auto SpawnCoinsByGrid = [&](int imgNum, float startCol, float endCol, float row) {
        for (float col = startCol; col <= endCol; col += 1.0f) {
            AddCoinByGrid(imgNum, col, row);
        }
    };

    // 鋪設 1-3 的基本地平線地板
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

    // 🧱 【新增：隱形暗牆配置專區】
    // 一開始是黑色(blackblock)，頂到現形(afterblack)，再頂變(finalafterblack)，裡面藏金幣(COIN)
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 10.0f, 5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 13.0f, 5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 11.0f, 9.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 7, 14.0f, 9.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 8, 0.0f,  5.0f, Block::ItemType::COIN);
    AddBlockByGrid(Block::Type::INVISIBLE_ITEM, 8, 1.0f,  9.0f, Block::ItemType::COIN);

    // 🎁 【第 3 關】特定的問號箱子配置
    AddBlockByGrid(Block::Type::QUESTION, 2, 14.0f, 8.0f, Block::ItemType::MUSHROOM);

    // 🎯 移動平台配置區
    std::string platformImg = GA_RESOURCE_DIR"/Image/Items/road.png";
    AddMovingBlockByGrid(platformImg, 9, 6.0f, 9.0f, MovingBlock::Axis::HORIZONTAL, 150.0f, 100.0f);

    // 重置瑪利歐位置的程式碼
    m_Player->SetWorldPosition(-400.0f, 80.0f);
    m_Player->ResetStatus();
    m_Player->GetCharacter()->SetVisible(true);
}

// 🔄 新增：萬能的關卡切換器 (直接貼在 LoadLevel3Objects 下方)
void App::SwitchLevel(int nextLevel) {
    // 1. 🧹 徹底清空上一關的殘留物
    for (auto& block : m_Blocks) {
        m_Root.RemoveChild(block->GetCharacter());
    }
    m_Blocks.clear();
    m_Collision.Clear(); // 清除上一關所有的隱形牆

    // 2. 🗺️ 載入新關卡
    if (nextLevel == 2) {
        LoadLevel2Objects();
    }
    else if (nextLevel == 3) {
        LoadLevel3Objects();
    }

    // (註：如果有重置瑪利歐位置與鏡頭的需求，請在這邊加上你專案對應的變數，例如 m_Player->SetPosition(...) 等等)
}

void App::AddBlock(Block::Type type, int gridX, float gridY, Block::ItemType item) {
    float worldX = -360.0f + (static_cast<float>(gridX) * 48.0f);

    auto block = std::make_shared<Block>(type, glm::vec2{ worldX, gridY });
    block->SetItemType(item);

    m_Blocks.push_back(block);
    m_Root.AddChild(block->GetCharacter());
}