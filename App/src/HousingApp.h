#pragma once
#include <TitanEngine/TitanEngine.h>
#include "Layers/HousingViewerLayer.h"

class HousingApp : public Titan::Application {
public:
    void OnInit() override {
        PushLayer(new HousingViewerLayer());
    }
};