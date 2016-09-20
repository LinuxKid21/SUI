#include <SUI/Utility.hpp>

namespace sui {
    unsigned int fitTextVertically(const sf::Font &font, const float height, const char base) {
		unsigned int fonts_size = height;
        
        auto shrink = [&](){
            float letter_height = font.getGlyph(base, fonts_size, false).bounds.height;
            return letter_height > height;
        };
        
        auto grow = [&](){
            float letter_height = font.getGlyph(base, fonts_size, false).bounds.height;
            return letter_height < height;
        };
        
        while(grow()){fonts_size++;}
        while(shrink()){fonts_size--;}
        
        return fonts_size;
    }


    void setTextOrigin(sf::Text &text, ORIGIN x, ORIGIN y, char base) {
		sf::FloatRect textRect = text.getLocalBounds();

		sf::FloatRect letter_bound = text.getFont()->getGlyph(base, text.getCharacterSize(), false).bounds;
        
        float origin_x;
		if(x == ORIGIN_MIDDLE) {
            origin_x = textRect.left + textRect.width/2.f;
		} else if(x == ORIGIN_START) {
            origin_x = textRect.left;
		} else if(x == ORIGIN_END){
            origin_x = textRect.left + textRect.width;
		} else {
            throw "bad x origin!";
        }
        
        float origin_y;
		if(y == ORIGIN_MIDDLE) {
            origin_y = letter_bound.top+text.getCharacterSize()+letter_bound.height/2.f;
		} else if(y == ORIGIN_START) {
            origin_y = letter_bound.top+text.getCharacterSize();
		} else if(y == ORIGIN_END){
            origin_y = letter_bound.top+text.getCharacterSize()+letter_bound.height;
		} else {
            throw "bad y origin!";
        }
        text.setOrigin(origin_x,origin_y);
    }
    
    sf::String wrapText(const sf::Font &font, const int font_size, const float width, const sf::String str) {
        sf::String string = str;
        sf::Uint32 prevChar = 0;
        float text_width = 0;
        for(int i = 0;i < string.getSize(); i++) {
            sf::Uint32 curChar = string[i];
            if(curChar == '\n') {
                text_width = 0;
                continue;
            }
    		const float letter_advance = font.getGlyph(curChar, font_size, false).advance;
            const float kerning = font.getKerning(prevChar, curChar, font_size);
            text_width += letter_advance + kerning;
            prevChar = curChar;
            
            if(text_width > width) {
                i--;
                string.insert(i, "\n");
                text_width = 0;
            }
        }
        return string;
    }
}
