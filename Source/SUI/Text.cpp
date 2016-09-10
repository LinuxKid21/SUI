#include <SUI/Text.hpp>
#include <SUI/Utility.hpp>


namespace sui {
    Text::Text() : Widget() {
        mText = sf::Text();
        mText.setString("test");
    }
    void Text::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mText, states);
    }
    void Text::onInput(sf::Event e) {
        return; // no input handling here
    }
    
    void Text::onUpdate() {
        mText.setFont(*getProperty("font").as<sf::Font *>());
        mText.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        mText.setColor(getProperty("textColor").as<sf::Color>());
        insureTextFits();
    }
    
    void Text::insureTextFits() {
        sf::String str = getProperty("text").as<sf::String>();
        
        // assume it fits until it doesn't :)
        mText.setString(str);
        
        if(getSize().x == 0) {
            return;
        }
        unsigned int font_size = fitTextVertically(*mText.getFont(), getSize().y, 'J');
        mText.setCharacterSize(font_size);
        setTextOrigin(mText, ORIGIN_START, ORIGIN_START, 'J');

        
        // calculate how much text fits.
        float total_space = 0;
        for(unsigned int i = 0;i < str.getSize(); i++) {
            auto &glyph = mText.getFont()->getGlyph(str[i], font_size, false);
            total_space += glyph.advance;
            if(total_space > getSize().x) {
                total_space += mText.getFont()->getGlyph('.', font_size, false).advance*3; // make room for an ellipse
                int j = i;
                for(; j >= 0; j--) {
                    total_space -= mText.getFont()->getGlyph(str[j], font_size, false).advance;
                    if(total_space <= getSize().x) {
                        break;
                    }
                }
                // it totally cannot fit at all
                if(j < 0) {
                    mText.setString("");
                    return;
                }
                sf::String new_str;
                for(unsigned int k = 0;k < j; k++) {
                    new_str += str[k];
                }
                new_str += '.';
                new_str += '.';
                new_str += '.';
                mText.setString(new_str);
                return;
            }
        }
    }
}
