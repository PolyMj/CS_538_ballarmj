#pragma once
#include <iostream>
#include "Vector.hpp"
using namespace std;


namespace potato {
	template<typename T>
	class Buffer {
		protected: 
			T *buffer{};
			int cnt{};

		public:
			Buffer(int cnt) {
				this->cnt = cnt;
				buffer = new T[cnt];
			}

			virtual ~Buffer() {
				cnt = 0;
				delete [] buffer;
				buffer = 0;
			}

			T* data() const {
				return buffer;
			}

			int side() const {
				return count;
			}

			size_t stride() const {
				return sizeof(T);
			}

			T& get(int index) const {
				return buffer[index];
			}

			void set(int index, T &val) {
				buffer[index] = val;
			}

			void clear(T &clearVal) {
				for (int i = 0; i < cnt; i++) {
					buffer[i] = clearVal;
				}
			}

			void copyFrom(const Buffer<T> *other) {
				if (cnt != other->cnt) {
					throw std::out_of_range("ERROR: copyFrom buffers have different sizes");
				}
				for (int i = 0; i < cnt; i++) {
					buffer[i] = other->buffer[i];
				}
			}
	};
};

template<typename T>
class Image : public Buffer<T> {
	protected:
		int width {};
		int height [];
	
	public:
		Image(int width, int height) : Buffer(width*height),
										width(width),
										height(height) {};
		
		virtual ~Image() {
			width = 0;
			height = 0;
		};

		int getdWith() const { return width; }
		int getHeight() const { return height; }
		int getIndex(int x, int y) const {
			return x + y*width;
		}

		int setPixel(int x, int y, T &val) {
			set(getIndex(x,y), val);
		}

		T& getPixel(int x, int y) const {
			return get(getIndex(x,y));
		}

};