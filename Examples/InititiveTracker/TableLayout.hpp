#pragma once
#include <SUI/BoxLayout.hpp>
#include <SUI/Text.hpp>
#include <SUI/TextField.hpp>
#include <SUI/FrameLayout.hpp>
#include <SUI/Button.hpp>
#include <vector>
#include <fstream>

struct ThemeReader {
    ThemeReader() {
        file.open("Theme.txt", std::ios::in);
    }
    void setWidgetProps(sui::Widget *w, const std::string section) {
        file.clear();
        file.seekg(0, std::ios::beg);
        
        std::string line;
        while(getline(file, line)) {
            if(line == "[" + section + "]")
                break;
        };
        while(getline(file, line)) {
            if(line.size() == 0) continue;
            if(line[0] == '#') continue; // comment
            if(line[0] == '[') break; // new section
            std::string propName;
            std::string propValue;
            bool foundName = false;
            for(char c : line) {
                if(c == '=') {
                    foundName = true;
                } else if(foundName) {
                    propValue += c;
                } else {
                    propName += c;
                }
            }
            assignProp(w, propName, propValue);
        }
    }
    void assignProp(sui::Widget *w, std::string name, std::string value) {
        if(name == "font") {
            fonts[c_font] = sf::Font();
            fonts[c_font].loadFromFile(value);
            w->setProperty(name, sui::Property::make(&fonts[c_font]));
            c_font++;
        } else if(name == "outlineThickness" || name == "fontSize" || name == "textPadding" || name == "padding") {
            w->setProperty(name, sui::Property::make<float>(std::stof(value)));
        } else if(name == "fillColor" || name == "fillColorHovered" || name == "fillColorClicked" ||
                  name == "textColor" || name == "outlineColor" || name == "normalColor" || name == "errorColor") {
            sf::Color color;
            unsigned int i = 0;
            auto getV = [&i, &value](){
                std::string v = "";
                for(;i < value.size(); i++) {
                    char c = value[i];
                    if(c == ',') {
                        i++;
                        break;
                    }
                    v += c;
                }
                return v;
            };
            color.r = std::stof(getV());
            color.g = std::stof(getV());
            color.b = std::stof(getV());
            color.a = std::stof(getV());
            w->setProperty(name, sui::Property::make(color));
        } else if(name == "textAlignX" || name == "textAlignY") {
            sui::ORIGIN o;
            if(value == "start") { o = sui::ORIGIN_START;}
            else if(value == "end") {o = sui::ORIGIN_END;}
            else {o = sui::ORIGIN_MIDDLE;}
            w->setProperty(name, sui::Property::make(o));
        }
    }
    ~ThemeReader() {
        file.close();
    }
    sf::Font fonts[10]; // just in case :)
    int c_font = 0;
    std::ifstream file;
};




class BoxLayout : public sui::BoxLayout {
public:
    BoxLayout();
    
protected:
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void onUpdate(const bool posChanged, const bool sizeChanged);
    void setLineColors(sf::Color color);
    
    sf::Color mColor;
    std::vector<sf::RectangleShape> mLines;
    bool mFirstUpdate;
};


// not well encapselated. Should not allow external use of addChild or removeChild
// because it assumes too much about them and will most likely result in a crash
class TableLayout : public BoxLayout {
public:
    TableLayout(ThemeReader &reader);
    
    // will likely be handled internally with an X button
    // void removeColumn(unsigned int index);
    void sort();
    
    void setAutoSort(bool autoSort) {this->shouldAutoSort = autoSort;}

    float getUsedWidth();
    std::function<void(sui::Widget *)> addedTextField;
protected:
    virtual void onInput(sf::Event e);
    virtual void onUpdate(const bool posChanged, const bool sizeChanged) {
        for(auto &d : delete_me) {
            delete d;
        }
        delete_me.clear();
        if(shouldaddColumn) {
            addColumn();
            onChanged();
            shouldaddColumn = false;
        }
        BoxLayout::onUpdate(posChanged, sizeChanged);
    }
private:
	bool shouldSort = false;
	bool shouldaddColumn = false;
    void addColumn();
    const static int column_width = 100;
    const static int button_width = 50;
    const static int text_width = 120;
    sui::Button *add_button;
    bool shouldAutoSort;
    void onChanged() {
        const sui::Property &p = getProperty("onChanged");
        if(p) p.asFunc()();
    }
    
    void autoSort(sui::TextField *textfield);
    ThemeReader &mReader;
    std::vector<sui::Widget *> delete_me;
};

