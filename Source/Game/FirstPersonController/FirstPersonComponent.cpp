#include "Game/FirstPersonController/FirstPersonComponent.h"
namespace FirstPersonController {
REGISTER_COMPONENT(FirstPersonComponent);
REGISTER_SERIALIZED_CLASS(ViewSettings);
REGISTER_SERIALIZED_CLASS(MovementSettings);

FirstPersonComponent::~FirstPersonComponent() {
  delete PlayerViewSettings;
  delete PlayerMovementSettings;
};
}; // namespace FirstPersonController
