#pragma once
#include "Core/Light.h"

namespace TX{
	class DirectionalLight : public Light {
	public:
		DirectionalLight(const Color& intensity, int sample_count = 1);
		DirectionalLight(const Color& intensity, const Vec3& dir, int sample_count = 1);

		void SampleDirect(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
		float Pdf(const Vec3& pos, const Vec3& dir) const;
		bool IsDelta() const;

		virtual Color Intensity() const;
		virtual Vec4 Position() const;
		virtual Vec3 Direction() const;
	public:
		Color intensity;
		Vec3 dir;
	};
}
