#pragma once
#include "txbase/shape/obj.h"
#include "txbase/opengl/opengl.h"
#include "txbase/shader/phong.h"

#include "Core/Primitive.h"

namespace TX {
	class ObjViewer {
	public:
		struct Obj {
			Obj(const Primitive& prim) :
				prim(prim) {
			}
			GL::Mesh mesh;
			const Primitive& prim;
		};
	public:
		Phong::LightSource lightSource;
	public:
		ObjViewer(const Camera& camera, const Scene& scene);
	public:
		void Render(const Primitive *selected);
	private:
		Phong											programPhong;
		ShaderProgram									programSingleColor;
		std::vector<Obj>								objs;
		const Camera&									camera;
		const Scene&									scene;
	};
}
