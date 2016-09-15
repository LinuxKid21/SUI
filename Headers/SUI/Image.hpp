#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Image : public Widget {
    public:
        Image();
        
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
    private:
        void calculateSpriteSize();
        sf::Sprite sprite;
    };
}