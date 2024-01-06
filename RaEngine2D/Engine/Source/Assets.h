#pragma once

#include "Animation.h"
#include "Common.h"

// the numbers are the ascii values of the first letter of the types
enum eAssetsTypes { TEXTURE = 84,
                    ANIMATION = 65,
                    FONT = 70 };

class Assets {
  std::map<std::string, sf::Texture> m_textureMap;
  std::map<std::string, Animation> m_animationMap;
  std::map<std::string, sf::Font> m_fontMap;
  //  std::vector<std::string>                          m_animation_typess;

  void addTexture(std::string&texture_types, const std::string &path,
                  bool smooth = true);
  void addAnimation(std::string&animation_types,
      std::string&texture_types, size_t frameCount,
                    size_t speed);
  void addFont(std::string&fontName, const std::string &path);

  public:
  Assets();

  void loadFromFile(const std::string &path);

  const sf::Texture &getTexture(std::string texture_types) const;
  const Animation &getAnimation(std::string animation_types) const;
  //  const std::vector<std::string> & getanimation_typess() const;
  const sf::Font &getFont(std::string fontName) const;
};
