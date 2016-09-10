#include <SUI/TextField.hpp>
#include <SUI/Utility.hpp>

namespace sui {
    TextField::TextField() : Widget() {
        mCursorShape.setFillColor(sf::Color(0,0,0,255));
        mHighlightShape.setFillColor(sf::Color(0,0,255,128));
        mCursorPositionStart = 0;
        mCursorPositionEnd = 0;
        mCursorTimer = sf::Clock();
        mHovered = false;
        mActive = false;
        mSelecting = false;
        setProperty("text", sui::Property::make<sf::String>(""));
    }
    void TextField::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        target.draw(mText, states);
        if(mCursorTimer.getElapsedTime().asSeconds() > .5) {
            mCursorVisible = !mCursorVisible;
            mCursorTimer.restart();
        }
        if(mActive && mCursorPositionStart != mCursorPositionEnd) {
            target.draw(mHighlightShape, states);
        } else if(mActive && mCursorVisible) {
            target.draw(mCursorShape, states);
        }
    }
    void TextField::onInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            
            if(mSelecting) {
                float x_diff = mousePos.x-(getGlobalPosition().x+getTextPadding());
                mCursorPositionEnd = findNearestCursorPoint(x_diff);
                updateCursor();
            }
            
            if(getGlobalBounds().contains(mousePos)) {
                // call only while entering, not constantly while inside
                if(!mHovered) {
                    onEntered();
                }

                mHovered = true;
            } else if(mHovered) {
                    onExited();
                mHovered = false;
            }
            
        } else if(e.type == sf::Event::MouseButtonPressed) {
            if(e.mouseButton.button == sf::Mouse::Button::Left) {
                mActive = getGlobalBounds().contains(sf::Vector2f(e.mouseButton.x, e.mouseButton.y));
                if(mActive) {
                    // how for the cursor is from the left of the TextField
                    float x_diff = e.mouseButton.x-(getGlobalPosition().x+getTextPadding());
                    
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                       mCursorPositionEnd = findNearestCursorPoint(x_diff);
                    } else {
                        mCursorPositionStart = findNearestCursorPoint(x_diff);
                        mCursorPositionEnd = mCursorPositionStart;
                    }
                    updateCursor();
                    
                    onActivated();
                    mSelecting = true;
                }
                
                if(!mActive) {
                    onDeactivated();
                }
            }
        } else if(e.type == sf::Event::MouseButtonReleased) {
            if(e.mouseButton.button == sf::Mouse::Button::Left)
                mSelecting = false;
        } else if(e.type == sf::Event::KeyPressed && mActive) {
            if(e.key.code == sf::Keyboard::Key::Left) {
                moveCursorLeft(e.key.shift);
            } else if(e.key.code == sf::Keyboard::Key::Right) {
                moveCursorRight(e.key.shift);
            } else if(e.key.code == sf::Keyboard::Key::Delete) {
                if(mCursorPositionStart == mCursorPositionEnd) {
                    sf::String &str = getText();
                    if(mCursorPositionStart < str.getSize()) {
                        str.erase(mCursorPositionStart);
                        updateText();
                    }
                } else {
                    deleteInsideCursor();
                }
                onChanged();
                resetCursorBlink();
            } else if(e.key.code == sf::Keyboard::Key::BackSpace) {
                if(mCursorPositionStart == mCursorPositionEnd) {
                    if(mCursorPositionStart > 0) {
                        getText().erase(mCursorPositionStart-1);
                        moveCursorLeft(false);
                        updateText();
                    }
                } else {
                    deleteInsideCursor();
                }
                onChanged();
                resetCursorBlink();
            } else if(e.key.code == sf::Keyboard::Key::Return) {
                mActive = false;
                onDeactivated();
            }
        } else if(e.type == sf::Event::TextEntered && mActive) {
            // backspace = 8, enter = 13, tab = 9, escape = 27, delete = 127
            for(int key : {8,13,9,27,127}) {
                if(e.text.unicode == key) return;
            }
            
            if(mCursorPositionStart != mCursorPositionEnd) {
              deleteInsideCursor();
            }
            if(mText.getLocalBounds().left +
                      mText.getLocalBounds().width +
                      mText.getFont()->getGlyph(e.text.unicode,mText.getCharacterSize(), false).advance < getGlobalBounds().width-getTextPadding()) {
                getText().insert(mCursorPositionStart, e.text.unicode);
                moveCursorRight(false);
            }
            onChanged();
            updateText();
        }
    }
    
    void TextField::onPropertyChanged(const std::string key) {
        if(key == "outlineColor") {
            mRectangleShape.setOutlineColor(getProperty(key).as<sf::Color>());
        } else if(key == "outlineThickness") {
            mRectangleShape.setOutlineThickness(-getProperty(key).as<float>());
        } else if(key == "font") {
            mText.setFont(*(getProperty(key).as<std::shared_ptr<sf::Font> >()));
            updateText();
            updateCursor();
        } else if(key == "textColor") {
            mText.setColor(getProperty(key).as<sf::Color>());
        } else if(key == "fillColor") {
            mRectangleShape.setFillColor(getProperty(key).as<sf::Color>());
        }
    }
    void TextField::onPositionChanged() {
        mRectangleShape.setPosition(sf::Vector2f(getGlobalPosition().x, getGlobalPosition().y));
        updateText();
        updateCursor();
    }
    void TextField::onSizeChanged() {
        mRectangleShape.setSize(getSize());
        mCursorShape.setSize(sf::Vector2f(1, getSize().y-8));
        mCursorShape.setOrigin(sf::Vector2f(0,-4));
        
        updateText();
        updateCursor();
    }
    
    void TextField::resetCursorBlink() {
        mCursorVisible = true;
        mCursorTimer.restart();
    }
    void TextField::moveCursorLeft(bool shift) {
        resetCursorBlink();
        if(!shift) {
            // if we are currently selecting a range then snap
            // to the left of that range
            if(mCursorPositionStart != mCursorPositionEnd) {
                if(mCursorPositionStart < mCursorPositionEnd) {
                    mCursorPositionEnd = mCursorPositionStart;
                } else {
                    mCursorPositionStart = mCursorPositionEnd;
                }
                updateCursor();
                return;
            }
            if(mCursorPositionStart != 0) mCursorPositionStart--;
        }
        if(mCursorPositionEnd != 0) mCursorPositionEnd--;
        updateCursor();
    }
    void TextField::moveCursorRight(bool shift) {
        resetCursorBlink();
        sf::String &str = getText();
        if(!shift) {
            // if we are currently selecting a range then snap
            // to the right of that range
            if(mCursorPositionStart != mCursorPositionEnd) {
                if(mCursorPositionStart > mCursorPositionEnd) {
                    mCursorPositionEnd = mCursorPositionStart;
                } else {
                    mCursorPositionStart = mCursorPositionEnd;
                }
                updateCursor();
                return;
            }
            mCursorPositionStart++;
            if(mCursorPositionStart > str.getSize()) mCursorPositionStart = str.getSize();
        }
        mCursorPositionEnd++;
        if(mCursorPositionEnd > str.getSize()) mCursorPositionEnd = str.getSize();
        updateCursor();
    }
    
    int TextField::findNearestCursorPoint(float x_diff) {
        sf::String &str = getText();
        for(unsigned int i = 0;i < str.getSize(); i++) {
            auto &glyph = mText.getFont()->getGlyph(str[i],mText.getCharacterSize(), false);
            x_diff -= glyph.advance;
            if(x_diff <= 0) {
                if(x_diff >= -glyph.advance/2) {
                    return i+1;
                }
                return i;
            }
        }
        return str.getSize();
    }
    
    void TextField::updateText() {
        mText.setString(getText());

        setTextOrigin(mText, ORIGIN_START, ORIGIN_MIDDLE, 't');
        auto r = getGlobalBounds();
        mText.setPosition(sf::Vector2f(r.left+5, r.top+r.height/2));
        mText.setCharacterSize(r.height*.7); //.7 is jut a magic number to make letters like 'g' fit in
    }
    
    void TextField::updateCursor() {
        sf::String &str = getText();
        float text_padding = getTextPadding();
        auto pos_1 = sf::Vector2f(getGlobalBounds().left+text_padding, getGlobalBounds().top);
        for(unsigned int i = 0;i < mCursorPositionStart; i++) {
            auto &glyph = mText.getFont()->getGlyph(str[i],mText.getCharacterSize(), false);
            pos_1.x += glyph.advance;
        }
        mCursorShape.setPosition(pos_1);
        
        if(mCursorPositionStart != mCursorPositionEnd) {
            auto pos_2 = sf::Vector2f(getGlobalBounds().left+text_padding, getGlobalBounds().top);
            for(unsigned int i = 0;i < mCursorPositionEnd; i++) {
                auto &glyph = mText.getFont()->getGlyph(str[i],mText.getCharacterSize(), false);
                pos_2.x += glyph.advance;
            }
            mHighlightShape.setPosition(pos_1);
            mHighlightShape.setSize(sf::Vector2f(pos_2.x-pos_1.x, getGlobalBounds().height));
        }
    }
    void TextField::deleteInsideCursor() {
        int start = mCursorPositionStart;
        int end = mCursorPositionEnd;
        if(start > end) {
            end = mCursorPositionStart;
            start = mCursorPositionEnd;
        }
        getText().erase(start, end-start);
        mCursorPositionStart = start;
        mCursorPositionEnd = mCursorPositionStart;
        updateText();
        updateCursor();
    }
}

