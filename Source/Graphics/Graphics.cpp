#include <PGN/Assets/PNM.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/Heap.h>
#include "DirectionalLight.h"
#include "EditableModel.h"
#include "Graphics.h"
#include "Model.h"
#include "NavModel.h"
#include "PointLight.h"
#include "SkeletalModel.h"

Graphics::Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
	: renderer(displayPrototype, assetStream, cacheStream)
{
	modelPool = pgn::Pool::create(sizeof(Model));
	editableModelPool = pgn::Pool::create(sizeof(EditableModel));
	skeletalModelPool = pgn::Pool::create(sizeof(SkeletalModel));
	navModelPool = pgn::Pool::create(sizeof(NavModel));
	pointLightPool = pgn::Pool::create(sizeof(PointLight));
	dirLightPool = pgn::Pool::create(sizeof(DirectionalLight));
	tmpBuf = pgn::Heap::create();
}

void Graphics::dispose()
{
	modelPool->destroy();
	editableModelPool->destroy();
	skeletalModelPool->destroy();
	navModelPool->destroy();
	pointLightPool->destroy();
	dirLightPool->destroy();
	tmpBuf->destroy();
}

pgn::Graphics* pgn::Graphics::create(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
{
	return debug_new ::Graphics(displayPrototype, assetStream, cacheStream);
}

void Graphics::_free()
{
	delete this;
}

void Graphics::beginDraw(pgn::Window* wnd)
{
	//renderer.beginDraw(wnd, &lightIndexedDeferredRendering);
	renderer.beginDraw(wnd, &lightIndexedForwardRendering);
}

void Graphics::endDraw()
{
	renderer.finish();
	performPendingRemovals();
	renderer.endDraw();
}

void Graphics::performPendingRemovals()
{
	while (!pendingModelRemovals.empty())
	{
		Model* model = pendingModelRemovals.front();

		if (model->submittingStamp > renderer.finishCount) break;

		model->~Model();
		modelPool->_free(model);
		pendingModelRemovals.pop_front();
	}

	while (!pendingEditableModelRemovals.empty())
	{
		EditableModel* model = pendingEditableModelRemovals.front();

		if (model->submittingStamp > renderer.finishCount) break;

		model->~EditableModel();
		editableModelPool->_free(model);
		pendingEditableModelRemovals.pop_front();
	}

	while (!pendingNavModelRemovals.empty())
	{
		NavModel* model = pendingNavModelRemovals.front();

		if (model->submittingStamp > renderer.finishCount) break;

		model->~NavModel();
		navModelPool->_free(model);
		pendingNavModelRemovals.pop_front();
	}
}
