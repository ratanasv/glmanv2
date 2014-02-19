#pragma once



class GLSLAttributeBinder {
public:
	virtual ~GLSLAttributeBinder() {};
	virtual void EnablePositionAttribute(const int vecLength = 4) = 0;
	virtual void EnableNormalAttribute(const int vecLength = 4) = 0;
	virtual void EnableColorAttribute(const int vecLength = 4) = 0;
	virtual void EnableTexCoordAttribute(const int vecLength = 4) = 0;
};