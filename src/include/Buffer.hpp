#pragma once

#include "Vector.hpp"
using namespace std;

namespace potato {
    template<typename T>
    class Buffer {
        protected:
            T *buffer {};
            int cnt {};
        public:        
            Buffer(int cnt) {
                this->cnt = cnt;
                buffer = new T[cnt];
            };
            virtual ~Buffer() {
                cnt = 0;
                delete [] buffer;
            };

            T* data() const {
                return buffer;
            };

            int size() const {
                return cnt;
            };

            size_t stride() const {
                return sizeof(T);
            };

            void set(int index, T val) {
                buffer[index] = val;
            };

            T& get(int index) const {
                return buffer[index];
            };       

            void clear(T clearVal) {
                for(int i = 0; i < cnt; i++) {
                    buffer[i] = clearVal;
                }
            };   

            void copyFrom(const Buffer<T> *other) {
                if(cnt != other->cnt) 
                    throw std::out_of_range("ERROR: Unequal sizes! " 
                                            + to_string(cnt) + " vs. " 
                                            + to_string(other->cnt));
            
                for(int i = 0; i < cnt; i++) {
                    buffer[i] = other->buffer[i];
                }
            };
    };

    template<typename T>
    class Image : public Buffer<T> {
        protected:
            int width {};
            int height {};
        public:
            using Buffer<T>::set;
            using Buffer<T>::get;
            using Buffer<T>::clear;

            Image(int width, int height) : Buffer<T>::Buffer(width*height), width(width), height(height) {};
            virtual ~Image() {
                width = 0;
                height = 0;
            };

            int getWidth() const { return width; };
            int getHeight() const { return height; };

            int getIndex(int x, int y) const {
                return (x + y*width);
            };

            void setPixel(int x, int y, T val) {
                set(getIndex(x,y), val);                
            };

            T& getPixel(int x, int y) const {
                return get(getIndex(x,y));
            };
    };
};
