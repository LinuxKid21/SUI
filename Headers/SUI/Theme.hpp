#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include <algorithm>
#include <iostream>

namespace sui {
    enum ORIGIN {
        ORIGIN_START = 0,
        ORIGIN_MIDDLE,
        ORIGIN_END
    };
    
    class Theme {
    public:
        void loadDefaults(const char *font_file);
        
        enum TYPE {
            TYPE_FLOAT = 0,
            TYPE_BOOL,
            TYPE_COLOR,
            TYPE_VECTOR2F,
            TYPE_ORIGIN,
            TYPE_STRING,
            TYPE_FONT,
            TYPE_VOID
        };
        std::string typeToString(TYPE type) {
            #define CHECK(e) if(type == e) return #e
            CHECK(TYPE_FLOAT);
            CHECK(TYPE_BOOL);
            CHECK(TYPE_COLOR);
            CHECK(TYPE_VECTOR2F);
            CHECK(TYPE_ORIGIN);
            CHECK(TYPE_STRING);
            CHECK(TYPE_FONT);
            CHECK(TYPE_VOID);
            #undef ENUM_TO_STRING
        }
        

        static void registerType(const std::string new_type, const std::string parent);
        
        void setNumberProperty(const std::string key, float value);
        void setBoolProperty(const std::string key, bool value);
        void setColorProperty(const std::string key, sf::Color value);
        void setVector2fProperty(const std::string key, sf::Vector2f value);
        void setOriginProperty(const std::string key, ORIGIN value);
        void setStringProperty(const std::string key, sf::String value);
        void setFontProperty(const std::string key, std::shared_ptr<sf::Font> value);
        template<class T>
        void setVoidProperty(const std::string key, T *value) {
            if(hasProperty(key)) checkProperty(key, TYPE_VOID);
            else properties[key] = Property();
            properties[key].propVoid = std::shared_ptr<void>(value);
            properties[key].type = TYPE_VOID;
        }

        float getNumberProperty(const std::string key);
        bool getBoolProperty(const std::string key);
        sf::Color getColorProperty(const std::string key);
        sf::Vector2f getVector2fProperty(const std::string key);
        ORIGIN getOriginProperty(const std::string key);
        sf::String getStringProperty(const std::string key);
        std::shared_ptr<sf::Font> getFontProperty(const std::string key);
        template<class T>
        T getVoidProperty(const std::string key) {
            return static_cast<T>(findProperty(key, TYPE_VOID).propVoid.get());
        }
        std::shared_ptr<void> getSharedProperty(const std::string key);
        
        TYPE getPropertyType(const std::string key);
        void removeProperty(const std::string key);
        bool hasProperty(const std::string key);
    private:
        struct ParsedString {
            std::string object;
            std::string property;
        };
        struct ObjectType {
            ObjectType() {
                id = current_id++;
            }
            std::string name;
            int id;
            ObjectType *parent;
        private:
            static int current_id;
        };
        
        static ParsedString parseString(const char *c) {
            ParsedString parsed_string;
            bool foundPeriod = false;
            while(*c != '\0') {
                if(!foundPeriod) {
                    if(*c == '.') {
                        foundPeriod = true;
                    } else {
                        parsed_string.object.append(1, *c);
                    }
                } else {
                    if(*c == '.') {
                        throw std::string("Error! second '.' invalid in string ") + std::string(c);
                    } else {
                        parsed_string.property.append(1, *c);
                    }
                }
                c++;
            }
            if(!foundPeriod) {
                throw std::string("no period found in ") + std::string(c) + std::string("! input should be in the form 'object.property'");
            }
            return parsed_string;
        }
        
        struct Property {
            union {
                float propFloat;
                bool propBool;
                sf::Color propColor;
                sf::Vector2f propVector2f;
                ORIGIN propOrigin;
                sf::String propString;
                std::shared_ptr<sf::Font> propFont;
                std::shared_ptr<void> propVoid;
            };
            TYPE type;
            static int current_id;
            int id;
            Property(){
                id = current_id++;
            }
            void operator=(const Property &other) {
                id = other.id; // map requires this but do not use it for anything else because it's incomplete!
            }
            ~Property(){
                ; // the union requires this. find out why 'cause I'm clueless
            }
        };

        void checkProperty(const std::string key, TYPE type);
        void checkPropertyType(const std::string key, TYPE type);
        void checkObjectExists(const std::string object); // this takes just the object (the one before the dot)
        void checkPropertyExists(const std::string object); // this takes just the object (the one before the dot)
        Property &findProperty(const std::string key, TYPE type);
        
        
        static std::map <std::string, ObjectType> parentObjectType;
        std::map <std::string, Property> properties;
    };
}

