#include <SUI/Text.hpp>
#include <SUI/Utility.hpp>


namespace sui {
    Text::Text() : Widget() {
        mText = sf::Text();
    }
    void Text::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mText, states);
    }
    void Text::onUpdate(const bool posChanged, const bool sizeChanged) {
        Widget::onUpdate(posChanged, sizeChanged);
        const bool font_changed = hasPropChanged("font");
        const bool text_changed = hasPropChanged("text");
        const bool color_changed = hasPropChanged("textColor");
        if(font_changed) {
            mText.setFont(*getProperty("font").as<sf::Font *>());
        }
        if(color_changed) {
            mText.setColor(getProperty("textColor").as<sf::Color>());
        }
        if(posChanged) {
            mText.setPosition(getGlobalTopLeftCorner());
        }
        if(sizeChanged || font_changed || text_changed) {
            mText.setString(getText());
            insureTextFitsVertically();
            insureTextFitsHorizontally();
        }
    }
    
    void Text::insureTextFitsVertically() {
        if(!getProperty("font")) return;
        
        unsigned int font_size = fitTextVertically(*mText.getFont(), getSize().y, 'J');
        mText.setCharacterSize(font_size);
        setTextOrigin(mText, ORIGIN_START, ORIGIN_START, 'J');
    }
    void Text::insureTextFitsHorizontally() {
        if(!getProperty("font")) return;
        sf::String str = getText();
        
        if(getSize().x == 0) {
            return;
        }
        const unsigned int font_size = mText.getCharacterSize();
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
