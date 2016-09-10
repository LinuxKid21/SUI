#pragma once
#include "Theme.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

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
        void setProperty(const std::string key, const Property &prop);
        bool removeProperty(const std::string key);
        void clearAllProperties();
        
    protected:
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
        virtual void onInput(sf::Event e) = 0;
        virtual void onUpdate() {};
        virtual void onPropertyChanged(const std::string key) {}
        virtual void onPositionChanged() {}
        virtual void onSizeChanged() {}
        
        ORIGIN mOriginX;
        ORIGIN mOriginY;
        
    private:
        Property nullProperty;
        // necessary extra layer of complexity so container has some extra control nothing else needs
        virtual void _onUpdate();
        virtual void _onPositionChanged();
        virtual void _onSizeChanged();
        
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
        
        // containers almost always need some specific data it stores with its
        // widgets of all types so here's a void pointer!
        // BoxLayout for example allows children to define their size(in either or both directions)
        // or to figure out sizes automatically.
        void *mContainerData;
        
        bool mVisible;
        std::map <std::string, Property> mProperties;
    };
}