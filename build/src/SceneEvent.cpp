#include "SceneEvent.h"

COMA_USING_NS

const std::string SceneEvent::CHANGED = "scene_sceneChanged";

SceneEvent::SceneEvent(std::string type, Scene* scene)
:Event(type), scene(scene)
{

}


SceneEvent::~SceneEvent()
{
}
