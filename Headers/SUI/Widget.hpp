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
        Widget(Theme &theme);
        virtual ~Widget();
        
        virtual bool handleInput(sf::Event e) = 0;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
        
        // sets the local position and returns false if the parent did not consent permission
        bool setPosition(sf::Vector2f pos);
        sf::Vector2f getLocalPosition() const;
        sf::Vector2f getGlobalPosition();
        
        // sets the local size and returns false if the parent did not consent permission
        bool setSize(sf::Vector2f size);
        sf::Vector2f getSize() const;
        
        // these are very helpful because they take origin into account for you.
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds();
        
        bool setOrigin(ORIGIN originX, ORIGIN originY);
        Widget *getParent();
        
        virtual void layoutChanged();
        
        std::string getThemeObject();
        
        virtual std::string getThemeObjectType();
        
        
        void setNumberProperty(const std::string property, float value);
        void setBoolProperty(const std::string property, bool value);
        void setColorProperty(const std::string property, sf::Color value);
        void setVector2fProperty(const std::string property, sf::Vector2f value);
        void setOriginProperty(const std::string property, ORIGIN value);
        void setStringProperty(const std::string property, sf::String value);
        void setFontProperty(const std::string property, std::shared_ptr<sf::Font> value);
        template<class T>
        void setVoidProperty(const std::string property, T *value) {
            mTheme->setVoidProperty<T>(getThemeObject() + "." + property, value);
        }
        
        
        float getNumberProperty(const std::string property);
        bool getBoolProperty(const std::string property);
        sf::Color getColorProperty(const std::string property);
        sf::Vector2f getVector2fProperty(const std::string property);
        ORIGIN getOriginProperty(const std::string property);
        sf::String getStringProperty(const std::string property);
        std::shared_ptr<sf::Font> getFontProperty(const std::string property);
        template<class T>
        T getVoidProperty(const std::string property) {
            return mTheme->getVoidProperty<T>(getThemeObject() + "." + property);
        }
        std::shared_ptr<void> getSharedProperty(const std::string property);
        
        Theme &getTheme();
    private:
        // helper for getLocalBounds and getGlobalBounds functions
        sf::FloatRect _getBoundsGeneric(const float x, const float y, const float w, const float h) const;
    
        Container *mParent;
        
        sf::Vector2f mLocalPosition;
        sf::Vector2f mLocalSize;
        
        sf::Vector2f mGlobalPosition;
        
        // setting this to true means mGlobalPosition will be recomputed next time getGlobalPosition is called
        bool mInvalidGlobalPosition;
        
        // global position calculation(always calculated from the origin)
        void updateGlobalPosition();
        
        // containers almost always need some specific data it stores with its
        // widgets of all types so here's a void pointer!
        // BoxLayout for example allows children to define their size(in either or both directions)
        // or to figure out sizes automatically.
        void *mContainerData;
        
        int id;
        static int current_id;
        
        Theme *mTheme;
    protected:
        ORIGIN mOriginX;
        ORIGIN mOriginY;
        
        // I see no reason this should be virtual
        ORIGIN getPreferredOriginX() const;
        ORIGIN getPreferredOriginY() const;
        
        // override this to communicate the most desired origin of children widgets
        // A table class might return the center of it's cell so it makes sense children
        // have origin in center so they align well
        virtual ORIGIN getPreferredChildOriginX() const;
        virtual ORIGIN getPreferredChildOriginY() const;
    };
}