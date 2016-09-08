#include <SUI/Text.hpp>
#include <SUI/Utility.hpp>


namespace sui {
    Text::Text(Theme &theme) : Widget(theme) {
        mText = sf::Text();
        mStr = "";
        mText.setString(mStr);
        updateTheme();
    }
    void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mText, states);
    }
    bool Text::handleInput(sf::Event e) {
        return false; // no input handling here
    }
    void Text::setText(const sf::String str) {
        mStr = str;
        insureTextFits();
    }
    void Text::updateTheme() {
        mText.setFont(*getFontProperty("font"));
        layoutChanged();
    }
    
    void Text::layoutChanged() {
        mText.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        insureTextFits();
    }
    std::string Text::getThemeObject() {
        return "sui::Text";
    }
    void Text::insureTextFits() {
        if(getSize().x == 0) {
            return;
        }
        unsigned int font_size = fitTextVertically(*mText.getFont(), getSize().y, 'J');
        mText.setCharacterSize(font_size);
        setTextOrigin(mText, ORIGIN_START, ORIGIN_START, 'J');

        // assume it fits until it doesn't :)
        mText.setString(mStr);
        
        // calculate how much text fits.
        float total_space = 0;
        for(unsigned int i = 0;i < mStr.getSize(); i++) {
            auto &glyph = mText.getFont()->getGlyph(mStr[i], font_size, false);
            total_space += glyph.advance;
            if(total_space > getSize().x) {
                total_space += mText.getFont()->getGlyph('.', font_size, false).advance*3; // make room for an ellipse
                int j = i;
                for(; j >= 0; j--) {
                    total_space -= mText.getFont()->getGlyph(mStr[j], font_size, false).advance;
                    if(total_space <= getSize().x) {
                        break;
                    }
                }
                // it totally cannot fit at all
                if(j < 0) {
                    mText.setString("");
                    return;
                }
                sf::String str;
                for(unsigned int k = 0;k < j; k++) {
                    str += mStr[k];
                }
                str += '.';
                str += '.';
                str += '.';
                mText.setString(str);
                return;
            }
        }
    }
}
