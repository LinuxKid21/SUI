#include <SUI/Image.hpp>


namespace sui {
    Image::Image() : Widget() {
        ;
    }
    void Image::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(sprite, states);
    }
    void Image::onInput(sf::Event e) {
        return;
    }
    
    void Image::onUpdate() {
        sprite = sf::Sprite(*getProperty("texture").as<sf::Texture *>());
        sprite.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        sprite.setScale(sf::Vector2f(getSize().x/sprite.getTexture()->getSize().x, getSize().y/sprite.getTexture()->getSize().y));
    }
}