#ifndef CAMERA_TRANSLATOR
#define CAMERA_TRANSLATOR

#include "c4d.h"
#include <vector>

class CameraTranslator{
public:
	//CameraTranslator(CameraObject* c, BaseDocument* doc);
	//void CacheTransformation(BaseDocument* doc);
	//void EmitCamera();

private:
	CameraObject* camera;
	std::vector<Matrix> transform_samples;
	std::vector<Int32> fov_samples;
};

#endif