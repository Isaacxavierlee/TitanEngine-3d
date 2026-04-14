#pragma once
#include "TitanEngine/Events/Event.h"
#include <string>

namespace Titan {

    class Layer {
    public:
        explicit Layer(const std::string& name = "Layer") : m_Name(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach()          {}
        virtual void OnDetach()          {}
        virtual void OnUpdate(float dt)  {}
        virtual void OnImGuiRender()     {}
        virtual void OnEvent(Event& e)   {}

        const std::string& GetName() const { return m_Name; }

    protected:
        std::string m_Name;
    };

} // namespace Titan
