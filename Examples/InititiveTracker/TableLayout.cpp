#include "TableLayout.hpp"
#include <algorithm>
#include <string>

BoxLayout::BoxLayout() : sui::BoxLayout() {
    mFirstUpdate = true;
}
void BoxLayout::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    sui::BoxLayout::onDraw(target, states);
    for(auto &l : mLines) {
        target.draw(l, states);
    }
}

void BoxLayout::setLineColors(sf::Color color) {
    mColor = color;
    for(auto &l : mLines) {
        l.setFillColor(color);
    }
}

void BoxLayout::onUpdate(const bool posChanged, const bool sizeChanged) {
    sui::BoxLayout::onUpdate(posChanged, sizeChanged);
    const bool amount_changed = hasPropChanged("childAdded") || hasPropChanged("childRemoved") || hasPropChanged("padding") || mFirstUpdate;
    if(amount_changed) {
        mLines.clear();
        mFirstUpdate = false;
    }
    
    // items flow horizontally so lines are vertical between them
    if(getDirection() == HORIZONTAL) {
        // place bar after every item except the last one
        if(!getChildren().empty())
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            if(sizeChanged || amount_changed) {
                const float width = getPadding();
                if(amount_changed) {
                    mLines.push_back(sf::RectangleShape(sf::Vector2f(width, getSize().y)));
                } else {
                    mLines[i].setSize(sf::Vector2f(width, getSize().y));
                }
            }
            if(posChanged || amount_changed) {
                const float place = c->getGlobalBounds().left + c->getSize().x;
                mLines[i].setPosition(sf::Vector2f(place, getGlobalBounds().top));
            }
            
        }
    } else {
        // place bar after every item except the last one
        if(!getChildren().empty())
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            if(sizeChanged || amount_changed) {
                const float height = getPadding();
                if(amount_changed) {
                    mLines.push_back(sf::RectangleShape(sf::Vector2f(getSize().x, height)));
                } else {
                    mLines[i].setSize(sf::Vector2f(getSize().x, height));
                }
            } 
            if(posChanged || amount_changed) {
                const float place = c->getGlobalBounds().top + c->getSize().y;
                mLines[i].setPosition(sf::Vector2f(getGlobalBounds().left, place));
            }
        }
    }
    
    // not being called on table's vboxes
    if(hasPropChanged("outlineColor") || amount_changed) {
        setLineColors(getProperty("outlineColor").as<sf::Color>());
    }
}





TableLayout::TableLayout(ThemeReader &reader) : BoxLayout(), mReader(reader) {
    setProperty("direction", sui::BoxLayout::HORIZONTAL);
    
    BoxLayout *vbox = new BoxLayout();
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    vbox->setProperty("boxSize", sui::Property::make(sf::Vector2f(text_width,1)));
    vbox->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    vbox->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
    addChild(vbox);
    
    auto addText = [&vbox, this](sf::String &&str) {
        sui::FrameLayout *frame = new sui::FrameLayout();
        sui::Text *text = new sui::Text();
        mReader.setWidgetProps(text, "tableText");
        text->setProperty("text", str);
        text->setProperty("frameSize", sf::Vector2f(1,.6));
        text->setProperty("framePos", sf::Vector2f(0,.5));
        text->setProperty("scaleTypeX", sui::BoxLayout::RELATIVE);
        text->setProperty("scaleTypeY", sui::BoxLayout::RELATIVE);
        text->setProperty("posTypeX", sui::BoxLayout::RELATIVE);
        text->setProperty("posTypeY", sui::BoxLayout::RELATIVE);
        
        frame->addChild(text);
        vbox->addChild(frame);
    };
    
    addText("Name");
    addText("Initiative Roll");
    addText("Initiative Mod");
    addText("Health");
    
    
    add_button = new sui::Button();
    addChild(add_button);
    mReader.setWidgetProps(add_button, "addButton");
    add_button->setProperty("text", sui::Property::make<sf::String>("+"));
    add_button->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    add_button->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    add_button->setProperty("outlineThickness", sui::Property::make(0.f));
    add_button->setProperty("boxSize", sui::Property::make(sf::Vector2f(button_width, 1)));
    add_button->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    add_button->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
    add_button->setProperty("onClickedDown", sui::Property::makeFunc([this](){
        // if we called addColumn directly then the button would be moved to the
        // next onInput iteration and be called again which would call this
        // again which would call it again... death loop!!!!!! *evil laugh*
        shouldaddColumn = true;
    }));

    shouldAutoSort = false;
}

void TableLayout::onInput(sf::Event e) {
	BoxLayout::onInput(e);
	if(shouldSort) {
		sort();
		shouldSort = false;
	}
}

