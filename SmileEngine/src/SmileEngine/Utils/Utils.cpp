#include "smpch.h"
#include "Utils.h"

namespace Smile::Utils
{
	bool CompareFloats(float f1, float f2, float epsilon)
	{
		float deltaF = f1 - f2;
		return (deltaF < epsilon) && (deltaF > -epsilon);
	}
}