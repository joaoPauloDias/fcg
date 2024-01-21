#pragma once

class GameObject {
    public:
        virtual void Render() = 0;
        virtual void Update(float dt) = 0;
};