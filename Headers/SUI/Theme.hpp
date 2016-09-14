#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include <algorithm>
#include <exception>
#include <iostream>

namespace sui {
    struct nullPropertyAccessed : public std::exception
    {
        std::string message;
        nullPropertyAccessed(const std::string name) {
            message = std::string("null property '") + name + "' accessed!";
        }
        const char *what() const throw() {
            return message.c_str();
        }
    };
    enum ORIGIN {
        ORIGIN_START = 0,
        ORIGIN_MIDDLE,
        ORIGIN_END
    };

    class Widget;
    class Property {
    private:
        struct PropertyImplBase {
            virtual ~PropertyImplBase(){}
            virtual PropertyImplBase *copy() = 0;
        };
        //handle by value
        template<class T>
        struct PropertyImpl : public PropertyImplBase {
            PropertyImpl(T value) : data(value) {}
            PropertyImplBase *copy() {
                return new PropertyImpl<T>(data);
            }
            ~PropertyImpl() {}
            T data;
        };
        
        // handle references
        template<class T>
        struct PropertyImpl<T&> : public PropertyImplBase {
            PropertyImpl(T value) : data(value) {}
            PropertyImplBase *copy() {
                return new PropertyImpl<T>(data);
            }
            ~PropertyImpl() {}
            T data;
        };

        // only one property can have the same impl at the same time and assignment must be const
        // this is a somewhat nasty hack but I can't think of a better solution
        PropertyImplBase *impl;
    public:
        template<class T>
        static const Property make(T &&value) {
            Property p;
            p.impl = new PropertyImpl<T>(value);
            return p;
        }
        static const Property makeFunc(std::function<void()> &&value) {
            return make<std::function<void()> >(std::move(value));
        }
        
        Property() {
            impl = nullptr;
        }
        Property(const Property &other) {
            if(other.impl)
                this->impl = other.impl->copy();
            else
                this->impl = nullptr;
        }
        void operator=(const Property &other) {
            if(other.impl)
                this->impl = other.impl->copy();
            else
                this->impl = nullptr;
        }
        
        Property(Property &&other) {
            this->impl = other.impl;
            other.impl = nullptr;
        }
        void operator=(Property &&other) {
            this->impl = other.impl;
            other.impl = nullptr;
        }
        
        explicit operator bool() const {
            return impl != nullptr;
        }
        ~Property() {
            if(impl)
                delete impl;
        }
        template<class T>
        T as() const {
            const PropertyImpl<T> *t = static_cast<PropertyImpl<T> *>(impl);
            return static_cast<T>(t->data);
        }
        std::function<void()> asFunc() const {
            return as<std::function<void()> >();
        }
        
        template<class T>
        T *asPointer() const {
            PropertyImpl<T> *t = static_cast<PropertyImpl<T> *>(impl);
            return static_cast<T *>(&t->data);
        }
        
        template<class T>
        void set(T value) {
            const PropertyImpl<T> *t = static_cast<PropertyImpl<T> *>(impl);
            static_cast<T>(t->data) = value;
        }
    };
}

