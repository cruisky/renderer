#pragma once

#include "txbase/math/sample.h"
#include "Core/RayTracer.h"

namespace TX{
	class PathTracing : public RayTracer {
	public:
		PathTracing(int maxdepth = 6);
		~PathTracing(){}
		void BakeSamples(const Scene *scene, const CameraSample *samplebuf);
	protected:
		Color Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf);
	private:
		static const int SAMPLE_DEPTH;
		std::vector<SampleOffset> light_samples_;
		std::vector<SampleOffset> bsdf_samples_;
		std::vector<SampleOffset> scatter_samples_;
	};
}
