#include <SUI/Container.hpp>

namespace sui {
    Container::Container() : Widget() {
        lockLocation(true);
        lockSize(true);
    }
    Container::~Container() {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            delete mChildren[i];
        }
    }
    
    void Container::onInput(sf::Event e) {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            mChildren[i]->onInput(e);
        }
    }
    
    void Container::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(auto &child : mChildren) {
            child->onDraw(target, states);
        }
    }

    void Container::_onUpdate() {
        lockLocation(false);
        lockSize(false);
        onUpdate();
        for(auto &child : mChildren) {
            child->_onUpdate();
        }
        lockLocation(true);
        lockSize(true);
    }
    void Container::onPropertyChanged(const std::string key) {
        for(unsigned int i = 0; i < mChildren.size(); i++) {
            const Widget *c = mChildren[i];
            // recursively alert children that the property changed
            // if if they don't have that property defined locally
            // (because that property overrides this one for it so
            // it didn't really change for it)
            if(c->mProperties.find(key) == c->mProperties.end()) {
                mChildren[i]->onPropertyChanged(key);
            }
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
    
    
    const std::vector<Widget *> &Container::getChildren() {
        return mChildren;
    }

    void **Container::getChildCustomData(Widget *child) {
        return &child->mContainerData;
    }
}