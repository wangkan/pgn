#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

struct Float3;
struct Float4;
struct Float4x3;
class SkeletonTemplate;

class SkeletonTemplateFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE SkeletonTemplateFactory* create();

	virtual SkeletonTemplate* createSkeletonTemplate(int numBones, Float4x3* offsetMats, Float4 defaultRot[], Float3 defaultPos[], unsigned char parentIndices[]) = 0;
};

}
