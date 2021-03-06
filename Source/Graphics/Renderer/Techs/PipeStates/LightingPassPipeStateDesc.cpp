#include <PGN/RenderingSystem/DepthStencilState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char lightingPassVS[];
extern "C" char lightingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
};

static DepthStencilStateDesc depthStencilStateDesc =
{
	true,					// depthEnable
	false,					// depthWrite
	NOT_EQUAL,				// depthFunc

	false,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};

static PipeStateDesc pipeStateDesc =
{
	lightingPassVS,
	lightingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	&depthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* lightingPassPipeStateDesc = &pipeStateDesc;
