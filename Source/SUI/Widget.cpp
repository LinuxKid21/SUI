#include <SUI/Widget.hpp>
#include <SUI/Container.hpp>
#include <iostream>
namespace sui {
    Widget::Widget() {
        mInvalidGlobalPosition = true;
        mParent = nullptr;
        
        mOriginX = ORIGIN_START;
        mOriginY = ORIGIN_START;
        
        // something default...
        mLocalPosition = sf::Vector2f(0,0);
        mLocalSize = sf::Vector2f(0,0);
        
        mVisible = true;
        
        mPosChanged = false;
        mSizeChanged = false;
        
        setProperty<bool>("hovering", false);
        setProperty<bool>("clicking", false);
    }
    Widget::~Widget() {
        ;
    }
    
    void Widget::handleInput(sf::Event e) {
        if(!mVisible) return;
        onInput(e);
    }
    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(!mVisible) return;
        onDraw(target, states);
    }
    void Widget::update() {
        mVisible = getProperty("visible") ? getProperty("visible").as<bool>() : true;
        if(!mVisible) return;
        _onUpdate();
    }
    // overriden in Container
    void Widget::_onUpdate() {
        // an origin in the center or right means that to resize is to move
        mPosChanged = mPosChanged ||
                mSizeChanged && (mOriginX != ORIGIN_START || mOriginY != ORIGIN_START);
        if(mPosChanged) mInvalidGlobalPosition = true;
        onUpdate(mPosChanged, mSizeChanged);
        mSizeChanged = false, mPosChanged = false;
        mChangedKeys.clear();
    }
    void Widget::onUpdate(const bool posChanged, const bool sizeChanged) {
        if(hasPropChanged("hovering")) {
            if(getProperty("hovering").as<bool>()) {
                onEntered();
            } else {
                onExited();
            }
        }
        if(hasPropChanged("clicking")) {
            if(getProperty("clicking").as<bool>()) {
                onClickedDown();
            } else {
                onClickedUp();
            }
        }
    }
    
    void Widget::onInput(sf::Event e) {
        if(e.type == sf::Event::MouseMoved) {
            const auto mousePos = sf::Vector2f(e.mouseMove.x, e.mouseMove.y);
            if(getGlobalBounds().contains(mousePos)) {
                if(!getProperty("hovering").as<bool>())
                    setProperty<bool>("hovering", true);
            } else if(getProperty("hovering").as<bool>()) {
                setProperty<bool>("hovering", false);
            }
        } else if(e.type == sf::Event::MouseButtonPressed) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                setProperty<bool>("clicking", true);
            }
        } else if(e.type == sf::Event::MouseButtonReleased && getProperty("clicking").as<bool>()) {
            const auto mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);
            if(getGlobalBounds().contains(mousePos)) {
                if(!getProperty("hovering").as<bool>())
                    setProperty<bool>("hovering", true);
            } else {
                setProperty<bool>("hovering", false);
            }
            setProperty<bool>("clicking", false);
        }
    }
    
    bool Widget::setPosition(sf::Vector2f pos) {
        if(mParent && !mParent->childrenCanMove()) {
            return false;
        }
        mLocalPosition = pos;
        mInvalidGlobalPosition = true;
        mPosChanged = true;
        return true;
    }
    sf::Vector2f Widget::getLocalPosition() const {
        return mLocalPosition;
    }
    sf::Vector2f Widget::getGlobalPosition() const {
        updateGlobalPosition();
        return mGlobalPosition;
    }
    
    bool Widget::setSize(sf::Vector2f size) {
        if(mParent && !mParent->childrenCanResize()) {
            return false;
        }
        mLocalSize = size;
        mSizeChanged = true;
        
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
        mPosChanged = true;
        
        return true;
    }
    
    
    Container *Widget::getParent() {
        return mParent;
    }
    
    const Property &Widget::getProperty(const std::string key) const {
        // traverse up the tree to find the property
        const Widget *iter = this;
        while(iter) {
            if(iter->mProperties.find(key) != iter->mProperties.end()) {
                return iter->mProperties.at(key);
            }
            iter = iter->mParent;
        }
        return nullProperty;
    }
    bool Widget::removeProperty(const std::string key) {
        return mProperties.erase(key) == 1;
    }
    void Widget::clearAllProperties() {
        mProperties.clear();
    }

    sf::Vector2f Widget::getLocalTopLeftCorner() const {
        sf::Vector2f pos;
        const float x = getLocalPosition().x;
        const float y = getLocalPosition().y;
        const float w = getSize().x;
        const float h = getSize().y;
        if(mOriginX == ORIGIN_START) {
            pos.x = x;
        } else if(mOriginX == ORIGIN_MIDDLE) {
            pos.x = x-w/2.f;
        } else if(mOriginX == ORIGIN_END) {
            pos.x = x-w;
        }
        
        if(mOriginY == ORIGIN_START) {
            pos.y = y;
        } else if(mOriginY == ORIGIN_MIDDLE) {
            pos.y = y-h/2.f;
        } else if(mOriginY == ORIGIN_END) {
            pos.y = y-h;
        }
        return pos;
    }
    sf::Vector2f Widget::getGlobalTopLeftCorner() const {
        sf::Vector2f pos;
        const float x = getGlobalPosition().x;
        const float y = getGlobalPosition().y;
        const float w = getSize().x;
        const float h = getSize().y;
        if(mOriginX == ORIGIN_START) {
            pos.x = x;
        } else if(mOriginX == ORIGIN_MIDDLE) {
            pos.x = x-w/2.f;
        } else if(mOriginX == ORIGIN_END) {
            pos.x = x-w;
        }
        
        if(mOriginY == ORIGIN_START) {
            pos.y = y;
        } else if(mOriginY == ORIGIN_MIDDLE) {
            pos.y = y-h/2.f;
        } else if(mOriginY == ORIGIN_END) {
            pos.y = y-h;
        }
        return pos;
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
    
    
    void Widget::updateGlobalPosition() const {
        if(mInvalidGlobalPosition) {
            mGlobalPosition = mLocalPosition;
            if(mParent) {
                mGlobalPosition += mParent->getGlobalPosition();
            }
            mInvalidGlobalPosition = false;
        }
    }
}
