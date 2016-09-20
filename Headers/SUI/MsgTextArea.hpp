#pragma once
#include "Widget.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

/*
    This uses the msg format which is simply unicode text where
    && prints a single '&' character
    &T creates a tab which is not converted to a space
    &- creates a space
    &N creates a new line which is not converted to a space or 
    &L signifies a link until cleared *
    &B signifies bold until cleared
    &I signifies italic until cleared
    &U signifies underlined until cleared
    &S signifies striked until cleared
    &0 Use property "textColor0" 0 is the default color
    &n Use property "textColorn" where n is a digit from 0 to 9
    &9 Use property "textColor9"
    &* signifies clear all
    
    * a link will read a .msg file with the same name of whatever is highlighted
      Automatically underlines and italicizes and sets color to a blue which
      gets slightly brighter on hovering. Sets this MsgTextArea to whatever
      was clicked if clicked
    
    all tabs and newlines are converted into spaces and no double spaces exist
    
    Note: There must be a space before the '&' character for things to work well
    subtle spaces may be missing otherwise!
*/
namespace sui {
    class MsgTextArea : public Widget {
        struct Symbol {
            int newLines;
            int tabs;
            int spaces; //spaces of less than zero means mid-word new symbol. Don't add a space!
            int linkId;
            sf::Color color;
            std::string colorKey;
            sf::Text::Style style;
            sf::String value;
        };
    public:
        MsgTextArea();
    private:
        sf::RectangleShape mRectangleShape;
        sf::Clock mTypingTimer;
        unsigned int mTypedCount;
        unsigned int mCurrentText;
        float mTimeElapsed;
        float mInterval;
        
        std::vector<sf::Text> mTexts;
        std::vector<Symbol> mSymbols;
        sf::String mStr;
        
        
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void onUpdate(const bool posChanged, const bool sizeChanged) override;
        virtual void onInput(sf::Event e) override;
        
        void updateSymbols();
        void updateText();
        void loadFile();
        
        float getTextPadding() {
            if(getProperty("textPadding")) return getProperty("textPadding").as<float>();
            return 5;
        }
        float getFontSize() {
            if(getProperty("fontSize")) return getProperty("fontSize").as<float>();
            return 10;
        }
        sf::Color getLinkColor() {
            if(getProperty("linkColor")) return getProperty("linkColor").as<sf::Color>();
            return sf::Color(0,0,255,255);
        }
        sf::Color getLinkColorHover() {
            if(getProperty("linkColorHover")) return getProperty("linkColorHover").as<sf::Color>();
            return sf::Color(100,100,255,255);
        }
    };
}