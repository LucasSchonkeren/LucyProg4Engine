#include "ResourceDisplayLogic.h"
#include "../Actor.h"
#include "ResourceTracker.h"

#include <algorithm>
#include <regex>

namespace eng::cpt {

REGISTER_COMPONENT(ResourceDisplayLogic)

nlohmann::ordered_json eng::cpt::ResourceDisplayLogic::Serialize() {
    nlohmann::ordered_json f_Json{};
    f_Json["FormatString"] = m_FormatString;
    return f_Json;
}

std::unique_ptr<ResourceDisplayLogic> eng::cpt::ResourceDisplayLogic::Deserialize(Actor& owner, const nlohmann::json& json) {
    std::string f_FormatString{json.value("FormatString", "")};
    return std::make_unique<ResourceDisplayLogic>(owner, f_FormatString);
}

void eng::cpt::ResourceDisplayLogic::SetText(std::string_view formatString) {
    m_FormatString = formatString;
}

void eng::cpt::ResourceDisplayLogic::Start() {
    m_TextRendererPtr = Owner().GetComponent<TextRenderer>();
    assert(m_TextRendererPtr and "This actor requires a Text Renderer Component");
}

void eng::cpt::ResourceDisplayLogic::OnEvent(Event event) {
    if (event.eventTypeHash != eng::eventHash::actorResourceChanged) return;
    UpdateTextComponent(std::any_cast<eng::eventContext::ResourceChanged>(event.context).sender);
}

void eng::cpt::ResourceDisplayLogic::UpdateTextComponent(ResourceTracker* const resourceTrackerPtr) {
    std::regex pattern(R"(\{([^}]*)\})");
    std::string f_Result{}, f_Suffix{};

    for (std::sregex_iterator it(m_FormatString.begin(), m_FormatString.end(), pattern), endIt{}; it != endIt; ++it) {
        f_Result.append(it->prefix().str());

        std::string content = it->str(1);



        if (int value{ resourceTrackerPtr->GetResource(content) }; value >= 0) {
            f_Result.append(std::to_string(value));
        }
        else {
            f_Result.append(it->str(0));
        }

        f_Suffix = it->suffix().str();
    }

    f_Result.append(f_Suffix);

    m_TextRendererPtr->SetText(f_Result);
}

} // !eng::cpt