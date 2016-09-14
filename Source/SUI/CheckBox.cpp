#include <SUI/CheckBox.hpp>

namespace sui {
    CheckBox::CheckBox() : Widget() {
        mState = false;
        mCrossShape1.setRotation(45);
        mCrossShape2.setRotation(135);
        updateFillColors();
    }
    
    void CheckBox::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mRectangleShape, states);
        if(mState) {
            target.draw(mCrossShape1, states);
            target.draw(mCrossShape2, states);
        }
    }
    
    void CheckBox::onInput(sf::Event e) {
        if(e.type == sf::Event::MouseButtonPressed) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                mState = !mState;
                onToggled();
                if(mState) {
                    onToggledOn();
                }
                if(!mState) {
                    onToggledOff();
                }
            }
        }
    }
    void CheckBox::onUpdate(const bool posChanged, const bool sizeChanged) {
        const bool thickness_changed = hasPropChanged("outlineThickness");
        if(thickness_changed) {
            const float bar_width = getProperty("outlineThickness").as<float>();
            mRectangleShape.setOutlineThickness(-bar_width);
            mCrossShape1.setOrigin(sf::Vector2f(0, bar_width/2.f));
            mCrossShape2.setOrigin(sf::Vector2f(0, bar_width/2.f));
        }
        
        if(posChanged) {
            mRectangleShape.setPosition(getGlobalTopLeftCorner());
        }
        if(sizeChanged) {
            mRectangleShape.setSize(getSize());
        }
        if(posChanged || sizeChanged) {
            calculateCross();
        }
    }
    void CheckBox::calculateCross() {
        constexpr float sqrt_2 = 1.41421356237;
        
        const float left = getGlobalTopLeftCorner().x;
        const float top = getGlobalTopLeftCorner().y;
        
        mCrossShape1.setPosition(sf::Vector2f(left+getSize().x/2-getSize().x/(sqrt_2*2), top+getSize().y/2-getSize().y/(sqrt_2*2)));
        mCrossShape2.setPosition(sf::Vector2f(left+getSize().x/2+getSize().x/(sqrt_2*2), top+getSize().y/2-getSize().y/(sqrt_2*2)));
        
        const float bar_width = getOutlineThickness();
        mCrossShape1.setSize(sf::Vector2f(getSize().x, bar_width));
        mCrossShape2.setSize(sf::Vector2f(getSize().x, bar_width));
    }
    
    void CheckBox::updateFillColors() {
        mRectangleShape.setFillColor(sf::Color(0,0,0,255));
        mRectangleShape.setOutlineColor(sf::Color(255,255,255,255));
    }
}
