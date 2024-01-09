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
    m_title = "Not Mario";
    m_menuStrings.emplace_back("Menu 1");
    m_menuStrings.emplace_back("Menu 2");
    m_menuStrings.emplace_back("Menu 3");
    
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
        auto player = GameEngine::Instance()->entityManager.addEntity("PLAYER");
        player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("RUN"), true);
        player->addComponents<CTransform>(GameEngine::gridToMidPixel((float)100, (float)200, player));
        player->addComponents<CBoundingBox>(player->getComponent<CAnimation>().animation.getSize() / 1.25f);
        player->addComponents<CRigidbody>(Rigidbody(), 0.1);
    }

    //TILES
    {
        for (int i = 0; i < 5; i++)
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
    // drawing the game title in the top-left of the screen
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(32);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(GameEngine::Instance()->window().getSize().x / 2 - m_menuText.getCharacterSize() * 3, 10));
    GameEngine::Instance()->window().draw(m_menuText);
    // draw all the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings.at(i));
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color::Black);
        m_menuText.setPosition(sf::Vector2f(GameEngine::Instance()->window().getSize().x / 2 - m_menuText.getCharacterSize() * 2, 250 + i * 72));
        GameEngine::Instance()->window().draw(m_menuText);
    }

    m_menuText.setString("Left :  A        S :  Down        Up : W        D : Do");
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
    bool collidingDown = false;

    for (auto& e : GameEngine::Instance()->entityManager.getEntities("PLAYER"))
    {
        m_player = e;
    }

    if (action.type() == START)
    {
        
        for (auto& e : GameEngine::Instance()->entityManager.getEntities())
        {
            if (Physics::CheckCollisionDown(m_player, e))
            {
                collidingDown = true;
                break;
            }
        }

        if (collidingDown)
        {
            std::cout << "grounded" << std::endl;
            m_player->getComponent<CInput>().canJump = true;
        }
        else
        {
            std::cout << "in air" << std::endl;
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
            m_player->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation("RUN"), true);
            (m_player->getComponent<CTransform>().scale.x > 0) ? m_player->getComponent<CTransform>().scale.x *= -1 : 1;
            m_player->getComponent<CState>().state = "RUNNING";
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
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
			e->getComponent<CTransform>().velocity.x = 0;
		}

        if (e->getComponent<CInput>().up)
        {
            e->getComponent<CTransform>().velocity.y = -10;
        }

        //std::cout << e->getComponent<CTransform>().velocity << std::endl;
	}
}



