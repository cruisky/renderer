#pragma once

#include "fwddecl.h"

namespace Cruisky{
	namespace RayTracer{
		class Film {
		public:
			Film(int width, int height){ Resize(width, height); }
			~Film(){ Release(); }

			//TODO
			//void Commit(Sample& sample, Color& color);

			inline int GetWidth() const { return width_; }
			inline int GetHeight() const { return height_; }
			void Resize(int width, int height);
			const Color *GetPixels() const;
			
			void Release();

		private:
			int width_, height_;
			unique_ptr<Color[]> pixels_;
		};
	}
}