void TableLayout::addColumn() {
    BoxLayout *vbox = new BoxLayout();
    insertChild(vbox, getChildren().size()-1);
    vbox->setProperty("scaleTypeY", sui::BoxLayout::RELATIVE);
    vbox->setProperty("boxSize", sf::Vector2f(column_width,1));
    vbox->setProperty("direction", sui::BoxLayout::VERTICAL);
    
    for(unsigned int i = 0;i < 4; i++) {
        sui::TextField *textfield = new sui::TextField();
        mReader.setWidgetProps(textfield, "tableTextField");
        textfield->setProperty("outlineThickness", 0.f);
        textfield->setProperty("fillColor", textfield->getProperty("normalColor"));
        if(i == 1 || i == 2)
            textfield->setProperty("onChanged", sui::Property::makeFunc([this, textfield](){
                autoSort(textfield);
            }));
        if(i == 0) {
            textfield->setProperty("scaleTypeX", sui::BoxLayout::RELATIVE);
            textfield->setProperty("scaleTypeY", sui::BoxLayout::RELATIVE);
            textfield->setProperty("boxSize", sf::Vector2f(1,1));
        
            sui::BoxLayout *hbox = new sui::BoxLayout();
            hbox->setProperty("direction", sui::BoxLayout::HORIZONTAL);
            hbox->setProperty("padding", 0.f);
            hbox->addChild(textfield);
            auto *button = hbox->addChild(new sui::Button());
            button->setProperty("fontSize", 12.f);
            button->setProperty("text", sf::String("X"));
            button->setProperty("scaleTypeX", sui::BoxLayout::ABSOLUTE);
            button->setProperty("scaleTypeY", sui::BoxLayout::RELATIVE);
            button->setProperty("boxSize", sf::Vector2f(30,1));
            button->setProperty("onClickedDown", sui::Property::makeFunc([this, vbox](){
                removeChild(vbox);
                onChanged();
                
                delete_me.push_back(vbox);
            }));
            
            vbox->addChild(hbox);
        } else {
            vbox->addChild(textfield);
        }
    }
}
void TableLayout::autoSort(sui::TextField *textfield) {
    textfield->setProperty("fillColor", textfield->getProperty("normalColor"));
    sf::String str = textfield->getProperty("text").as<sf::String>();
    for(unsigned int i = 0; i < str.getSize(); i++) {
        if(str[i] == L'+' || str[i] == L'-') {
            if(i == 0) {
                continue;
            } else {
                textfield->setProperty("fillColor", textfield->getProperty("errorColor"));
                break;
            }
        }
        if(str[i] != L'1' && str[i] != L'2' && str[i] != L'3' && str[i] != L'4' && str[i] != L'5' && str[i] != L'6' && str[i] != L'7' && str[i] != L'8' && str[i] != L'9' && str[i] != L'0') {
            textfield->setProperty("fillColor", textfield->getProperty("errorColor"));
            break;
        }
    }
    if(str.getSize() > 9) {
        textfield->setProperty("fillColor", textfield->getProperty("errorColor"));
    }
    if(shouldAutoSort) {
		// we don't want to call sort immediately
		// because it may move this over to the right
		// where the next onInput iterator will call
		// it again and we get double input!
        shouldSort = true;
    }
}
void TableLayout::sort() {
    std::sort(mChildren.begin()+1, mChildren.end()-1,
        [](sui::Widget *i, sui::Widget *j){
            const sui::Widget *text_field_roll_i = static_cast<sui::BoxLayout *>(i)->getChildren()[1];
            const sui::Widget *text_field_mod_i = static_cast<sui::BoxLayout *>(i)->getChildren()[2];
            const std::string roll_string_i = text_field_roll_i->getProperty("text").as<sf::String>().toAnsiString();
            const std::string mod_string_i = text_field_mod_i->getProperty("text").as<sf::String>().toAnsiString();
            int roll_i;
            int mod_i;
            try {
                mod_i = std::stoi(mod_string_i);
            } catch(std::invalid_argument arg) {
                mod_i = 0;
            } catch(std::out_of_range rng) {
                return false;
            }
            try {
                roll_i = std::stoi(roll_string_i)+mod_i;
            } catch(std::invalid_argument arg) {
                return false;
            } catch(std::out_of_range rng) {
                return false;
            }
            
            const sui::Widget *text_field_roll_j = static_cast<sui::BoxLayout *>(j)->getChildren()[1];
            const sui::Widget *text_field_mod_j = static_cast<sui::BoxLayout *>(j)->getChildren()[2];
            const std::string roll_string_j = text_field_roll_j->getProperty("text").as<sf::String>().toAnsiString();
            const std::string mod_string_j = text_field_mod_j->getProperty("text").as<sf::String>().toAnsiString();
            int roll_j;
            int mod_j;
            try {
                mod_j = std::stoi(mod_string_j);
            } catch(std::invalid_argument arg) {
                mod_j = 0;
            } catch(std::out_of_range rng) {
                return false;
            }
            try {
                roll_j = std::stoi(roll_string_j)+mod_j;
            } catch(std::invalid_argument arg) {
                return false;
            } catch(std::out_of_range rng) {
                return false;
            }
            
            if(roll_i > roll_j) {
                return true;
            } else if(roll_i < roll_j) {
                return false;
            } else {
                if(mod_i > mod_j) {
                    return true;
                } else if(mod_i < mod_j) {
                    return false;
                } else {
                    return false; //keep order if tied cause why move?
                }
            }
        });
    
    // we just changed pointers which doesn't say anything to update. force update.
    // this property doesn't actually do anything. "childAdded" is a special signal
    // recieved in onUpdate via hasPropChanged whenever a child was added and usually
    // requires a complete recomputation like what we want here.
    setProperty("childAdded", true);
    update();
}

float TableLayout::getUsedWidth() {
    return (getChildren().size()-2)*column_width + button_width + text_width;
}


