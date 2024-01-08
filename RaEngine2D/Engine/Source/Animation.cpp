#include "Animation.h"
#include "Components.h"
#include "GameEngine.h"
#include <cmath>

Animation::Animation() {}

Animation::Animation(std::string&name, const sf::Texture &t)
    : Animation(name, t, 1, 0) {}

Animation::Animation(std::string&name, const sf::Texture &t,
                     size_t frameCount, size_t speed)
    : m_name(name), m_sprite(t), m_frameCount(frameCount), m_speed(speed) {
  m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
  m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
  m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0,
                                      m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update() {
  m_currentFrame++;
  int animationFrame = (int)(m_currentFrame / m_speed) % (int)m_frameCount;
  this->getSprite().setTextureRect(
      sf::IntRect(animationFrame * m_size.x, 0, m_size.x, m_size.y));
}

bool Animation::hasEnded() const {
  if ((int)(m_currentFrame / m_speed) % (int)m_frameCount == m_frameCount - 1)
    return true;
  else
    return false;
}

std::string Animation::getName() const { return m_name; }

const Vec2 &Animation::getSize() const { return m_size; }

sf::Sprite &Animation::getSprite() { return m_sprite; }

void Animation::playAnimationAtPosition(std::shared_ptr<Entity> entity, std::string animName, Vec2 pos)
{
    //auto entity = GameEngine::Instance()->entityManager.addEntity(tag);
    entity->addComponents<CAnimation>(GameEngine::Instance()->getAssets().getAnimation(animName), true);
    entity->addComponents<CTransform>(GameEngine::gridToMidPixel((float)pos.x, (float)pos.y, entity));
    if(entity->tag() == "TILE")
        entity->addComponents<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
    else if(entity->tag() == "PLAYER")
        entity->addComponents<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize() / 1.25f);
}

void Animation::renderAnimations()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();
        if (e->hasComponent<CAnimation>()) {
            auto& animation = e->getComponent<CAnimation>().animation;
            animation.getSprite().setRotation(transform.angle);
            animation.getSprite().setPosition(transform.getPos().x, transform.getPos().y);
            animation.getSprite().setScale(transform.scale.x, transform.scale.y);
            animation.getSprite().setColor(sf::Color(animation.getSprite().getColor().r, animation.getSprite().getColor().g, animation.getSprite().getColor().b, 255));
            GameEngine::Instance()->window().draw(animation.getSprite());
            animation.update();
        }
    }
}
