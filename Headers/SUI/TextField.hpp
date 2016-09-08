#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

namespace sui {
    class TextField : public Widget {
    public:
        TextField(Theme &theme);
        void setText(sf::String str);
        sf::String getText() const;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool handleInput(sf::Event e);
        
        // if theme was changed then call this function to update it!
        void updateTheme();
        
        void setOnEntered(std::function<void()> func);
        void setOnExited(std::function<void()> func);
        void setOnActivated(std::function<void()> func);
        void setOnDeactivated(std::function<void()> func);
        void setOnChanged(std::function<void()> func);
        
        virtual void layoutChanged();
        virtual std::string getThemeObjectType();
    private:
        sf::RectangleShape mRectangleShape;
        sf::Text mText;
        sf::String mStr;
        
        sf::RectangleShape mCursorShape;
        sf::RectangleShape mHighlightShape;
        unsigned int mCursorPositionStart;
        unsigned int mCursorPositionEnd;
        
        // hack to make these work within the SFML draw cycle which is const
        // unfortunately I don't see a better place to put the blinking cursor
        // then in the draw cycle without adding a dedicated update function
        // which I might just do, but I would like to avoid that
        mutable bool mCursorVisible;
        mutable sf::Clock mCursorTimer;

        void resetCursorBlink();
        void moveCursorLeft(bool shift);
        void moveCursorRight(bool shift);
        
        std::function<void()> mOnEntered;
        std::function<void()> mOnExited;
        std::function<void()> mOnActivated;
        std::function<void()> mOnDeactivated;
        std::function<void()> mOnChanged;
        
        // x_diff is how far away the cursor is in the x direction from start of the textbox
        int findNearestCursorPoint(float x_diff);
        
        bool mActive;
        bool mHovered;
        bool mSelecting;
        void updateText();
        void updateCursor();
        
        void deleteInsideCursor();
        
        // how far from the left the text is
        static const int text_padding = 5;
    };
}
