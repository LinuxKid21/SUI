#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Image : public Widget {
    public:
        Image(Theme &theme, sf::Texture &texture);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        void setTexture(sf::Texture &texture);
        
        virtual void layoutChanged();
        std::string getThemeObject();
    private:
        sf::Sprite sprite;
    };
}