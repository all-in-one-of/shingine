using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

namespace Shingine
{
  public partial class SceneExporter
  {
    Node MakeExternalReferenceNode(Node node)
    {
      var assetName = node.Name + "_" + node.UniqueId;
      IAttribute attr = null;
      foreach (var a in node.Attributes)
        if (a.Name == "Name")
        {
          attr = a;
          break;
        }

      if (attr != null)
        assetName = (string)attr.Value;

      var fullPath = _exportData.GetFullAssetPathDir(node.Name);
      var assetFileName = assetName + DefaultFileExtension;
      var assetSaveFileName = Path.Combine(fullPath, assetFileName);
      var infoAssetFileName = Path.Combine(
        Path.Combine(SceneExportData.AssetDir, node.Name), assetFileName);
      var externalNode = NodeUtils.MakeExternalNode(node.UniqueId, node.Name, infoAssetFileName);
      _binaryExportData[assetSaveFileName] = BinaryConverter.NodesToBytes(new Node[] { node });
      _externalPathsDict[node.UniqueId] = infoAssetFileName;
      return externalNode;
    }
    public static void SetTextureImporterFormat(Texture2D texture, bool isReadable)
    {
      if (null == texture)
        return;

      string assetPath = AssetDatabase.GetAssetPath(texture);
      var textureImporter = AssetImporter.GetAtPath(assetPath) as TextureImporter;
      if (textureImporter == null)
        return;
      if (textureImporter.isReadable)
        return;
      textureImporter.textureType = TextureImporterType.Default;
      textureImporter.isReadable = isReadable;
      AssetDatabase.ImportAsset(assetPath);
      AssetDatabase.Refresh();
    }
    uint GetTextureId(Texture texture)
    {
      if (texture == null)
        return 0;
      var texture2d = (Texture2D)texture;
      // this will generate a new texture asset and save it as an external asset
      if (texture2d == null)
        return 0;
      // discard rectangular textures
      if (texture2d.width != texture2d.height)
        return 0;
      // discard non power of two textures
      int w = texture2d.width;
      if (((w != 0) && ((w & (w - 1)) == 0)) == false)
        return 0;

      if (_textures.ContainsKey(texture2d.name))
        return _textures[texture2d.name].UniqueId;
      // create new texture node and save external asset
      SetTextureImporterFormat(texture2d, true);
      ProgressBar("Processing texture " + texture2d.name, 0.7f, false);
      var textureNode = NodeUtils.MakeTextureNode(texture2d);
      var external = MakeExternalReferenceNode(textureNode);
      _textures[texture2d.name] = external;
      return external.UniqueId;
    }
    void SetTextureAssetAttributes(Material material, Node materialNode)
    {
      string[] materialPropertyNames = material.GetTexturePropertyNames();
      List<string> texturePaths = new List<string>();
      for (int x = 0; x < materialPropertyNames.Length; x++)
      {
        var texture = material.GetTexture(materialPropertyNames[x]);
        uint textureId = GetTextureId(texture);
        string externalPath = textureId == 0 ? "" : _externalPathsDict[textureId];
        texturePaths.Add(externalPath);
      }
      materialNode.AddAttribute(new Attribute<string[]>("TextureUniformNames", materialPropertyNames));
      materialNode.AddAttribute(new Attribute<string[]>("ExternalTexturePaths", texturePaths.ToArray()));
    }
    NodeUtils.MaterialInfo ProcessMaterial(Material mat)
    {
      NodeUtils.MaterialInfo materialInfo = new NodeUtils.MaterialInfo()
      { MaterialNode = null, MaterialName = "", MaterialId = 0 };
      if (mat == null)
        return materialInfo;

      materialInfo.MaterialName = mat.name;
      if (!_materials.ContainsKey(mat.name))
      {
        var materialNode = NodeUtils.MakeMaterialNode(mat);
        SetTextureAssetAttributes(mat, materialNode);
        materialInfo.MaterialNode = materialNode;
        materialInfo.MaterialId = materialNode.UniqueId;
      }
      else
      {
        materialInfo.MaterialId = _materials[materialInfo.MaterialName].UniqueId;
      }
      return materialInfo;
    }
    uint GetMaterialId(Material material)
    {
      var materialInfo = ProcessMaterial(material);
      if (materialInfo.MaterialNode != null)
      {
        _materials[materialInfo.MaterialName] =
          MakeExternalReferenceNode(materialInfo.MaterialNode);
      }
      return materialInfo.MaterialId;
    }
    uint GetMeshId(Mesh mesh)
    {
      if (!_meshes.ContainsKey(mesh.name))
      {
        var meshNode = NodeUtils.MakeMeshNode(mesh);
        uint id = meshNode.UniqueId;
        ProgressBar("Saving mesh " + mesh.name, 0.55f, false);
        _meshes[mesh.name] = MakeExternalReferenceNode(meshNode);
        return id;
      }
      return _meshes[mesh.name].UniqueId;
    }
  }
}