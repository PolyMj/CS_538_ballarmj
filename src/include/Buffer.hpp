#pragma once

#include <algorithm>
#include <cstring>
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

            void set(int index, const T val) {
                buffer[index] = val;
            };

            T get(int index) const {
                return buffer[index];
            };       

            void clear(const T clearVal) {                
                T *current = buffer;                
                for(int i = 0; i < cnt; i++) {                    
                    (*current) = clearVal;
                    current++;
                }
            };  

            void clear() {
                memset(buffer, 0, cnt*sizeof(T));
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

            template<typename U, typename S>
            void copyFrom(const Buffer<U> *other, S alpha, S beta) {
                if(cnt != other->size()) 
                    throw std::out_of_range("ERROR: Unequal sizes! " 
                                            + to_string(cnt) + " vs. " 
                                            + to_string(other->size()));
            
                for(int i = 0; i < cnt; i++) {
                    auto val = other->get(i);
                    val = val*alpha;
                    val = val + beta;                    
                    buffer[i] = T(val);
                }
            };
    };

    template<typename T>
    class Image : public Buffer<T> {
        protected:
            int width {};
            int height {};
        public:            
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

            void setPixel(int x, int y, const T &val) {
                Buffer<T>::set(getIndex(x,y), val);                
            };

            T& getPixel(int x, int y) const {
                return Buffer<T>::get(getIndex(x,y));
            };
    };
};
