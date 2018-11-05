using System;
using System.Collections.Generic;

namespace Shingine
{
  public class Node
  {
    public List<IAttribute> Attributes { get { return _attributes; } }
    public List<Node> Nodes { get { return _nodes; } }
    public Node(string nodeName = "Node", int uid = -1)
    {
      _name = nodeName;

      if (uid == -1)
        _uid = GetUid();
      else
        _uid = Convert.ToUInt32(uid);
    }
    public string Name { get { return _name; } }
    public uint UniqueId { get { return _uid; } }
    public void AddAttribute(IAttribute attr)
    {
      _attributes.Add(attr);
    }
    public void InitializeUid()
    {
      NextUid = 50;
    }
    uint GetUid()
    {
      return NextUid++;
    }
    static uint NextUid = 50;
    string _name;
    uint _uid;
    List<IAttribute> _attributes = new List<IAttribute>();
    List<Node> _nodes = new List<Node>();
  }
}