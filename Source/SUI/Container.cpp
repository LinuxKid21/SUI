#include <SUI/Container.hpp>

namespace sui {
    Container::Container(Theme &theme) : Widget(theme) {
        lockLocation(false);
        lockSize(false);
    }
    Container::~Container() {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            delete mChildren[i];
        }
    }
    
    bool Container::handleInput(sf::Event e) {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            mChildren[i]->handleInput(e);
        }
        return false;
    }
    
    void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(auto &child : mChildren) {
            target.draw(*child, states);
        }
    }
    
    Widget *Container::addChild(Widget *widget) {
        if(widget->mParent) {
            return nullptr;
        }
        mChildren.push_back(widget);
        widget->mParent = this;
        return widget;
    }
    
    Widget *Container::insertChild(Widget *widget, unsigned int pos) {
        if(widget->mParent) {
            return nullptr;
        }
        if(pos > mChildren.size()) {
            throw "Error, index passed in is invalid!";
        }
        mChildren.insert(mChildren.begin() + pos, widget);
        widget->mParent = this;
        return widget;
    }
    
    Widget *Container::removeChild(Widget *widget) {
        for(int i = 0;i < mChildren.size(); i++) {
            if(mChildren[i] == widget) {
                mChildren.erase(mChildren.begin()+i);
                widget->mParent = nullptr;
                return widget;
            }
        }
        return nullptr;
    }

    void Container::layoutChanged() {
        Widget::layoutChanged();
        for(auto &child : mChildren) {
            child->layoutChanged();
        }
    }
    
    bool Container::childrenCanMove() {
        return !mLocLocked;
    }
    bool Container::childrenCanResize() {
        return !mSizeLocked;
    }
    
    void Container::lockLocation(bool locked) {
        mLocLocked = locked;
    }
    void Container::lockSize(bool locked) {
        mSizeLocked = locked;
    }
    
    bool Container::isChild(Widget *widget) {
        return widget->mParent == this;
    }
    
    
    std::vector<Widget *> &Container::getChildren() {
        return mChildren;
    }

    void **Container::getChildCustomData(Widget *child) {
        return &child->mContainerData;
    }
}