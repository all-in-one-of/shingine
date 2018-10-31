#include "Modules/Scene/Scene.h"

Scene::Scene() {}
Scene::~Scene() {}
void Scene::Save() {}

void Scene::SetSerializedData(const std::vector<IDataNode *> &nodes) {
  SerializedData = nodes;
}
