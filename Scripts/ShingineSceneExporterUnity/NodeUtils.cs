using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace Shingine
{
  public static class NodeUtils
  {
    public static class Constants
    {
      public const string TransformComponentName = "TransformComponent";
      public const string MetadataComponentName = "ObjectMetadataComponent";
      public const string RendererComponentName = "RendererComponent";
      public const string LightComponentName = "LightComponent";
      public const string MeshName = "Mesh";
      public const string MaterialName = "Material";
      public const string TextureName = "Texture2D";
      public const string ShaderName = "Shader";
      public const string ShaderSourceName = "ShaderSource";
      public const string ExternalName = "ExternalAsset";
    }
    public static Node MakeExternalNode(uint uniqueId, string nodeName, string fileName)
    {
      int idConv = Convert.ToInt32(uniqueId);
      var node = new Node(Constants.ExternalName, idConv);
      node.AddAttribute(new Attribute<string>("Name", nodeName, true));
      node.AddAttribute(new Attribute<string>("FileName", fileName, true));
      return node;
    }
    public static Node MakeObjectMetadataNode(string nodeName)
    {
      var metadata = new Node(Constants.MetadataComponentName);
      metadata.AddAttribute(new Attribute<string>("Name", nodeName, true));
      metadata.AddAttribute(new Attribute<string>("Tag", "default", true));
      metadata.AddAttribute(new Attribute<string>("Layer", "default", true));
      return metadata;
    }
    public static Node MakeTextureNode(Texture2D texture)
    {
      var textureNode = new Node(Constants.TextureName);
      textureNode.AddAttribute(new Attribute<string>("Name", texture.name, true));
      textureNode.AddAttribute(new Attribute<uint>("Width", Convert.ToUInt32(texture.width), true));
      textureNode.AddAttribute(new Attribute<uint>("Height", Convert.ToUInt32(texture.height), true));
      var pixels = texture.GetPixels();
      float[] pixelArray = new float[pixels.Length * 4];
      for (int x = 0; x < pixels.Length; x++)
      {
        int index = x * 4;
        pixelArray[index + 0] = pixels[x].r;
        pixelArray[index + 1] = pixels[x].g;
        pixelArray[index + 2] = pixels[x].b;
        pixelArray[index + 3] = pixels[x].a;
      }
      textureNode.AddAttribute(new Attribute<float[]>("Pixels", pixelArray));
      return textureNode;
    }
    public static Node MakeTransformNode(uint parentId, TransformNodeData transformNodeData)
    {
      var xformNode = new Node(Constants.TransformComponentName);
      xformNode.AddAttribute(new Attribute<uid>("ParentID", parentId, true));
      xformNode.AddAttribute(new Attribute<byte>("IsDynamic", 0, true));
      xformNode.AddAttribute(new Attribute<float[]>("LocalPosition",
        new float[] { transformNodeData.Position.x, transformNodeData.Position.y, transformNodeData.Position.z }));
      xformNode.AddAttribute(new Attribute<float[]>("LocalRotation",
        new float[] { transformNodeData.Rotation.x, transformNodeData.Rotation.y, transformNodeData.Rotation.z, transformNodeData.Rotation.w }));
      xformNode.AddAttribute(new Attribute<float[]>("LocalScale",
        new float[] { transformNodeData.Scale.x, transformNodeData.Scale.y, transformNodeData.Scale.z }));
      return xformNode;
    }
    public struct TransformNodeData
    {
      public Vector3 Position, Scale;
      public Quaternion Rotation;
      public static TransformNodeData CalculateTransformNodeData(Transform xform)
      {
        TransformNodeData xformData;
        xformData.Position = xform.localPosition;
        xformData.Rotation = xform.localRotation;
        xformData.Scale = xform.localScale;
        return xformData;
      }
    }
    public struct MaterialInfo
    {
      public Node MaterialNode;
      public string MaterialName;
      public uint MaterialId;
    }
    public static Node MakeMeshNode(Mesh mesh)
    {
      var meshNode = new Node(Constants.MeshName);
      meshNode.AddAttribute(new Attribute<string>("Name", mesh.name, true));
      float[] uv = new float[mesh.uv.Length * 3];
      float[] vertices = new float[mesh.vertices.Length * 3];
      float[] normals = new float[mesh.normals.Length * 3];
      for (uint x = 0; x < mesh.uv.Length; x++) 
      {
        uint index = x * 3;
        uv[index + 0] = mesh.uv[x].x;
        uv[index + 1] = mesh.uv[x].y;
        uv[index + 2] = 0;

        vertices[index + 0] = mesh.vertices[x].x;
        vertices[index + 1] = mesh.vertices[x].y;
        vertices[index + 2] = mesh.vertices[x].z;

        normals[index + 0] = mesh.normals[x].x;
        normals[index + 1] = mesh.normals[x].y;
        normals[index + 2] = mesh.normals[x].z;
      }
      uint[] indices = new uint[mesh.triangles.Length];
      for (uint x = 0; x < mesh.triangles.Length; x++) indices[x] = (uint) mesh.triangles[x];
      meshNode.AddAttribute(new Attribute<uint[]>("Indices", indices));
      meshNode.AddAttribute(new Attribute<float[]>("Normals", normals));
      meshNode.AddAttribute(new Attribute<float[]>("Positions", vertices));
      meshNode.AddAttribute(new Attribute<float[]>("TexCoord", uv));
      return meshNode;
    }
    public static Node MakeRendererNode(uint materialId, uint meshId)
    {
      var rendererNode = new Node(Constants.RendererComponentName);
      rendererNode.AddAttribute(new Attribute<byte>("DrawType", 1, true)); // Fill
      rendererNode.AddAttribute(new Attribute<byte>("Enabled", 1, true));
      rendererNode.AddAttribute(new Attribute<uid>("MeshReference", meshId, true));
      rendererNode.AddAttribute(new Attribute<uid>("MaterialReference", materialId, true));
      return rendererNode;
    }
    public static Node MakeMaterialNode(Material mat)
    {
      var materialNode = new Node(Constants.MaterialName);
      var shader = mat.shader;
      var propertyCount = ShaderUtil.GetPropertyCount(shader);

      List<string> floatPropertyNames = new List<string>();
      List<float> floatPropertyValues = new List<float>();
      List<string> vectorPropertyNames = new List<string>();
      List<float> vectorPropertyValues = new List<float>();

      for (int x = 0; x < propertyCount; x++)
      {
        var propertyName = ShaderUtil.GetPropertyName(shader, x);
        var propertyType = ShaderUtil.GetPropertyType(shader, x);
        if (propertyType == ShaderUtil.ShaderPropertyType.Color)
        {
          vectorPropertyNames.Add(propertyName);
          var color = mat.GetColor(propertyName);
          vectorPropertyValues.AddRange(new float[] { color.r, color.g, color.b, color.a });
        }

        if (propertyType == ShaderUtil.ShaderPropertyType.Vector)
        {
          vectorPropertyNames.Add(propertyName);
          var vec = mat.GetVector(propertyName);
          vectorPropertyValues.AddRange(new float[] { vec.x, vec.y, vec.z, vec.w });
        }

        if (propertyType == ShaderUtil.ShaderPropertyType.Float)
        {
          floatPropertyNames.Add(propertyName);
          var floatValue = mat.GetFloat(propertyName);
          floatPropertyValues.Add(floatValue);
        }
      }
      materialNode.AddAttribute(new Attribute<string[]>("FloatUniformNames",
        floatPropertyNames.ToArray()));
      materialNode.AddAttribute(new Attribute<float[]>("FloatUniformValues",
        floatPropertyValues.ToArray()));

      materialNode.AddAttribute(new Attribute<string[]>("VectorUniformNames",
        vectorPropertyNames.ToArray()));
      materialNode.AddAttribute(new Attribute<float[]>("VectorUniformValues",
        vectorPropertyValues.ToArray()));

      return materialNode;
    }
  }
}