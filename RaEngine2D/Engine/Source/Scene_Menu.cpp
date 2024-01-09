#include "Scene_Menu.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "Scene_Play.h"

Scene_Menu::Scene_Menu() : Scene() { init(); }

void Scene_Menu::init()
{
    GameEngine::Instance()->window().setKeyRepeatEnabled(false);
    m_menuText.setFont(GameEngine::Instance()->getAssets().getFont("PIXEL"));
    m_title = "Not Mario";
    m_menuStrings.emplace_back("Play 1");
    m_menuStrings.emplace_back("Play 2");
    m_menuStrings.emplace_back("Play 3");
}

void Scene_Menu::update()
{
    sRender();
    sUserInput();
}

void Scene_Menu::sUserInput()
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
                GameEngine::Instance()->changeScene("PLAY", std::make_shared<Scene_Play>());
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

void Scene_Menu::sRender()
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

void Scene_Menu::onEnd() {}

void Scene_Menu::sDoAction(const InputAction& action) {}
