#pragma once
#include <set>

namespace eng {

class AbstractRenderer {
public:
	virtual ~AbstractRenderer();

	virtual void TrackRenderable(AbstractRenderable* RenderablePtr);
	virtual void UntrackRenderable(AbstractRenderable* RenderablePtr);

	virtual void Render() = 0;

private:
	std::set<AbstractRenderable*> m_RenderablePtrs;
};

class AbstractRenderable {
	friend AbstractRenderer;
public:
	AbstractRenderable(AbstractRenderer& renderer)	{ renderer.TrackRenderable(this); m_RendererPtr = &renderer; }
	virtual ~AbstractRenderable()					{ if (m_RendererPtr) m_RendererPtr->UntrackRenderable(this); }

	virtual void Render() = 0;

private:
	AbstractRenderer* m_RendererPtr;
};

} // !eng