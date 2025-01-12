#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H


namespace Engine::Scene {
    class Scene {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(float deltaTime) {}
    };
}

#endif //ENGINE_SCENE_H
