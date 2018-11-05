using System;
using System.IO;
namespace Shingine
{
  public class SceneExportData
  {
    public string AbsoluteFileName { get { return _absoluteFileName; } }
    public string AbsoluteBasePath { get { return Path.GetDirectoryName(AbsoluteFileName); } }
    public const string AssetDir = "SceneAssets";
    public string GetFullAssetPathDir(string nodeTypeName)
    {
      return Path.Combine(Path.Combine(AbsoluteBasePath, AssetDir), nodeTypeName);
    }
    object FileInfo(string absoluteFileName)
    {
      return null;
    }
    public string BasePath;
    public SceneExportData(string fileName)
    {
      _absoluteFileName = fileName;
    }
    string _absoluteFileName;
  }
}