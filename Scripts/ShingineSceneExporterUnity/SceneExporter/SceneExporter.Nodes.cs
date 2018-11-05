using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace Shingine
{
  public partial class SceneExporter
  {
    void Initialize()
    {
      _meshes.Clear();
      _textures.Clear();
      _materials.Clear();
      _nodes.Clear();
      _nodeNameToUid.Clear();
      _uidToGameObject.Clear();
      _uidToComponents.Clear();
      _components.Clear();
    }
    void CreateNodeListFromCollectedData()
    {
      var idCollectionNode = new Node("EntityIdCollection");
      uid[] ids = new uid[_uidToComponents.Count];
      uint x = 0;
      foreach (var u in _uidToComponents) ids[x++] = u.Key;
      idCollectionNode.AddAttribute(new Attribute<uid[]>("Ids", ids));
      List<Node> comIdCollection = new List<Node>();
      foreach (var u in _uidToComponents.Keys)
      {
        var cMid = new Node("ComponentIdCollection");
        cMid.AddAttribute(new Attribute<uid[]>("Ids", _uidToComponents[u].ToArray()));
        comIdCollection.Add(cMid);
      }
      idCollectionNode.AddAttribute(new Attribute<Node[]>("Components", comIdCollection.ToArray()));
      _nodes.Add(idCollectionNode);
      _nodes.AddRange(_components);
      _nodes.AddRange(_meshes.Values);
      _nodes.AddRange(_materials.Values);
      _nodes.AddRange(_textures.Values);
    }
    uint GetParentNodeUid(GameObject go)
    {
      var inputNode = go.transform.parent;
      if (inputNode != null)
      {
        var parentName = inputNode.name;
        if (!_nodeNameToUid.ContainsKey(parentName))
          return 0;
        return _nodeNameToUid[parentName];
      }
      return 0;
    }
    void CollectDataFromGameObjects()
    {
      // Debug.Log(_uidToGameObject.Count);
      foreach (var entry in _uidToGameObject)
      {
        var uid = entry.Key;
        var go = entry.Value;
        // add object metadata 
        AddComponentNode(uid, NodeUtils.MakeObjectMetadataNode(go.name));
        var parentId = GetParentNodeUid(go);
        // transform
        AddComponentNode(uid, NodeUtils.MakeTransformNode(parentId,
          NodeUtils.TransformNodeData.CalculateTransformNodeData(go.transform)));
        // find lights
        FindLight(uid, go);
        // supposedly will collect all info about materials textures and meshes
        FindRenderer(uid, go);
      }
    }
    void AddComponentNode(uint uid, Node componentNode)
    {
      _components.Add(componentNode);
      _uidToComponents[uid].Add(componentNode.UniqueId);
    }
    void SetParentIds()
    {
      GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>();
      foreach (var go in allObjects)
      {
        var entity = new Node("Entity");
        _nodes.Add(entity);
        var uid = entity.UniqueId;
        _uidToGameObject[uid] = go;
        _nodeNameToUid[go.name] = uid;
        _uidToComponents[uid] = new List<uid>();
      }
    }
  }
}