#pragma once
#include <map>
#include "EntityProperties.h"
#include "ForwardDeclarations.h"

public ref class EntityView
{
	Options^ options;

	bool _isDisposed;

	pgn::Window* wnd;
	pgn::FileStream* assetStream1;
	pgn::FileStream* assetStream2;
	pgn::FileStream* cacheStream;
	pgn::Graphics* graphics;

	pgn::Scene* scene;

	pgn::Camera* camera;
	pgn::DirectionalLight* dirLight;
	pgn::SceneDirectionalLight* sceneDirLight;

	pgn::Entity* entity;
	pgn::EditableModel* model;
	pgn::SkeletonFactory* skelFactory;
	pgn::Skeleton* skel;
	pgn::SceneEntity* sceneEntity;
	pgn::AnimationFactory* animFactory;
	std::map<std::string, pgn::Animation*>* anims;

	pgn::Clock* clock;
	int t;

	pgn::DraggingGestureRecognizer* draggingGestureRecognizer;

	bool viewMatDirty;

public:
	EntityView(IntPtr^ hWnd, Options^ options);
	~EntityView();

	property bool isDisposed
	{
		bool get()
		{
			return _isDisposed;
		}
	}

	void update(EntityProperties^ props, DirtyFlags^ dirtyFlags);
};
