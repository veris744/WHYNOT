#pragma once

#include "Component.h"
#include "LightData.h"


class LightSource : public Component
{
    
public:
    LightSource()
    {
        name = "LIGHT";
        lightData = LightData();
    }

    void Update() override
    {
        
    }

    LightData lightData;    
};
