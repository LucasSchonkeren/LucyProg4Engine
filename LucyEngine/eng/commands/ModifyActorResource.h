#pragma once
#include "../abstract/AbstractCommand.h"
#include "../components/ResourceTracker.h"

namespace eng::cmd {

class ModifyActorResource final : public ICommand {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	ModifyActorResource(std::string_view resource, int value) : m_Resource(resource), m_Value(value) {};
	~ModifyActorResource()	= default;

	ModifyActorResource				(const ModifyActorResource&)	= delete;
	ModifyActorResource& operator=	(const ModifyActorResource&)	= delete;

	ModifyActorResource				(const ModifyActorResource&&)	= delete;
	ModifyActorResource& operator=	(const ModifyActorResource&&)	= delete;


public: //---------------------------|Execute|-------------------------

	bool Execute(Actor& target) override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	std::string m_Resource;
	int m_Value;
	cpt::ResourceTracker* m_ResourceTrackerPtr{};

}; // !ModifyActorResource

} // !eng::cmd