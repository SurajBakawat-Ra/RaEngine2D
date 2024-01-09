#include "Scene_Menu.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "Scene_Play.h"
#include "Physics.h"

Scene_Play::Scene_Play() : Scene() { init(); }

void Scene_Play::init()
{
    GameEngine::Instance()->window().setKeyRepeatEnabled(false);
    m_menuText.setFont(GameEngine::Instance()->getAssets().getFont("PIXEL"));

    registerAction(sf::Keyboard::Z, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::P, "TOGGLE_GRID");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::F, "FIRE");

    //Player
    {
        m_player = GameEngine::Instance()->entityManager.addEntity("PLAYER");
        auto bigBrick = GameEngine::Instance()->entityManager.addEntity("PLAYER2");

        m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("RUN"), true);
        m_player->addComponents<CTransform>(GameEngine::gridToMidPixel((float)100, (float)200, m_player));
        m_player->addComponents<CBoundingBox>(m_player->getComponent<CAnimation>().animation.getSize() / 1.25f);
        m_player->addComponents<CRigidbody>(Rigidbody(), 0.1);
        bigBrick->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("AniBRICK"), true);
        bigBrick->addComponents<CTransform>(GameEngine::gridToMidPixel((float)100, (float)500, bigBrick));
        bigBrick->addComponents<CBoundingBox>(bigBrick->getComponent<CAnimation>().animation.getSize());
        //player2->addComponents<CRigidbody>(Rigidbody(), 0.1);

        m_playerFeet = GameEngine::Instance()->entityManager.addEntity("FEET");
        m_playerFeet->addComponents<CTransform>(GameEngine::gridToMidPixel((float)64, (float)600, m_playerFeet));
        m_playerFeet->getComponent<CTransform>().SetPosition(Vec2(m_player->getComponent<CTransform>().getPos().x, (m_player->getComponent<CTransform>().getPos().y + m_player->getComponent<CBoundingBox>().size.y/2) + 10));
        m_playerFeet->addComponents<CBoundingBox>(Vec2(200, 2));
    }

    //TILES
    {
        for (int i = 0; i < 10; i++)
        {
            auto brick = GameEngine::Instance()->entityManager.addEntity("TILE");
            brick->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("AniBRICK"), true);
            brick->addComponents<CTransform>(GameEngine::gridToMidPixel((float)64 * i, (float)600, brick));
            brick->addComponents<CBoundingBox>(brick->getComponent<CAnimation>().animation.getSize());
        }
    }


}

void Scene_Play::update()
{
    sRender();
    sUserInput();
    sMovement();

    m_playerFeet->getComponent<CTransform>().SetPosition(Vec2(m_player->getComponent<CTransform>().getPos().x, (m_player->getComponent<CTransform>().getPos().y + m_player->getComponent<CBoundingBox>().size.y / 2) + 10));

    std::shared_ptr<Entity> m_player;

    for (auto& e : GameEngine::Instance()->entityManager.getEntities("PLAYER"))
    {
        m_player = e;
    }

    auto& input = m_player->getComponent<CInput>();

    if (!input.right && !input.left && !input.shoot && !input.up)
    {
        if (isPlayerGrounded())
        {
            m_player->getComponent<CAnimation>().animation = GameEngine::Instance()->getAssets().getAnimation("STAND");
        }
        else
        {
            m_player->getComponent<CAnimation>().animation = GameEngine::Instance()->getAssets().getAnimation("JUMP");
        }
    }
}

void Scene_Play::sUserInput()
{
    //sf::Event event{};
    //while (GameEngine::Instance()->window().pollEvent(event))
    //{
    //    if (event.type == sf::Event::Closed)
    //    {
    //        GameEngine::Instance()->quit();
    //    }
    //    // this event is triggered when a key is pressed
    //    if (event.type == sf::Event::KeyPressed)
    //    {
    //        switch (event.key.code)
    //        {
    //        case sf::Keyboard::Escape:
    //        {
    //            GameEngine::Instance()->quit();
    //            break;
    //        }
    //        case sf::Keyboard::W:
    //        {
    //            if (m_selectedMenuIndex > 0)
    //            {
    //                m_selectedMenuIndex--;
    //            }
    //            else
    //            {
    //                m_selectedMenuIndex = m_menuStrings.size() - 1;
    //            }
    //            break;
    //        }
    //        case sf::Keyboard::S:
    //        {
    //            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
    //            break;
    //        }
    //        case sf::Keyboard::D:
    //        {
    //            GameEngine::Instance()->changeScene("MENU", std::make_shared<Scene_Menu>());
    //            GameEngine::Instance()->removeAllEntities();
    //            break;
    //        }
    //        default:
    //        {
    //            break;
    //        }
    //        }
    //    }
    //}
}

