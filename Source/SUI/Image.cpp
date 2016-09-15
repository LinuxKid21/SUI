#include <SUI/Image.hpp>


namespace sui {
    Image::Image() : Widget() {
        sprite = sf::Sprite();
    }
    void Image::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(sprite, states);
    }
    
    void Image::calculateSpriteSize() {
        sprite.setScale(sf::Vector2f(getSize().x/sprite.getTexture()->getSize().x, getSize().y/sprite.getTexture()->getSize().y));
    }
    void Image::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
        const bool text_changed = hasPropChanged("texture");
        if(text_changed) {
            sprite.setTexture(*getProperty("texture").as<sf::Texture *>());
        }
        if(posChanged) {
            sprite.setPosition(getGlobalTopLeftCorner());
        }
        if((text_changed || sizeChanged) && getProperty("texture")) {
            calculateSpriteSize();
        }
    }
}