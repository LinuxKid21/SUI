#pragma once
#include "Theme.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>

namespace sui {
    class Container;
    
    /* For all (non pure) virtual functions overriden make sure to call the
    * parent function as well. Things may work wierd if you don't do this!
    * A widget can only make requests to it's container(no parent means full control).
    * This design allows containers to not have to worry about any concievable possibility.
    */
    class Widget : public sf::Drawable {
        // container needs a few special priviledges
        friend class Container;
    public:
        Widget();
        virtual ~Widget();
        
        void handleInput(sf::Event e);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update();
        Container *getParent();
        
        // sets the local position and returns false if the parent did not consent permission
        bool setPosition(sf::Vector2f pos);
        sf::Vector2f getLocalPosition() const;
        sf::Vector2f getGlobalPosition() const;
        
        // sets the local size and returns false if the parent did not consent permission
        bool setSize(sf::Vector2f size);
        sf::Vector2f getSize() const;
        
        sf::Vector2f getLocalTopLeftCorner() const;
        sf::Vector2f getGlobalTopLeftCorner() const;
        
        // these are very helpful because they take origin into account for you.
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds();
        
        bool setOrigin(ORIGIN originX, ORIGIN originY);
        
        const Property &getProperty(const std::string key) const;
        
        void setProperty(const std::string key, const Property &prop) {
            mProperties[key] = prop;
            _setPropertyChanged(key);
        }
        
        // from next line to line thereafter with this nice change
        // setProperty("key", sui::Property::make<type>(value))
        // setProperty<type>("key", value)
        template <class T>
        void setProperty(const std::string key, T prop) {
            setProperty(key, Property::make<T>(std::move(prop)));
        }
        
        bool removeProperty(const std::string key);
        void clearAllProperties();
        
        // to be used ONLY inside onUpdate to detect changed properties
        bool hasPropChanged(const std::string key) {
            return mChangedKeys.find(key) != mChangedKeys.end();
        }
        
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
        virtual void onInput(sf::Event e);
        virtual void onUpdate(const bool posChanged, const bool sizeChanged);
        
        
        ORIGIN mOriginX;
        ORIGIN mOriginY;
        
    private:
        bool mPosChanged;
        bool mSizeChanged;
        Property nullProperty;
        // necessary extra layer of complexity so container has some extra control nothing else needs
        virtual void _onUpdate();
        virtual void _setPropertyChanged(const std::string key) {
            mChangedKeys.insert(key);
        }
        
        void onEntered() {
            const Property &p = getProperty("onEntered");
            if(p) p.asFunc()();
        }
        void onExited() {
            const Property &p = getProperty("onExited");
            if(p) p.asFunc()();
        }
        void onClickedDown() {
            const Property &p = getProperty("onClickedDown");
            if(p) p.asFunc()();
        }
        void onClickedUp() {
            const Property &p = getProperty("onClickedUp");
            if(p) p.asFunc()();
        }
        
        // helper for getLocalBounds and getGlobalBounds functions
        sf::FloatRect _getBoundsGeneric(const float x, const float y, const float w, const float h) const;
        
        Container *mParent;
        
        sf::Vector2f mLocalPosition;
        sf::Vector2f mLocalSize;
        
        mutable sf::Vector2f mGlobalPosition;
        
        // setting this to true means mGlobalPosition will be recomputed next time getGlobalPosition is called
        mutable bool mInvalidGlobalPosition;
        
        // global position calculation
        void updateGlobalPosition() const;
        
        bool mVisible;
        std::map <std::string, Property> mProperties;
        std::unordered_set<std::string> mChangedKeys;
    };
}