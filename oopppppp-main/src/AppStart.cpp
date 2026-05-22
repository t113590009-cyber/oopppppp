#include "App.hpp"
#include "Util/Logger.hpp"
#include "Coin.hpp"

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

    // 🌟 2. 核心關鍵：在這裡補上這個迴圈，確保「遊戲第一次剛啟動」時，水管和所有方塊會立刻安全地被加進畫面中！
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
    float ROW_1_Y = -72.0f;

    // 🌟 修正：只有進入第二關時，這裡才會配合 ResetLevel() 確保可見
    if (m_Map) {
        m_Map->SetVisible(true);
    }

    AddBlock(Block::Type::BRICK_FRAGILE, 10, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 12, ROW_1_Y, Block::ItemType::MUSHROOM);

    m_Collision.AddObstacle(-1000.0f, -360.0f, 10000.0f, 96.0f);
}

void App::AddBlock(Block::Type type, int gridX, float gridY, Block::ItemType item) {
    float worldX = -360.0f + (static_cast<float>(gridX) * 48.0f);

    auto block = std::make_shared<Block>(type, glm::vec2{ worldX, gridY });
    block->SetItemType(item);

    m_Blocks.push_back(block);
    m_Root.AddChild(block->GetCharacter());
}