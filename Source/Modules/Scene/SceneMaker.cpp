#include <map>
#include <vector>

#include "Modules/Scene/Scene.h"
#include "Modules/Scene/SceneMaker.h"

IScene *SceneMaker::Create(const std::vector<IDataNode *> &nodes) {
  Scene *scene = new Scene();
  scene->SetSerializedData(nodes);
  return scene;
}
