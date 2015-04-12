#include "stdafx.h"
#include "Math.h"
#include "Ray.h"
#include "Intersection.h"
#include "Shape.h"

namespace Cruisky{
	namespace RayTracer
	{
		bool UnitSphere::Intersect(const Ray& lcr) const {
			float e2 = LengthSqr(lcr.origin);
			float d2 = LengthSqr(lcr.dir);
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max) && delta_quarter > 0){
				lcr.t_max = t;
				return true;
			}
			return false;
		}

		void UnitSphere::PostIntersect(const Ray& lcr, LocalGeo& geo) const {
			geo.point = lcr.origin + lcr.dir * lcr.t_max;	// local point
			geo.normal = geo.point;// .Normalize();
		}

		bool UnitSphere::Occlude(const Ray& lcr) const {
			float e2 = LengthSqr(lcr.origin);
			float d2 = LengthSqr(lcr.dir);
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			return delta_quarter > 0.f && Math::InBounds(t, lcr.t_min, lcr.t_max);
		}

		bool UnitPlane::Intersect(const Ray& lcr) const {
			float t = -lcr.origin.z / lcr.dir.z;
			Vector3 point = lcr.origin + t * lcr.dir;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max) &&
				Math::InBounds(point.x, -0.5f, 0.5f) &&
				Math::InBounds(point.y, -0.5f, 0.5f)) {
				lcr.t_max = t;
				return true;
			}
			return false;
		}

		void UnitPlane::PostIntersect(const Ray& lcr, LocalGeo& geo) const {
			geo.point = lcr.origin + lcr.dir * lcr.t_max;	// local point
			geo.normal = Vector3(0.f, 0.f, 1.f);// .Normalize();
		}

		bool UnitPlane::Occlude(const Ray& lcr) const {
			float t = -lcr.origin.z / lcr.dir.z;
			return Math::InBounds(t, lcr.t_min, lcr.t_max);
		}
	}
}