void Scene_Play::sRender()
{
    m_menuText.setString("Left :  A        S :  Down        Up : W        FIRE : F");
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setCharacterSize(12);
    m_menuText.setPosition(sf::Vector2f(m_menuText.getCharacterSize(), height() - m_menuText.getCharacterSize() * 2));
    GameEngine::Instance()->window().draw(m_menuText);

    GameEngine::Instance()->window().display();
}

void Scene_Play::onEnd() {}

void Scene_Play::sDoAction(const InputAction& action)
{
    std::shared_ptr<Entity> m_player;


    for (auto& e : GameEngine::Instance()->entityManager.getEntities("PLAYER"))
    {
        m_player = e;
    }

    if (action.type() == START)
    {
        if (isPlayerGrounded())
        {
            m_player->getComponent<CInput>().canJump = true;
        }
        else
        {
            m_player->getComponent<CInput>().canJump = false;
        }

        if (m_player->getComponent<CInput>().canJump)
        {
            m_player->getComponent<CInput>().down = false;
            m_player->getComponent<CInput>().left = false;
            m_player->getComponent<CInput>().right = false;
            m_player->getComponent<CInput>().shoot = false;
        }

        if (action.name() == "PAUSE")
        {
            setPaused(!m_paused);
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
        else if (action.name() == "UP")
        {
            if (m_player->getComponent<CInput>().canJump)
            {
                m_player->getComponent<CInput>().up = true;
                m_player->getComponent<CState>().state = "JUMPING";
                m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("JUMP"), true);
                m_player->getComponent<CTransform>().aimPos = m_player->getComponent<CTransform>().getPos().y - 5.f;
                m_player->getComponent<CInput>().canJump = false;
                m_player->getComponent<CState>().notGrounded = true;
            }
        }
        else if (action.name() == "FIRE")
        {
            m_player->getComponent<CInput>().shoot = true;
            m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("SHOT"), true);
            //spawnbullet
            m_player->getComponent<CState>().state = "SHOUTING";
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
            if (isPlayerGrounded())
                m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("RUN"), true);
            (m_player->getComponent<CTransform>().scale.x > 0) ? m_player->getComponent<CTransform>().scale.x *= -1 : 1;
            m_player->getComponent<CState>().state = "RUNNING";
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
            if (isPlayerGrounded())
                m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("RUN"), true);
            m_player->getComponent<CState>().state = "RUNNING";
            (m_player->getComponent<CTransform>().scale.x < 0) ? m_player->getComponent<CTransform>().scale.x *= -1 : 1;
        }
    }
    else if (action.type() == END)
    {
        if (action.name() == "UP")
        {
            if (m_player->getComponent<CInput>().up)
            {
                m_player->getComponent<CInput>().up = false;
                m_player->getComponent<CTransform>().velocity.y = 0;
                m_player->getComponent<CState>().notGrounded = true;
            }
        }
        else if (action.name() == "FIRE")
        {
            m_player->getComponent<CInput>().shoot = false;
            m_player->getComponent<CState>().state = "STANDING";
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
    }
}

void Scene_Play::sMovement()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities("PLAYER"))
    {
        if (e->getComponent<CInput>().left)
        {
            e->getComponent<CTransform>().velocity.x = -5;
        }
        else if (e->getComponent<CInput>().right)
        {
            e->getComponent<CTransform>().velocity.x = 5;
        }
        else
        {
            if (isPlayerGrounded())
                e->getComponent<CTransform>().velocity.x = 0;
        }

        if (e->getComponent<CInput>().up)
        {
            e->getComponent<CTransform>().velocity.y = -10;
        }

        //std::cout << e->getComponent<CTransform>().velocity << std::endl;
    }
}

bool Scene_Play::isPlayerGrounded()
{
    bool collidingDown = false;

    std::shared_ptr<Entity> m_player;

    for (auto& e : GameEngine::Instance()->entityManager.getEntities("PLAYER"))
    {
        m_player = e;
    }

    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {

        if (e->id() != m_playerFeet->id() && (Physics::GetOverlap(m_playerFeet, e) > Vec2(0.f, 0.f)))
        {
            collidingDown = true;
            break;
        }
    }

    return collidingDown;
}



