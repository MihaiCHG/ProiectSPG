#pragma once

#include "Object3d.h"

namespace myGame {
	class DynamicObject : public Object3d {

	private:
		

	public:
		double v;
		float mass;
		float orentation;
		bool moving;
		DynamicObject(float _x, float _y, float _z) : Object3d(_x, _y, _z) {
			v = 0.0;
			orentation = 0;
		}

		~DynamicObject(){}

		virtual void Update() = 0;

	};
	
}