using UnityEngine;
namespace Shingine
{
  public partial class SceneExporter
  {
    void FindRenderer(uint uid, GameObject go)
    {
      // find mesh renderer
      var meshRenderer = go.GetComponent<MeshRenderer>();
      var meshFilter = go.GetComponent<MeshFilter>();
      if (!meshRenderer || !meshFilter)
        return;

      var mesh = meshFilter.sharedMesh;
      var material = meshRenderer.sharedMaterial;

      uint materialId = GetMaterialId(material);
      uint meshId = GetMeshId(mesh);

      var rendererNode = NodeUtils.MakeRendererNode(materialId, meshId);
      // save mesh and material here
      AddComponentNode(uid, rendererNode);
    }
    void FindLight(uint uid, GameObject go)
    {
      Light light = go.GetComponent<Light>();
      if (!light)
        return;
      float[] color = new float[3] { light.color.r, light.color.g, light.color.b };
      float intensity = light.intensity;
      var unityLightType = light.type;
      byte lightType = 0;
      switch (unityLightType)
      {
        case LightType.Point: lightType = 0; break;
        case LightType.Spot: lightType = 1; break;
        case LightType.Directional: lightType = 2; break;
        default: break;
      }
      byte shadowEnabled = light.shadows != LightShadows.None
        ? (byte)1
        : (byte)0;
      float outerAngle = light.spotAngle;
      float cutoff = light.range;

      var lightNode = new Node("LightComponent");
      // add attributes
      lightNode.AddAttribute(new Attribute<float[]>("Color", color, false));
      lightNode.AddAttribute(new Attribute<float>("Exposure", 1.0f, true));
      lightNode.AddAttribute(new Attribute<float>("Intensity", intensity, true));

      lightNode.AddAttribute(new Attribute<byte>("LightType", lightType, true));
      lightNode.AddAttribute(new Attribute<float>("OuterAngle", outerAngle, true));
      lightNode.AddAttribute(new Attribute<float>("CutOff", cutoff, true));
      lightNode.AddAttribute(new Attribute<byte>("ShadowEnabled", shadowEnabled, true));
      AddComponentNode(uid, lightNode);
    }
  }
}