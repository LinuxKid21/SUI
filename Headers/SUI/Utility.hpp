#pragma once
#include <SFML/Graphics.hpp>
#include "Widget.hpp"
#include <iostream>
namespace sui {
    // returns a font size that will fit the letter 'base' in 'height'
    unsigned int fitTextVertically(const sf::Font &font, const float height, const char base);
    
    // sets the origin for text at x and y. The y is based off of a base instead
    // of the largest letter because I see that as far more consistent
    void setTextOrigin(sf::Text &text, ORIGIN x, ORIGIN y, char base);
    
    // wraps 'str' so it fits inside width and returns the string with the appropriate line breaks
    // currently does this anywhere instead of just whitespaces
    std::string wrapText(const sf::Font &font, const int font_size, const float width, const std::string str);
}