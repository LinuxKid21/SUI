#include <SUI/Image.hpp>


namespace sui {
    Image::Image(Theme &theme, sf::Texture &texture) : Widget(theme) {
        setTexture(texture);
    }
    void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(sprite, states);
    }
    bool Image::handleInput(sf::Event e) {
        return false;
    }
    
    void Image::setTexture(sf::Texture &texture) {
        sprite = sf::Sprite(texture);
    }
    
    void Image::layoutChanged() {
        sprite.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        sprite.setScale(sf::Vector2f(getSize().x/sprite.getTexture()->getSize().x, getSize().y/sprite.getTexture()->getSize().y));
    }
    std::string Image::getThemeObject() {
        return "sui::Image";
    }
}