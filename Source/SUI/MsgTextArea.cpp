#include <SUI/MsgTextArea.hpp>
#include <SUI/Utility.hpp>
#include <cctype> //isspace
#include <fstream>


namespace sui {
    MsgTextArea::MsgTextArea() {
        mRectangleShape = sf::RectangleShape();
        mTypingTimer = sf::Clock();
        mTypedCount = 0;
        mCurrentText = 0;
        mTimeElapsed = 0;
        mInterval = .02;
    }
    void MsgTextArea::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        for(auto &t : mTexts) {
            target.draw(t, states);
        }
        // target.draw(mText, states);
    }
    void MsgTextArea::onUpdate(const bool posChanged, const bool sizeChanged) {
        mTimeElapsed += mTypingTimer.getElapsedTime().asSeconds();
        mTypingTimer.restart();
        while(mTimeElapsed > mInterval) {
            mTimeElapsed -= mInterval;
            if(mCurrentText < mSymbols.size()) {
                mTexts[mCurrentText].setString(mSymbols[mCurrentText].value.substring(0, mTypedCount));
                mTypedCount++;
                if(mTypedCount > mSymbols[mCurrentText].value.getSize()) {
                    mTypedCount = 0;
                    mCurrentText++;
                }
            }
        }
        bool shouldUpdateText = false;
        if(sizeChanged) {
            mRectangleShape.setSize(getSize());
        }
        if(posChanged) {
            mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        }
        
        if(hasPropChanged("outlineColor")) {
            mRectangleShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        }
        if(hasPropChanged("outlineThickness")) {
            mRectangleShape.setOutlineThickness(-getProperty("outlineThickness").as<float>());
        }
        if(hasPropChanged("interval")) {
            mInterval = getProperty("interval").as<float>();
        }
        if(shouldUpdateText || sizeChanged || posChanged || hasPropChanged("text") || hasPropChanged("font") || hasPropChanged("file")) {
            updateText();
        }
    }
    
    void MsgTextArea::onInput(sf::Event e) {
        Widget::onInput(e);
        
        const bool mouse_moved   = e.type == sf::Event::MouseMoved;
        const bool mouse_clicked = e.type == sf::Event::MouseButtonPressed;
        if(!mouse_moved && !mouse_clicked) return;
        
        sf::Vector2f mouse_pos;
        if(mouse_moved)   mouse_pos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
        if(mouse_clicked) mouse_pos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
        
        
        
        // size of mSymbols and mTexts is the same
        const unsigned int size = mSymbols.size();
        
        
        for(unsigned int i = 0;i < size; i++) {
            if(mSymbols[i].linkId != -1) {
                mTexts[i].setColor(getLinkColor());
            }
        }
        
        auto setLinkIdHovered = [&size, this](int id){
            bool entered = false;
            for(unsigned int i = 0;i < size; i++) {
                if(mSymbols[i].linkId == id) {
                    mTexts[i].setColor(getLinkColorHover());
                    entered = true;
                } else if(entered) {
                    break;
                }
            }
        };
        
        auto setLinkIdClicked = [&size, this](int id){
            bool entered = false;
            sf::String updateText = "";
            for(unsigned int i = 0;i < size; i++) {
                if(mSymbols[i].linkId == id) {
                    if(entered) updateText += " ";
                    updateText += mSymbols[i].value;
                    entered = true;
                } else if(entered) {
                    break;
                }
            }
            setProperty("file", updateText + sf::String(".msg"));
        };
        
        for(unsigned int i = 0;i < size; i++) {
            if(mSymbols[i].linkId != -1 && mTexts[i].getGlobalBounds().contains(mouse_pos)) {
                if(mouse_moved) {
                    setLinkIdHovered(mSymbols[i].linkId);
                }
                if(mouse_clicked) {
                    setLinkIdClicked(mSymbols[i].linkId);
                }
                break;
            }
        }
    }
    
    void MsgTextArea::loadFile() {
        std::ifstream file;
        mStr = "";
        file.open(getProperty("file").as<sf::String>().toAnsiString().c_str());
        std::string output;
        if (file.is_open()) {
            while (std::getline (file,output)) {
                mStr += output;
                mStr += '\n';
            }
        }
    }
    
    void MsgTextArea::updateSymbols() {
        loadFile();
        sf::String str = mStr;
        // sf::String str = getProperty("file").as<sf::String>();
        
        mSymbols.clear();
        
        Symbol currentSymbol;
        currentSymbol.newLines = 0;
        currentSymbol.tabs = 0;
        currentSymbol.spaces = 0;
        currentSymbol.linkId = -1;
        currentSymbol.colorKey = "textColor";
        currentSymbol.style = sf::Text::Style::Regular;
        currentSymbol.value = "";
        
        auto make_symbol = [&currentSymbol, this](bool last_space){
            if(currentSymbol.value.getSize() != 0) {
                Symbol sym = currentSymbol;
                
                // account for the edge case where style is changed mid-word which
                // requires a new symbol. It should not be seperated with a space!
                if(!last_space && sym.newLines == 0 && sym.tabs == 0 && sym.spaces == 0) {
                    sym.spaces = -1; // signify not to add a space
                }
                mSymbols.push_back(std::move(sym));
                currentSymbol.newLines = 0;
                currentSymbol.tabs = 0;
                currentSymbol.spaces = 0;
                currentSymbol.value = "";
            }
        };
        
        
        bool special = false;
        bool last_space = false;
        int current_link_id = 0;
        for(auto c : str) {
            // handle all uses of the '&' character
            if(c == '&') {
                if(special) {
                    currentSymbol.value += '&';
                    special = false;
                    continue;
                }
                special = true;
                continue;
            }
            if(special) {
                make_symbol(last_space);
                
                if(c == 'T') {
                    currentSymbol.tabs++;
                } else if(c == 'N') {
                    currentSymbol.newLines++;
                } else if(c == '-') {
                    currentSymbol.spaces++;
                } else if(c == 'L') {
                    currentSymbol.linkId = current_link_id;
                } else if(c == 'B') {
                    currentSymbol.style = static_cast<sf::Text::Style>(currentSymbol.style | sf::Text::Style::Bold);
                } else if(c == 'I') {
                    currentSymbol.style = static_cast<sf::Text::Style>(currentSymbol.style | sf::Text::Style::Italic);
                } else if(c == 'U') {
                    currentSymbol.style = static_cast<sf::Text::Style>(currentSymbol.style | sf::Text::Style::Underlined);
                } else if(c == 'S') {
                    currentSymbol.style = static_cast<sf::Text::Style>(currentSymbol.style | sf::Text::Style::StrikeThrough);
                } else if(c == '0') {
                    currentSymbol.colorKey = "color0";
                } else if(c == '1') {
                    currentSymbol.colorKey = "color1";
                } else if(c == '2') {
                    currentSymbol.colorKey = "color2";
                } else if(c == '3') {
                    currentSymbol.colorKey = "color3";
                } else if(c == '4') {
                    currentSymbol.colorKey = "color4";
                } else if(c == '5') {
                    currentSymbol.colorKey = "color5";
                } else if(c == '6') {
                    currentSymbol.colorKey = "color6";
                } else if(c == '7') {
                    currentSymbol.colorKey = "color7";
                } else if(c == '8') {
                    currentSymbol.colorKey = "color8";
                } else if(c == '9') {
                    currentSymbol.colorKey = "color9";
                } else if(c == '*') {
                    currentSymbol.colorKey = "textColor";
                    currentSymbol.linkId = -1;
                    currentSymbol.style = sf::Text::Style::Regular;
                    current_link_id++;
                }
                special = false;
                continue;
            }
            if(last_space && !std::isspace(c)) {
                make_symbol(last_space);
            }
            if(!std::isspace(c)) {
                currentSymbol.value += c;
            }
            last_space = std::isspace(c);
        }
        make_symbol(last_space);
    }
    
    
    void MsgTextArea::updateText() {
        if(!getProperty("font")) return;
        if(!getProperty("file")) return;
        
        updateSymbols();
        
        mTexts.clear();
        const float left = getGlobalTopLeftCorner().x + getTextPadding();
        int x = left;
        int y = getGlobalTopLeftCorner().y + getTextPadding();
        
        float textSize = getFontSize();
        sf::Font *font = getProperty("font").as<sf::Font *>();
        const float space_size = font->getGlyph(' ', textSize, false).advance;
        const float line_size = font->getLineSpacing(textSize);
        for(auto &s : mSymbols) {
            sf::Text text;
            text.setFont(*font);
            text.setString(s.value);
            text.setStyle(s.style);
            text.setColor(getProperty(s.colorKey).as<sf::Color>());
            text.setCharacterSize(textSize);
            setTextOrigin(text, ORIGIN_START, ORIGIN_START, 'X');
            if(s.linkId != -1) {
                text.setColor(getLinkColor());
                
                // add to style underlined and italic
                text.setStyle(static_cast<sf::Text::Style>(s.style | sf::Text::Style::Underlined | sf::Text::Style::Italic));
            }
            float width = text.getLocalBounds().width;
            for(int i = 0; i < s.newLines; i++) {
                y += line_size;
                x = left;
            }
            for(int i = 0; i < s.tabs; i++) {
                x += space_size*4;
            }
            for(int i = 0; i < s.spaces; i++) {
                x += space_size;
            }
            if(x + width > getGlobalTopLeftCorner().x + getSize().x - getTextPadding()) {
                x = left;
                y += line_size;
            }
            text.setPosition(x, y);
            x += width;
            if(s.spaces >= 0) //if less than zero than this new symbol is mid-word and a space would be innappropriate
                x += space_size;
                
            // done measuring
            // we make the type come like it's being typed so it has to be empty at first
            text.setString("");
                
            mTexts.push_back(std::move(text));
        }
        
        mTypedCount = 0;
        mCurrentText = 0;
    }
}