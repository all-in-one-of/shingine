using UnityEngine;
using UnityEditor;
using System.Collections.Generic;
using System.IO;
using System;

namespace Shingine
{
  public partial class SceneExporter
  {
    
    public SceneExporter(string fileName)
    {
      _exportData = new SceneExportData(fileName);
    }

    void ProgressBar(string message, float progress, bool open = true)
    {
      if (!open)
      {
        EditorUtility.ClearProgressBar();
        return;
      }
      EditorUtility.DisplayProgressBar("Scene Exporter", message, progress);
    }
    public void CollectNodes()
    {
      ProgressBar("Initialize", 0.1f, false);
      Initialize();
      ProgressBar("SetParentIds", 0.2f, false);
      SetParentIds();
      ProgressBar("CollectDataFromGameObjects", 0.3f, false);
      CollectDataFromGameObjects();
      ProgressBar("CreateNodeListFromCollectedData", 0.3f, false);
      CreateNodeListFromCollectedData();
      _binaryExportData[_exportData.AbsoluteFileName] = BinaryConverter.NodesToBytes(_nodes.ToArray());
      ProgressBar("", 1.0f, false);
    }
    public void Save()
    {
      foreach (var data in _binaryExportData)
      {
        var fileName = data.Key;
        var arr = data.Value;

        var dirName = Path.GetDirectoryName(fileName);
        Directory.CreateDirectory(dirName);
        FileStream fs = new FileStream(fileName, FileMode.Create);
        BinaryWriter bw = new BinaryWriter(fs);
        bw.Write(arr);
        bw.Close();
        fs.Close();
      }
    }
    const string DefaultFileExtension = ".ssd";
    Dictionary<string, byte[]> _binaryExportData = new Dictionary<string, byte[]>();
    Dictionary<string, Node> _meshes = new Dictionary<string, Node>();
    Dictionary<string, Node> _textures = new Dictionary<string, Node>();
    Dictionary<string, Node> _materials = new Dictionary<string, Node>();
    List<Node> _nodes = new List<Node>();
    Dictionary<string, uint> _nodeNameToUid = new Dictionary<string, uint>();
    Dictionary<uint, GameObject> _uidToGameObject = new Dictionary<uint, GameObject>();
    Dictionary<uint, List<uid>> _uidToComponents = new Dictionary<uint, List<uid>>();
    List<Node> _components = new List<Node>();
    SceneExportData _exportData;
    Dictionary<uint, string> _externalPathsDict = new Dictionary<uint, string>();
  }
}