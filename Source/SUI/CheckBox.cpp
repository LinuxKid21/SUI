#include <SUI/CheckBox.hpp>

namespace sui {
    CheckBox::CheckBox() : Widget() {
        mState = false;
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

    void CheckBox::onUpdate() {
        const float left = getGlobalBounds().left;
        const float top = getGlobalBounds().top;
        constexpr float sqrt_2 = 1.41421356237;
        const float bar_width = getProperty("outlineThickness").as<float>();
        mCrossShape1.setSize(sf::Vector2f(getSize().x, bar_width));
        mCrossShape1.setOrigin(sf::Vector2f(0, bar_width/2.f));
        mCrossShape1.setPosition(sf::Vector2f(left+getSize().x/2-getSize().x/(sqrt_2*2), top+getSize().y/2-getSize().y/(sqrt_2*2)));
        mCrossShape1.setRotation(45);
        
        mCrossShape2.setSize(sf::Vector2f(getSize().x, bar_width));
        mCrossShape2.setOrigin(sf::Vector2f(0, bar_width/2.f));
        mCrossShape2.setPosition(sf::Vector2f(left+getSize().x/2+getSize().x/(sqrt_2*2), top+getSize().y/2-getSize().y/(sqrt_2*2)));
        mCrossShape2.setRotation(135);
        
        mRectangleShape.setPosition(sf::Vector2f(left, top));
        mRectangleShape.setSize(getSize());
        
        
        updateFillColors();
    }
    
    void CheckBox::updateFillColors() {
        mRectangleShape.setFillColor(sf::Color(0,0,0,255));
        mRectangleShape.setOutlineThickness(-getProperty("outlineThickness").as<float>());
        mRectangleShape.setOutlineColor(sf::Color(255,255,255,255));
    }
}
