#include <SUI/TextField.hpp>
#include <SUI/Utility.hpp>

namespace sui {
    TextField::TextField(Theme &theme) : Widget(theme) {
        mCursorShape.setFillColor(sf::Color(0,0,0,255));
        mHighlightShape.setFillColor(sf::Color(0,0,255,128));
        mCursorPositionStart = 0;
        mCursorPositionEnd = 0;
        mCursorTimer = sf::Clock();
        mHovered = false;
        mActive = false;
        mSelecting = false;
        updateTheme();
    }
    void TextField::setText(sf::String str) {
        mStr = str;
        mCursorPositionStart = 0;
        mCursorPositionEnd = 0;
        if(mOnChanged) {
            mOnChanged();
        }
    }
    sf::String TextField::getText() const {
        return mStr;
    }
    void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
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
    bool TextField::handleInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            
            if(mSelecting) {
                float x_diff = mousePos.x-(getGlobalPosition().x+text_padding);
                mCursorPositionEnd = findNearestCursorPoint(x_diff);
                updateCursor();
            }
            
            if(getGlobalBounds().contains(mousePos)) {
                // call only while entering, not constantly while inside
                if(mOnEntered && !mHovered) {
                    mOnEntered();
                }

                mHovered = true;
                return true;
            } else if(mHovered) {
                if(mOnExited) {
                    mOnExited();
                }
                mHovered = false;
            }
            
        }
    
        if(e.type == sf::Event::MouseButtonPressed) {
            if(e.mouseButton.button == sf::Mouse::Button::Left) {
                mActive = getGlobalBounds().contains(sf::Vector2f(e.mouseButton.x, e.mouseButton.y));
                if(mActive) {
                    // how for the cursor is from the left of the TextField
                    float x_diff = e.mouseButton.x-(getGlobalPosition().x+text_padding);
                    
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                       mCursorPositionEnd = findNearestCursorPoint(x_diff);
                    } else {
                        mCursorPositionStart = findNearestCursorPoint(x_diff);
                        mCursorPositionEnd = mCursorPositionStart;
                    }
                    updateCursor();
                    
                    if(mOnActivated) {
                        mOnActivated();
                    }
                    mSelecting = true;
                }
                
                if(!mActive && mOnDeactivated) {
                    mOnDeactivated();
                }
                return mActive;
            }
        }
        
        if(e.type == sf::Event::MouseButtonReleased) {
            if(e.mouseButton.button == sf::Mouse::Button::Left)
                mSelecting = false;
        }

        if(e.type == sf::Event::KeyPressed && mActive) {
            if(e.key.code == sf::Keyboard::Key::Left) {
                moveCursorLeft(e.key.shift);
            } else if(e.key.code == sf::Keyboard::Key::Right) {
                moveCursorRight(e.key.shift);
            } else if(e.key.code == sf::Keyboard::Key::Delete) {
                if(mCursorPositionStart == mCursorPositionEnd) {
                    if(mCursorPositionStart < mStr.getSize()) {
                        mStr.erase(mCursorPositionStart);
                        updateText();
                    }
                } else {
                    deleteInsideCursor();
                }
                if(mOnChanged) {
                    mOnChanged();
                }
                resetCursorBlink();
            } else if(e.key.code == sf::Keyboard::Key::BackSpace) {
                if(mCursorPositionStart == mCursorPositionEnd) {
                    if(mCursorPositionStart > 0) {
                        mStr.erase(mCursorPositionStart-1);
                        moveCursorLeft(false);
                        updateText();
                    }
                } else {
                    deleteInsideCursor();
                }
                if(mOnChanged) {
                    mOnChanged();
                }
                resetCursorBlink();
            } else if(e.key.code == sf::Keyboard::Key::Return) {
                mActive = false;
                if(mOnDeactivated) {
                    mOnDeactivated();
                }
            }
            // all input is captured because mActive is true
            return true;
        }
        if(e.type == sf::Event::TextEntered && mActive) {
            // backspace = 8, enter = 13, tab = 9, escape = 27, delete = 127
            for(int key : {8,13,9,27,127}) {
                if(e.text.unicode == key) return true;
            }
            
            if(mCursorPositionStart != mCursorPositionEnd) {
              deleteInsideCursor();
            }
            if(mText.getLocalBounds().left +
                      mText.getLocalBounds().width +
                      mText.getFont()->getGlyph(e.text.unicode,mText.getCharacterSize(), false).advance < getGlobalBounds().width-text_padding) {
                mStr.insert(mCursorPositionStart, e.text.unicode);
                moveCursorRight(false);
            }
            if(mOnChanged) {
                mOnChanged();
            }
            updateText();
            return true;
        }
        return false;
    }
    
    void TextField::updateTheme() {
        mRectangleShape.setFillColor(sf::Color(255,255,255,255)); // I d
        mRectangleShape.setOutlineColor(getColorProperty("outlineColor"));
        mRectangleShape.setOutlineThickness(getNumberProperty("outlineThickness"));
        mText.setFont(*getFontProperty("font"));
        mText.setColor(getColorProperty("textColor"));
        mText.setCharacterSize(getNumberProperty("fontSize"));
    }
    
    void TextField::setOnEntered(std::function<void()> func) {
        mOnEntered = func;
    }
    void TextField::setOnExited(std::function<void()> func) {
        mOnExited = func;
    }
    void TextField::setOnActivated(std::function<void()> func) {
        mOnActivated = func;
    }
    void TextField::setOnDeactivated(std::function<void()> func) {
        mOnDeactivated = func;
    }
    void TextField::setOnChanged(std::function<void()> func) {
        mOnChanged = func;
    }
    
    void TextField::layoutChanged() {
        Widget::layoutChanged();
        float outlineThickness = getNumberProperty("outlineThickness");
        mRectangleShape.setPosition(sf::Vector2f(getGlobalPosition().x+outlineThickness, getGlobalPosition().y+outlineThickness));
        mRectangleShape.setSize(getSize()-sf::Vector2f(2*outlineThickness, 2*outlineThickness));
        
        mCursorShape.setSize(sf::Vector2f(1, getSize().y-8));
        mCursorShape.setOrigin(sf::Vector2f(0,-4));
        
        updateText();
        updateCursor();
    }
    std::string TextField::getThemeObjectType() {
        return "sui::TextField";
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
            if(mCursorPositionStart > mStr.getSize()) mCursorPositionStart = mStr.getSize();
        }
        mCursorPositionEnd++;
        if(mCursorPositionEnd > mStr.getSize()) mCursorPositionEnd = mStr.getSize();
        updateCursor();
    }
    
    int TextField::findNearestCursorPoint(float x_diff) {
        for(unsigned int i = 0;i < mStr.getSize(); i++) {
            auto &glyph = mText.getFont()->getGlyph(mStr[i],mText.getCharacterSize(), false);
            x_diff -= glyph.advance;
            if(x_diff <= 0) {
                if(x_diff >= -glyph.advance/2) {
                    return i+1;
                }
                return i;
            }
        }
        return mStr.getSize();
    }
    
    void TextField::updateText() {
        mText.setString(mStr);

        setTextOrigin(mText, ORIGIN_START, ORIGIN_MIDDLE, 't');
        auto r = getGlobalBounds();
        mText.setPosition(sf::Vector2f(r.left+5, r.top+r.height/2));
        mText.setCharacterSize(r.height*.7); //.7 is jut a magic number to make letters like 'g' fit in
    }
    
    void TextField::updateCursor() {
        auto pos_1 = sf::Vector2f(getGlobalBounds().left+text_padding, getGlobalBounds().top);
        for(unsigned int i = 0;i < mCursorPositionStart; i++) {
            auto &glyph = mText.getFont()->getGlyph(mStr[i],mText.getCharacterSize(), false);
            pos_1.x += glyph.advance;
        }
        mCursorShape.setPosition(pos_1);
        
        if(mCursorPositionStart != mCursorPositionEnd) {
            auto pos_2 = sf::Vector2f(getGlobalBounds().left+text_padding, getGlobalBounds().top);
            for(unsigned int i = 0;i < mCursorPositionEnd; i++) {
                auto &glyph = mText.getFont()->getGlyph(mStr[i],mText.getCharacterSize(), false);
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
        mStr.erase(start, end-start);
        mCursorPositionStart = start;
        mCursorPositionEnd = mCursorPositionStart;
        updateText();
        updateCursor();
    }
}

