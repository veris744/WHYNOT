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
    ~LightSource() override = default;

    void Update(float deltaTime) override
    {
        
    }

    LightData lightData;    
};
