using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;

namespace Shingine
{
  public static class Main
  {
    [MenuItem("Shingine/Export Scene...")]
    public static void ExportScene()
    {
      string fileName = EditorUtility.SaveFilePanel("Save scene", "", SceneManager.GetActiveScene().name + ".ssd", "ssd");
      var sceneExporter = new SceneExporter(fileName);
      sceneExporter.CollectNodes();
      sceneExporter.Save();
    }
  }
}
