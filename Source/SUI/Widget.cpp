#include <SUI/Widget.hpp>
#include <SUI/Container.hpp>
#include <iostream>
namespace sui {
    int Widget::current_id = 0;
    Widget::Widget(Theme &theme) : mTheme(&theme) {
        mInvalidGlobalPosition = true;
        mParent = nullptr;
        
        mOriginX = getPreferredOriginX();
        mOriginY = getPreferredOriginY();
        
        // something default...
        mLocalPosition = sf::Vector2f(0,0);
        mLocalSize = sf::Vector2f(0,0);
        
        mContainerData = nullptr;
        id = current_id++;
        
        Theme::registerType(getThemeObject(), getThemeObjectType());
    }
    Widget::~Widget() {
        ;
    }
    
    bool Widget::setPosition(sf::Vector2f pos) {
        if(mParent && !mParent->childrenCanMove()) {
            return false;
        }
        mLocalPosition = pos;
        mInvalidGlobalPosition = true;
        
        return true;
    }
    sf::Vector2f Widget::getLocalPosition() const {
        return mLocalPosition;
    }
    sf::Vector2f Widget::getGlobalPosition() {
        updateGlobalPosition();
        return mGlobalPosition;
    }
    
    bool Widget::setSize(sf::Vector2f size) {
        if(mParent && !mParent->childrenCanResize()) {
            return false;
        }
        mLocalSize = size;
        
        return true;
    }
    sf::Vector2f Widget::getSize() const {
        return mLocalSize;
    }
    
    
    sf::FloatRect Widget::getLocalBounds() const {
        return _getBoundsGeneric(getLocalPosition().x,
                                 getLocalPosition().y,
                                 getSize().x,
                                 getSize().y);
    }
    
    sf::FloatRect Widget::getGlobalBounds() {
        return _getBoundsGeneric(getGlobalPosition().x,
                                 getGlobalPosition().y,
                                 getSize().x,
                                 getSize().y);
    }
    
    bool Widget::setOrigin(ORIGIN originX, ORIGIN originY) {
        if(mParent && !mParent->childrenCanMove()) {
            return false;
        }
        mOriginX = originX;
        mOriginY = originY;
        
        mInvalidGlobalPosition = true;
        return true;
    }
    
    
    Widget *Widget::getParent() {
        return mParent;
    }
    
    void Widget::layoutChanged() {
        ;
    }
    
    std::string Widget::getThemeObject() {
        return std::string("widget_gen_id@") + std::to_string(id);
    }
    std::string Widget::getThemeObjectType() {
        return "sui::Widget";
    }
    
    void Widget::setNumberProperty(const std::string property, float value) {
        mTheme->setNumberProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setBoolProperty(const std::string property, bool value) {
        mTheme->setBoolProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setColorProperty(const std::string property, sf::Color value) {
        mTheme->setColorProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setVector2fProperty(const std::string property, sf::Vector2f value) {
        mTheme->setVector2fProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setOriginProperty(const std::string property, ORIGIN value) {
        mTheme->setOriginProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setStringProperty(const std::string property, sf::String value) {
        mTheme->setStringProperty(getThemeObject() + "." + property, value);
    }
    void Widget::setFontProperty(const std::string property, std::shared_ptr<sf::Font> value) {
        mTheme->setFontProperty(getThemeObject() + "." + property, value);
    }
    
    
    float Widget::getNumberProperty(const std::string property) {
        return mTheme->getNumberProperty(getThemeObject() + "." + property);
    }
    bool Widget::getBoolProperty(const std::string property) {
        return mTheme->getBoolProperty(getThemeObject() + "." + property);
    }
    sf::Color Widget::getColorProperty(const std::string property) {
        return mTheme->getColorProperty(getThemeObject() + "." + property);
    }
    sf::Vector2f Widget::getVector2fProperty(const std::string property) {
        return mTheme->getVector2fProperty(getThemeObject() + "." + property);
    }
    ORIGIN Widget::getOriginProperty(const std::string property) {
        return mTheme->getOriginProperty(getThemeObject() + "." + property);
    }
    sf::String Widget::getStringProperty(const std::string property) {
        return mTheme->getStringProperty(getThemeObject() + "." + property);
    }
    std::shared_ptr<sf::Font> Widget::getFontProperty(const std::string property) {
        return mTheme->getFontProperty(getThemeObject() + "." + property);
    }
    std::shared_ptr<void> Widget::getSharedProperty(const std::string property) {
        return mTheme->getSharedProperty(getThemeObject() + "." + property);
    }
    Theme &Widget::getTheme() {
        return *mTheme;
    }
    
    sf::FloatRect Widget::_getBoundsGeneric(const float x, const float y, const float w, const float h) const {
        sf::FloatRect rect;
        if(mOriginX == ORIGIN_START) {
            rect.left = x;
        } else if(mOriginX == ORIGIN_MIDDLE) {
            rect.left = x-w/2.f;
        } else if(mOriginX == ORIGIN_END) {
            rect.left = x-w;
        } else {
            throw "Error! mOriginX is some how an invalid value";
        }
        
        if(mOriginY == ORIGIN_START) {
            rect.top = y;
        } else if(mOriginY == ORIGIN_MIDDLE) {
            rect.top = y-h/2.f;
        } else if(mOriginY == ORIGIN_END) {
            rect.top = y-h;
        } else {
            throw "Error! mOriginY is some how an invalid value";
        }
        
        rect.width = w;
        rect.height = h;
        return rect;
    }
    
    
    void Widget::updateGlobalPosition() {
        if(mInvalidGlobalPosition) {
            mGlobalPosition = mLocalPosition;
            if(mParent) {
                mGlobalPosition += mParent->getGlobalPosition();
            }
            mInvalidGlobalPosition = false;
        }
    }
    
    ORIGIN Widget::getPreferredOriginX() const {
        if(mParent) {
            return mParent->getPreferredChildOriginX();
        }
        return ORIGIN_START;
    }
    
    ORIGIN Widget::getPreferredChildOriginX() const {
        return ORIGIN_START;
    }
    
    
    ORIGIN Widget::getPreferredOriginY() const {
        if(mParent) {
            return mParent->getPreferredChildOriginY();
        }
        return ORIGIN_START;
    }
    
    ORIGIN Widget::getPreferredChildOriginY() const {
        return ORIGIN_START;
    }
    
    
}
