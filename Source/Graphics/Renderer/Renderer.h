#include <map>
#include <PGN/Math/Math.h>
#include <PGN/Platform/UI/Display.h>
#include <PGN/Utilities/HeapAllocator2.h>
#include <PGN/Utilities/Queue.h>
#include <string>
#include <vector>
#include "../Viewport.h"
#include "Batch.h"
#include "Geometry.h"
#include "Passes/EnvConstEnum.h"
#include "Passes/Passes.h"
#include "Passes/RTs/ResourceViewDesc.h"
#include "Passes/RTs/RTs.h"
#include "Techs/Techs.h"

namespace pgn {
	class AssetLoader;
	class DepthStencilState;
	class FileStream;
	class Heap;
	class OutputMergerState;
	class Pipeline;
	class Program;
	class RasterizerState;
	class RenderingContext;
	class RenderingSystem;
	class ResourceHandle;
	class ResourceManager;
	class ResourceView;
	class Sampler;
	class Shader;
	class Texture;
	class VertexInputLayout;
	class Window;
}

class CBufAllocator;
struct CBufRange;
class EditablePNMFactory;
class NAVFactory;
class PNMFactory;
class PNTFactory;
class RenderingStage;
struct ResourceView;
struct VertexFormat;

struct EnvConst
{
	void* p;
	int size;
};

struct RenderTarget
{
	ResourceView* view;
	bool clearNeeded;
	float clearValue[4];
};

struct DepthStencilBuf
{
	ResourceView* view;
	bool depthClearNeeded;
	bool stencilClearNeeded;
	float depthClearValue;
	unsigned stencilClearValue;
};

class Env
{
public:
	int numConsts;
	EnvConst** consts;
	int sizeCBuf;
	std::vector<RenderTarget> offscreenRTs;
	DepthStencilBuf depthStencilBuf;
};

class VertexInputLayoutPtr
{
public:
	pgn::VertexInputLayout* p;

	VertexInputLayoutPtr()
	{
		p = 0;
	}
};

class PipeState
{
public:
	pgn::Shader*					vs;
	pgn::Shader*					ps;
	pgn::Program*					program;
	std::vector<pgn::Sampler*>		samplers;
	pgn::RasterizerState*			rasterizerState;
	pgn::DepthStencilState*			depthStencilState;
	pgn::OutputMergerState*			outputMergerState;

	std::vector<pgn::Texture**>		texBindingPoints;

	std::map<const VertexFormat*, VertexInputLayoutPtr> vertexInputLayoutMap;
};

struct Tech
{
	PipeState* pipeSet[numPasses];
};

struct RendererConfig
{
	PassEnum* activePasses;
	int numActivePasses;

	PassEnum* opaqueEntityPasses;
	int numOpaqueEntityPasses;

	PassEnum* transparentEntityPasses;
	int numTransparentEntityPasses;

	PassEnum* postProcessingPasses;
	int numPostProcessingPasses;
};

extern RendererConfig lightIndexedDeferredRendering;
extern RendererConfig lightIndexedForwardRendering;

typedef pgn::HeapAllocator2<Batch> BatchAllocator;
typedef std::list<Batch, BatchAllocator> BatchGroup;

class BatchGroupPtr
{
public:
	BatchGroup* p;

	BatchGroupPtr()
	{
		p = 0;
	}
};

typedef std::map<const VertexFormat*, BatchGroupPtr> BatchGroupMap;

class SceneContext
{
public:
	pgn::Float4x3 view;
	pgn::Float4x4 proj;
	Viewport viewport;

	struct PointLight
	{
		pgn::Float4 intensity_spec;
		pgn::Float4 pos_att;
	};

	static const int maxNumPointLights = 256;
	PointLight wPointLights[maxNumPointLights];
	PointLight vPointLights[maxNumPointLights];
	int numPointLights;

	struct DirectionalLight
	{
		pgn::Float4 intensity_spec;
		pgn::Float4 dir_enabled;
	};

	static const int maxNumDirLights = 8;
	DirectionalLight wDirLights[maxNumDirLights];
	DirectionalLight vDirLights[maxNumDirLights];

	std::map<TechEnum, BatchGroupMap> batches[numPasses];
	pgn::Heap* heap;
	CBufAllocator* cbufAllocator;

	pgn::SyncPoint* sync;

	SceneContext(pgn::RenderingSystem* rs);
	~SceneContext();
};

class Renderer
{
public:
	pgn::RenderingContext* rc;

	SubsetAllocator subsetAllocator;

	PNMFactory* pnmFactory;
	EditablePNMFactory* editablePnmFactory;
	NAVFactory* navFactory;
	PNTFactory* pntFactory;

	pgn::Heap* heap;

	RendererConfig cfg;
	bool isPassActive[numPasses];

	pgn::RenderingSystem* rs;

	pgn::FileStream* assetStream;
	pgn::AssetLoader* resLoader;
	pgn::ResourceManager* geomMgr;
	pgn::ResourceManager* editableGeomMgr;
	pgn::ResourceManager* navGeomMgr;
	pgn::ResourceManager* texMgr;

	pgn::FileStream* cacheStream;

	EnvConst envConsts[numEnvConsts];
	ResourceView* resViews[numResViews];
	Env envs[numPasses];

	Tech techs[numTechs];
	std::map<std::string, pgn::Texture*> texBindingPointMap;

	pgn::ResourceHandle* gray;
	pgn::ResourceHandle* black;
	pgn::ResourceHandle* screenAlignedQuad;
	pgn::ResourceHandle* sphere;

	std::vector<SceneContext*> freeList;
	pgn::Queue<SceneContext*> retired;
	SceneContext* sceneContext;

	RenderingStage* renderingStage;
	pgn::Pipeline* renderQueue;

	int queuedFrameCount;
	long long submittingCount;
	long long retireCount;
	long long finishCount;

	Renderer(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream);
	~Renderer();

	bool buildPrograms(void* cacheFileBuf = 0);
	void beginDraw(pgn::Window* wnd, RendererConfig* cfg);
	void endDraw();
	bool beginFrame();
	void endFrame();
	SceneContext* beginSubmit();
	void submit(PassEnum passEnum, TechEnum techEnum, Batch* batch);
	void endSubmit();
	void render(SceneContext* sceneContext);
	void finish();
};
