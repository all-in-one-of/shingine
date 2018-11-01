#include <iostream>

#include "Application/Prototyping.h"
#include "Application/Setup.h"
#include "Core.h"
#include "Modules/Statics/IGraphics.h"
#include "Solver/Solver.h"

int main() {
  InitializeEngine();
  AddFirstPersonController();

  if (!ResourceLoader::LoadScene("Assets/Scenes/Test.ssd")) {
    std::cout << "Couldn't load the scene" << std::endl;
    return 1;
  }

  TestStuff();

  Statics::Get<IGraphics>()->SetupWindow();

  Solver *solver = new Solver();
  solver->AddSystem("TransformSystem");
  solver->AddSystem("RenderingSystem");
  solver->AddSystem("FirstPersonSystem");
  solver->AddSystem("LightViewerSystem");
  solver->InitializeSystems();

  while (solver->Simulate()) {
    if (!Statics::Get<IGraphics>()->Render())
      break;
  }
  return 0;
}
