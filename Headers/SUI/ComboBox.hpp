#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class ComboBox : public Widget {
    public:
        ComboBox();
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void onUpdate(const bool posChanged, const bool sizeChanged) override;
        virtual void onInput(sf::Event e) override;
    private:
        sf::RectangleShape mRectangleShape;
        std::vector<sf::Text> mTexts;
        std::vector<sf::String> mStrings;
        unsigned int mCurrentText;
        unsigned int mItemCount;
        sf::ConvexShape mLeftArrow;
        sf::ConvexShape mRightArrow;
        
        void placeArrows();
        void updateTexts();
        void updateTextPositions();
        
        ORIGIN getTextOriginX() {
            if(getProperty("textAlignX")) return getProperty("textAlignX").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        ORIGIN getTextOriginY() {
            if(getProperty("textAlignY")) return getProperty("textAlignY").as<ORIGIN>();
            return ORIGIN_MIDDLE;
        }
        float getTextPadding() {
            if(getProperty("textPadding")) return getProperty("textPadding").as<float>();
            return 5;
        }
        
        // take the arrows into account
        float getTotalTextPadding() {
            return getTextPadding()+arrow_size;
        }
        const float arrow_size = 20;
    };
}
