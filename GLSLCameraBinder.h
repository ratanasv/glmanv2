#pragma once

namespace Angel {
	template<class T = float> class vec3;
}

class GLSLCameraBinder {
public:
	virtual ~GLSLCameraBinder() {};
	virtual void LookAt(const Angel::vec3<>& eye, const Angel::vec3<>& at, 
		const Angel::vec3<>& up) = 0;
	virtual void Ortho(float left, float right, float bottom, float top, 
		float zNear, float zFar) = 0;
	virtual void Perspective(float fovy, float ratio, float zNear, float zFar) = 0;
	virtual void Rotate(float x, float y, float z) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void ClearProjection() = 0;
	virtual void ClearModelView() = 0;
};