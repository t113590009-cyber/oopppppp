#include "App.hpp"
#include "Util/Logger.hpp"
#include "Coin.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 1. 載入所有物件
    m_Menu = std::make_unique<Menu>();
    m_Player = std::make_unique<Player>();
    m_Map = std::make_unique<Map>();

    m_Map->Init(m_Root);

    // ==========================================
    // 🚩 旗桿與旗子初始化
    // ==========================================
    // 1. 旗桿 (elv_win)
    m_Flagpole = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/elv_win.png");
    m_Flagpole->m_Transform.scale = { 3.0f, 3.0f };
    m_Flagpole->SetZIndex(1); // 設在瑪利歐後面
    m_Flagpole->SetPosition({ 9147.0f, -95.0f }); // ⚠️ X 座標請依你的地圖實際狀況微調
    m_Flagpole->SetVisible(false);
    m_Root.AddChild(m_Flagpole);

    // 2. 旗子 (flag_win)
    m_Flag = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Props/1-1/flag_win.png");
    m_Flag->m_Transform.scale = { 3.0f, 3.0f };
    m_Flag->SetZIndex(2); // 設在旗桿前面一點點
    m_Flag->SetPosition({ 9120.0f, -22.0f });
    m_Flag->SetVisible(false);
    m_Root.AddChild(m_Flag);

    // 🌟 城堡初始化 (使用第一份的圖片路徑)
    m_Castle = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Background/stage1-1/castle1_1_win.png");
    m_Castle->m_Transform.scale = { 3.0f, 3.0f }; // 配合地圖比例放大
    m_Castle->SetZIndex(2);                       // 放在地圖上方，瑪利歐下方
    m_Castle->SetPosition({ 9436.0f, -145.0f });  // 設定在關卡最後端
    m_Castle->SetVisible(false);                  // 初始隱藏
    m_Root.AddChild(m_Castle);

    // 2. 把標題和游標推上舞台
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

    // 3. 畫黑邊 UI
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

    // ==========================================
    // 🌟 新增：載入失敗畫面 UI
    // ==========================================
    m_FailScreen = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/UI/fail.png");
    m_FailScreen->m_Transform.scale = { 3.0f, 3.0f }; // 可以依照你的需求調整大小
    m_FailScreen->SetZIndex(100);                     // 設為 100 確保蓋在最上層
    m_FailScreen->SetPosition({ 0.0f, 0.0f });        // 放在畫面正中央
    m_FailScreen->SetVisible(false);                  // 一開始先隱藏
    m_Root.AddChild(m_FailScreen);

    // ==========================================
    // 4. 碰撞與玩家 (隱形物理牆壁)
    // ==========================================
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

    // ==========================================
    // 🚇 地下室物理碰撞 (藏在地圖極右邊的 15000 區)
    // ==========================================
    m_Collision.AddObstacle(15000.0f, -360.0f, 768.0f, 96.0f);
    m_Collision.AddObstacle(15000.0f, -360.0f, 48.0f, 672.0f);
    m_Collision.AddObstacle(15624.0f, -264.0f, 144.0f, 96.0f);
    m_Collision.AddObstacle(15720.0f, -264.0f, 48.0f, 432.0f);
    m_Collision.AddObstacle(15192.0f, -264.0f, 336.0f, 144.0f);

    // ==========================================
    // 🪙 新增：固定金幣放置工具
    // ==========================================
    auto AddFixedCoin = [&](float absoluteX, float absoluteY) {
        auto coin = std::make_shared<Coin>(absoluteX, absoluteY, 1); // 1 = 懸浮金幣
        m_Root.AddChild(coin);
        m_Items.push_back(coin);
    };

    // ==========================================
    // 💰 經典 1-1 地下室隱藏金幣區
    // ==========================================
    // 根據你的物理牆壁設定，地下室中間突起的磚塊台起點在 X: 15192.0f
    // 磚塊台頂部高度大約在 Y: -120.0f，所以我們金幣中心點從 Y: -96.0f 開始往上疊

    float ugStartX = 15192.0f + 24.0f; // 加上 24.0f 讓金幣對齊磚塊的中心點
    float ugBaseY = -96.0f;            // 底層金幣的高度

    // 1️⃣ 下層 (7 枚金幣，鋪滿磚塊台)
    for (int i = 0; i < 7; ++i) {
        AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY);
    }

    // 2️⃣ 中層 (7 枚金幣，疊在下層正上方)
    for (int i = 0; i < 7; ++i) {
        AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY + 96.0f); // Y 往上提一個磚塊 (48.0f)
    }

    // 3️⃣ 上層 (5 枚金幣，左右各留空一格，所以 i 從 1 開始到 5)
    for (int i = 1; i <= 5; ++i) {
        AddFixedCoin(ugStartX + (i * 48.0f), ugBaseY + 192.0f); // Y 往上提兩個磚塊 (96.0f)
    }

    // ==========================================
    // 🍄 經典 1-1 關卡生成區 (視覺磚塊與水管)
    // ==========================================
    const float TILE = 48.0f;
    const float START_X = -360.0f;
    const float ROW_1_Y = -72.0f;
    const float ROW_2_Y = 120.0f;

    // 🌟 1. 幫 AddBlock 新增第三個參數 itemType，預設是 NONE (空的)
    auto AddBlock = [&](Block::Type type, float gridX, float absoluteY, Block::ItemType itemType = Block::ItemType::NONE) {
        float absoluteX = START_X + (gridX * TILE);
        auto block = std::make_shared<Block>(type, glm::vec2(absoluteX, absoluteY));

        // 🌟 2. 在這裡把道具塞進方塊的肚子裡！
        block->SetItemType(itemType);

        m_Blocks.push_back(block);
        m_Root.AddChild(block->GetCharacter());
    };

    auto entrancePipe = std::make_shared<Block>(Block::Type::PIPE_A, glm::vec2(2400.0f, -168.0f));
    m_Blocks.push_back(entrancePipe);
    m_Root.AddChild(entrancePipe->GetCharacter());

    auto exitPipe = std::make_shared<Block>(Block::Type::PIPE_B, glm::vec2(7492.0f, -216.0f));
    m_Blocks.push_back(exitPipe);
    m_Root.AddChild(exitPipe->GetCharacter());

    // 🌟 3. 開始塞道具！
    // 這是第一個問號方塊 (gridX = 16)，我們塞入蘑菇！
    AddBlock(Block::Type::QUESTION, 16, ROW_1_Y, Block::ItemType::MUSHROOM);

    AddBlock(Block::Type::BRICK_FRAGILE, 20, ROW_1_Y, Block::ItemType::COIN);

    // 這是第二個問號方塊 (gridX = 21)，我們塞入星星！
    AddBlock(Block::Type::QUESTION, 21, ROW_1_Y, Block::ItemType::STAR);

    // 下面的都不用改，因為我們預設參數給了 NONE，所以它們都會是空方塊！
    AddBlock(Block::Type::BRICK_FRAGILE, 22, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 23, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 24, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 22, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 77, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 78, ROW_1_Y);
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
    AddBlock(Block::Type::QUESTION, 94, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 94, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 100, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 101, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 106, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 109, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 112, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 109, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 118, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 121, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 122, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 123, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 129, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 130, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 128, ROW_2_Y);
    AddBlock(Block::Type::QUESTION, 129, ROW_2_Y);
    AddBlock(Block::Type::QUESTION, 130, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 131, ROW_2_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 168, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 169, ROW_1_Y);
    AddBlock(Block::Type::QUESTION, 170, ROW_1_Y);
    AddBlock(Block::Type::BRICK_FRAGILE, 171, ROW_1_Y);

    // --- (最後把瑪利歐加進去，確保他在最上層) ---
    m_Root.AddChild(m_Player->GetCharacter());

    // ==========================================
    // 🌟 載入上方 UI 並加進 m_Root
    // ==========================================
    m_TopUI = std::make_shared<TopUI>();
    for (auto& uiElem : m_TopUI->GetDrawables()) {
        m_Root.AddChild(uiElem);
    }

    // 🌟 新增這行：因為還在「封面」，所以先把它全數隱藏！
    m_TopUI->SetVisible(false);

    m_CurrentState = State::UPDATE;
}