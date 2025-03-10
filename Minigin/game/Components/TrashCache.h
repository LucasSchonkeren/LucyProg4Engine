#pragma once
#include "../../eng/abstract/AbstractComponent.h"

namespace cpt {

class TrashCacheImgui1 final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	TrashCacheImgui1(eng::Actor& owner) : AbstractComponent(owner) {};
	~TrashCacheImgui1() = default;

	TrashCacheImgui1(const TrashCacheImgui1&) = delete;
	TrashCacheImgui1& operator=	(const TrashCacheImgui1&) = delete;

	TrashCacheImgui1(const TrashCacheImgui1&&) = delete;
	TrashCacheImgui1& operator=	(const TrashCacheImgui1&&) = delete;

public: //--------------------|Gameloop methods|---------------------------

	void RenderImgui() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	std::vector<float> m_PlotValues{ };
	int m_SampleCount{ 10 };
	bool m_Calculating{ };

}; // !TrashCacheImgui1

class TrashCacheImgui2 final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	TrashCacheImgui2(eng::Actor& owner) : AbstractComponent(owner) {};
	~TrashCacheImgui2() = default;

	TrashCacheImgui2(const TrashCacheImgui2&) = delete;
	TrashCacheImgui2& operator=	(const TrashCacheImgui2&) = delete;

	TrashCacheImgui2(const TrashCacheImgui2&&) = delete;
	TrashCacheImgui2& operator=	(const TrashCacheImgui2&&) = delete;

public: //--------------------|Gameloop methods|---------------------------

	void RenderImgui() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	std::vector<float> m_PlotValues{ };
	std::vector<float> m_PlotValuesAlt{};
	int m_SampleCount{ 100 };
	bool m_Calculating{ }, m_CalculatingAlt{};

}; // !TrashCacheImgui1

} // !_namespace_