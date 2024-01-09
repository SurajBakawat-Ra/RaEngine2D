#include "Render.h"
#include "GameEngine.h"


void Render::update()
{
    GameEngine::Instance()->window().clear(sf::Color(50, 50, 150));

    //Render all animations
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();
        if (e->hasComponent<CAnimation>())
        {
            auto& animation = e->getComponent<CAnimation>().animation;
            animation.getSprite().setRotation(transform.angle);
            animation.getSprite().setPosition(transform.getPos().x, transform.getPos().y);
            animation.getSprite().setScale(transform.scale.x, transform.scale.y);
            animation.getSprite().setColor(sf::Color(animation.getSprite().getColor().r, animation.getSprite().getColor().g, animation.getSprite().getColor().b, 255));
            GameEngine::Instance()->window().draw(animation.getSprite());
            animation.update();
        }
    }

    //Render bounding boxes
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
}
