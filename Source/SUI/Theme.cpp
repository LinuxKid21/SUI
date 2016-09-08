#include <SUI/Theme.hpp>

namespace sui {
    int Theme::ObjectType::current_id = 0;
    int Theme::Property::current_id = 0;
    
    std::map <std::string, Theme::ObjectType> Theme::parentObjectType;
    
    void Theme::loadDefaults(const char *font_file) {
        sf::Font *font = new sf::Font();
        font->loadFromFile(font_file);
        setFontProperty("sui::Widget.font", std::shared_ptr<sf::Font>(font));
        setNumberProperty("sui::Widget.outlineThickness", 2);
        setColorProperty("sui::Widget.fillColor", sf::Color(255,255,255,255));
        setColorProperty("sui::Widget.fillColorHovered", sf::Color(200,200,200,255));
        setColorProperty("sui::Widget.fillColorClicked", sf::Color(100,100,100,255));
        setColorProperty("sui::Widget.textColor", sf::Color(0,0,0,255));
        setColorProperty("sui::Widget.outlineColor", sf::Color(0,255,0,255));
        setNumberProperty("sui::Widget.fontSize", 24);
    }
    
    void Theme::registerType(const std::string new_type, const std::string parent) {
        if(parentObjectType.find(new_type) != parentObjectType.end()) {
            // I think we should ignore it so we can call this in all
            // constructors as many times as necessary(the only way
            // to remove the spaghetti code alternative I can find)
            return;
        }
        if(parentObjectType.find(parent) == parentObjectType.end() && parent != "") {
            throw std::string("Parent '") + parent + std::string("' does not exist!");
        }
        parentObjectType[new_type] = ObjectType();
        parentObjectType[new_type].name = new_type;
        if(parent != "")
            parentObjectType[new_type].parent = &parentObjectType[parent];
        else
            parentObjectType[new_type].parent = nullptr;
    }
    
    void Theme::setNumberProperty(const std::string key, float value) {
        if(hasProperty(key)) checkProperty(key, TYPE_FLOAT);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propFloat = value;
        properties[key].type = TYPE_FLOAT;
    }
    void Theme::setBoolProperty(const std::string key, bool value) {
        if(hasProperty(key)) checkProperty(key, TYPE_BOOL);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propBool = value;
        properties[key].type = TYPE_BOOL;
    }
    void Theme::setColorProperty(const std::string key, sf::Color value) {
        if(hasProperty(key)) checkProperty(key, TYPE_COLOR);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propColor = value;
        properties[key].type = TYPE_COLOR;
    }
    void Theme::setVector2fProperty(const std::string key, sf::Vector2f value) {
        if(hasProperty(key)) checkProperty(key, TYPE_VECTOR2F);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propVector2f = value;
        properties[key].type = TYPE_VECTOR2F;
    }
    void Theme::setOriginProperty(const std::string key, ORIGIN value) {
        if(hasProperty(key)) checkProperty(key, TYPE_ORIGIN);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propOrigin = value;
        properties[key].type = TYPE_ORIGIN;
    }
    void Theme::setStringProperty(const std::string key, sf::String value) {
        if(hasProperty(key)) checkProperty(key, TYPE_ORIGIN);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propString = value;
        properties[key].type = TYPE_STRING;
    }
    void Theme::setFontProperty(const std::string key, std::shared_ptr<sf::Font> value) {
        if(hasProperty(key)) checkProperty(key, TYPE_FONT);
        else properties[key] = Property();
        properties[key] = Property();
        properties[key].propFont = value;
        properties[key].type = TYPE_FONT;
    }
    
    
    
    float Theme::getNumberProperty(const std::string key) {
        return findProperty(key, TYPE_FLOAT).propFloat;
    }
    bool Theme::getBoolProperty(const std::string key) {
        return findProperty(key, TYPE_BOOL).propBool;
    }
    sf::Color Theme::getColorProperty(const std::string key) {
        return findProperty(key, TYPE_COLOR).propColor;
    }
    sf::Vector2f Theme::getVector2fProperty(const std::string key) {
        return findProperty(key, TYPE_VECTOR2F).propVector2f;
    }
    ORIGIN Theme::getOriginProperty(const std::string key) {
        return findProperty(key, TYPE_ORIGIN).propOrigin;
    }
    sf::String Theme::getStringProperty(const std::string key) {
        return findProperty(key, TYPE_STRING).propString;
    }
    std::shared_ptr<sf::Font> Theme::getFontProperty(const std::string key) {
        return findProperty(key, TYPE_FONT).propFont;
    }
    std::shared_ptr<void> Theme::getSharedProperty(const std::string key) {
        return findProperty(key, TYPE_VOID).propVoid;
    }
    
    
    Theme::TYPE Theme::getPropertyType(const std::string key) {
        checkPropertyExists(key);
        return properties[key].type;
    }
    void Theme::removeProperty(const std::string key) {
        checkPropertyExists(key);
        properties.erase(key);
    }
    bool Theme::hasProperty(const std::string key) {
        return properties.find(key) != properties.end();
    }
    
    void Theme::checkProperty(const std::string key, TYPE type) {
        checkPropertyExists(key);
        checkPropertyType(key, type);
        checkObjectExists(parseString(key.c_str()).object);
    }
    void Theme::checkObjectExists(const std::string object) {
        if(parentObjectType.find(object) == parentObjectType.end()) {
            throw std::string("Error, object '") + object + std::string("' does not exist!");
        }
    }
    void Theme::checkPropertyExists(const std::string key) {
        if(!hasProperty(key))
            throw std::string("Property ") + key + std::string("does not exist!");
    }
    void Theme::checkPropertyType(const std::string key, TYPE type) {
        if(properties[key].type != type)
            throw std::string("Property '") + key + std::string("' is of wrong type '") + typeToString(properties[key].type) + std::string("' instead of the proper '") + typeToString(type) + std::string("'\n");
    }
    
    Theme::Property &Theme::findProperty(const std::string key, TYPE type) {
        ParsedString parsed = parseString(key.c_str());
        
        // must be called before the next line because the next line will create it if it does not exist(bad, bad, bad!)
        checkObjectExists(parsed.object);
        ObjectType *object_type = &parentObjectType[parsed.object];
        do {
            std::string new_key = object_type->name + "." + parsed.property;
            if(hasProperty(new_key)) {
                checkPropertyType(new_key, type);
                return properties[new_key];
            }
            object_type = object_type->parent;
        } while(object_type != nullptr);
        throw std::string("Property '") + key + std::string("' could not be found");
    }
}