#include <SUI/ComboBox.hpp>
#include <SUI/Utility.hpp>


namespace sui {
    ComboBox::ComboBox() : Widget() {
        mCurrentText = 0;
        mItemCount = 0;
        
        // we don't use all allocated space because that seems tight.
        const float real_size = arrow_size-4;
        mLeftArrow.setPointCount(3);
        mLeftArrow.setPoint(0, sf::Vector2f(0, real_size/2));
        mLeftArrow.setPoint(1, sf::Vector2f(real_size, real_size));
        mLeftArrow.setPoint(2, sf::Vector2f(real_size, 0));
        mLeftArrow.setOrigin(sf::Vector2f(real_size/2, real_size/2));
        
        mRightArrow.setPointCount(3);
        mRightArrow.setPoint(0, sf::Vector2f(real_size, real_size/2));
        mRightArrow.setPoint(1, sf::Vector2f(0, real_size));
        mRightArrow.setPoint(2, sf::Vector2f(0, 0));
        mRightArrow.setOrigin(sf::Vector2f(real_size/2, real_size/2));
    }
    
    void ComboBox::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        target.draw(mLeftArrow, states);
        target.draw(mRightArrow, states);
        if(mCurrentText < mTexts.size()) target.draw(mTexts[mCurrentText], states);
    }
    void ComboBox::onUpdate(const bool posChanged, const bool sizeChanged) {
        if(sizeChanged) {
            placeArrows();
            mRectangleShape.setSize(getSize());
        }
        if(posChanged) {
            placeArrows();
            mRectangleShape.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
        }
        
        if(hasPropChanged("textColor")) {
            mLeftArrow.setFillColor(getProperty("textColor").as<sf::Color>());
            mRightArrow.setFillColor(getProperty("textColor").as<sf::Color>());
        }
        if(hasPropChanged("outlineColor")) {
            mRectangleShape.setOutlineColor(getProperty("outlineColor").as<sf::Color>());
        }
        if(hasPropChanged("fillColor")) {
            mRectangleShape.setFillColor(getProperty("fillColor").as<sf::Color>());
        }
        if(hasPropChanged("outlineThickness")) {
            mRectangleShape.setOutlineThickness(-getProperty("outlineThickness").as<float>());
        }
        bool itemChanged = false;
        for(unsigned int i = 0;i <= mItemCount; i++) {
            std::string item = std::string("item") + std::to_string(i);
            if(hasPropChanged(item)) {
                if(i < mStrings.size()) {
                    mStrings[i] = getProperty(item).as<sf::String>();
                } else {
                    mStrings.push_back(getProperty(item).as<sf::String>());
                    mItemCount++;
                }
                itemChanged = true;
            }
        }
        if(itemChanged || hasPropChanged("font")) updateTexts();
    }
    void ComboBox::placeArrows() {
        mLeftArrow.setPosition(getGlobalTopLeftCorner().x+getTextPadding()+arrow_size/2, getGlobalTopLeftCorner().y+getSize().y/2);
        mRightArrow.setPosition(getGlobalTopLeftCorner().x+getSize().x-getTextPadding()-arrow_size/2, getGlobalTopLeftCorner().y+getSize().y/2);
    }
    void ComboBox::updateTextPositions() {
        if(!getProperty("font")) return;
        int x;
        int y;
        switch(getTextOriginX()) {
            case ORIGIN_START:
                x = getGlobalBounds().left + getTotalTextPadding();
                break;
            case ORIGIN_MIDDLE:
                x = getGlobalBounds().left+getGlobalBounds().width/2;
                break;
            case ORIGIN_END:
                x = getGlobalBounds().left+getGlobalBounds().width - getTotalTextPadding();
                break;
            default:
                throw "mTextOriginX is not a valid value!";
        }
        
        
        switch(getTextOriginY()) {
            case ORIGIN_START:
                y = getGlobalBounds().top + getTotalTextPadding();
                break;
            case ORIGIN_MIDDLE:
                y = getGlobalBounds().top+getGlobalBounds().height/2;
                break;
            case ORIGIN_END:
                y = getGlobalBounds().top+getGlobalBounds().height - getTotalTextPadding();
                break;
            default:
                throw "mTextOriginY is not a valid value!";
        }
        
        for(auto &t : mTexts) {
            setTextOrigin(t, getTextOriginX(), getTextOriginY(), 'X');
            t.setPosition(sf::Vector2f(x,y));
        }
    }
    void ComboBox::updateTexts() {
        if(!getProperty("font")) return;
        mTexts.clear();
        for(unsigned int i = 0; i < mStrings.size(); i++) {
            sf::Text text;
            text.setFont(*getProperty("font").as<sf::Font *>());
            text.setColor(getProperty("textColor").as<sf::Color>());
            text.setCharacterSize(getProperty("fontSize").as<float>());
            text.setString(mStrings[i]);
            mTexts.push_back(std::move(text));
        }
        updateTextPositions();
    }
    void ComboBox::onInput(sf::Event e) {
        Widget::onInput(e);
        if(e.type == sf::Event::MouseButtonPressed) {
            sf::Rect<float> left = sf::Rect<float>(getGlobalTopLeftCorner().x, getGlobalTopLeftCorner().y, arrow_size, getSize().y);
            sf::Rect<float> right = sf::Rect<float>(getSize().x + getGlobalTopLeftCorner().x-arrow_size, getGlobalTopLeftCorner().y, arrow_size, getSize().y);
            if(left.contains(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
                if(mCurrentText != 0) mCurrentText--;
                else mCurrentText = mTexts.size()-1;
            } else if(right.contains(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
                if(mCurrentText != mTexts.size()-1) mCurrentText++;
                else mCurrentText = 0;
            }
        }
    }
}
