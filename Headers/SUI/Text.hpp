#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>

namespace sui {
    class Text : public Widget {
    public:
        Text(Theme &theme);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        void setText(const sf::String str);
        
        void updateTheme();
        
        virtual void layoutChanged();
        std::string getThemeObject();
    private:
        void insureTextFits();
        sf::Text mText;
        sf::String mStr;
    };
}