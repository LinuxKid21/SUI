#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Image : public Widget {
    public:
        Image();
        
    protected:
        virtual void onUpdate();
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onInput(sf::Event e);
    private:
        sf::Sprite sprite;
    };
}