#pragma once

class VirtualScene; // Declaration to break circular dependency between GameObject and VirtualScene

class GameObject {
    private:
        VirtualScene *scene;

    public:
        void setScene(VirtualScene *scene);
        VirtualScene* getScene();
        virtual void Render() = 0;
        virtual void Update(float dt) = 0;
};