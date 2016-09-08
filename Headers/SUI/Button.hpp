#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>


namespace sui {
    class Button : public Widget {
    public:
        Button(Theme &theme);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        void setText(sf::String str);

        void setOnEntered(std::function<void()> func);
        void setOnExited(std::function<void()> func);
        void setOnClickedDown(std::function<void()> func);
        void setOnClickedUp(std::function<void()> func);

        // if theme was changed then call this function to update it!
        void updateTheme();
        
        void setTextAlign(ORIGIN textOriginX, ORIGIN textOriginY);
        
        virtual void layoutChanged();
        virtual std::string getThemeObjectType();
    private:
        sf::RectangleShape mRectangleShape;
        sf::Text mText;
        ORIGIN mTextOriginX;
        ORIGIN mTextOriginY;
        
        bool mHovered;
        bool mClicked;
        std::function<void()> mOnEntered;
        std::function<void()> mOnExited;
        std::function<void()> mOnClickedDown;
        std::function<void()> mOnClickedUp;
        
        void updateFillColors();
        void updateText();
    };
}