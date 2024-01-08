#include "Scene_Menu.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "Scene_Play.h"

Scene_Play::Scene_Play() : Scene() { init(""); }

void Scene_Play::init(const std::string& menuConfig)
{
    GameEngine::Instance()->window().setKeyRepeatEnabled(false);
    m_menuText.setFont(GameEngine::Instance()->getAssets().getFont("PIXEL"));
    m_title = "Not Mario";
    m_menuStrings.emplace_back("Menu 1");
    m_menuStrings.emplace_back("Menu 2");
    m_menuStrings.emplace_back("Menu 3");

    auto brick = GameEngine::Instance()->entityManager.addEntity("TILE");
    auto brick1 = GameEngine::Instance()->entityManager.addEntity("TILE");
    auto brick2 = GameEngine::Instance()->entityManager.addEntity("TILE");
    auto brick3 = GameEngine::Instance()->entityManager.addEntity("TILE");
    auto brick4 = GameEngine::Instance()->entityManager.addEntity("TILE");
    auto player = GameEngine::Instance()->entityManager.addEntity("PLAYER");
    Animation::playAnimationAtPosition(brick, "AniBRICK", Vec2(100, 600));
    Animation::playAnimationAtPosition(brick1, "AniBRICK", Vec2(200, 600));
    Animation::playAnimationAtPosition(brick2, "AniBRICK", Vec2(300, 600));
    Animation::playAnimationAtPosition(brick3, "AniBRICK", Vec2(400, 600));
    Animation::playAnimationAtPosition(brick4, "AniBRICK", Vec2(500, 600));
    Animation::playAnimationAtPosition(player, "RUN", Vec2(100, 200));
    player->addComponents<CRigidbody>(Rigidbody(), 0.1);
}

void Scene_Play::update()
{
    sRender();
    GameEngine::Instance()->entityManager.update();
    sUserInput();
}

void Scene_Play::sUserInput()
{
    sf::Event event{};
    while (GameEngine::Instance()->window().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            GameEngine::Instance()->quit();
        }
        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
            {
                GameEngine::Instance()->quit();
                break;
            }
            case sf::Keyboard::W:
            {
                if (m_selectedMenuIndex > 0)
                {
                    m_selectedMenuIndex--;
                }
                else
                {
                    m_selectedMenuIndex = m_menuStrings.size() - 1;
                }
                break;
            }
            case sf::Keyboard::S:
            {
                m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
                break;
            }
            case sf::Keyboard::D:
            {
                GameEngine::Instance()->changeScene("MENU", std::make_shared<Scene_Menu>());
                GameEngine::Instance()->removeAllEntities();
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
}

void Scene_Play::sRender()
{
    GameEngine::Instance()->window().clear(sf::Color(50, 50, 150));
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

    Animation::renderAnimations();

    if (true)
    {
        for (auto& e : GameEngine::Instance()->entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.size.x / 2, box.size.y / 2));
                rect.setPosition(transform.getPos().x, transform.getPos().y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                GameEngine::Instance()->window().draw(rect);
            }
        }
    }

    GameEngine::Instance()->window().display();
}

void Scene_Play::onEnd() {}

void Scene_Play::sDoAction(const InputAction& action) {}


