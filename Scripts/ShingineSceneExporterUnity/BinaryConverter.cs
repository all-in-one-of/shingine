using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace Shingine
{
  public class BinaryConverter
  {
    const string Signature = "SSD";
    const byte Version = 0x01;
    const byte NodeBegin = 0xaa;
    const byte NodeEnd = 0xab;
    const byte AttrBegin = 0xba;
    const byte AttrEnd = 0xbb;
    const byte CharacterLimit = 32;
    static byte[] MakeByteHeader()
    {
      List<byte> bytes = new List<byte>(StringToBytes(Signature));
      bytes.Add(Version);
      return bytes.ToArray();
    }
    static byte[] Uint16ToBytes(ushort val)
    {
      byte byte0 = Convert.ToByte((val >> 8) & 0xff);
      byte byte1 = Convert.ToByte(val & 0xff);
      return new byte[2] { byte0, byte1 };
    }
    static byte[] Uint32ToBytes(uint val)
    {
      return new byte[4] {
        Convert.ToByte((val >> 24) & 0xff),
        Convert.ToByte((val >> 16) & 0xff),
        Convert.ToByte((val >> 8) & 0xff),
        Convert.ToByte(val & 0xff)
      };
    }
    static byte[] StringToBytes(string val)
    {
      return Encoding.ASCII.GetBytes(val);
    }

    static byte[] AttributeToBytes(IAttribute attr)
    {
      List<byte> data = new List<byte>();
      data.Add(AttrBegin);
      data.Add(Convert.ToByte(attr.Name.Length + 1));
      data.AddRange(StringToBytes(attr.Name));
      data.Add(0);

      data.Add(Convert.ToByte(attr.DataTypeName.Length + 1));
      data.AddRange(StringToBytes(attr.DataTypeName));
      data.Add(0);

      data.Add(Convert.ToByte((attr.IsSingleValue ? 1 : 0) & 0xff));
      if (attr.DataTypeName == "char")
      {
        if (attr.IsSingleValue)
        {
          var charAttr = attr as Attribute<string>;
          // byte count
          data.AddRange(Uint32ToBytes((uint)(charAttr.UnboxedValue.Length + 1)));
          // element count
          data.AddRange(Uint32ToBytes(1u));
          // value
          data.AddRange(StringToBytes(charAttr.UnboxedValue));
          data.Add(0);
        }
        else
        {
          var strAttr = attr as Attribute<string[]>;
          var elementCount = (uint)strAttr.UnboxedValue.Length;
          List<byte> values = new List<byte>();
          foreach (var val in strAttr.UnboxedValue)
          {
            values.AddRange(StringToBytes(val));
            values.Add(0);
          }
          // byte count
          data.AddRange(Uint32ToBytes((uint)values.Count));
          // element count 
          data.AddRange(Uint32ToBytes(elementCount));
          // value
          data.AddRange(values);
        }
      }
      else if (attr.DataTypeName == "SerializedClass")
      {
        var unAttr = attr as Attribute<Node[]>;
        var elementCount = (uint)unAttr.UnboxedValue.Length;
        // byte count
        //element count
        data.AddRange(Uint32ToBytes(elementCount));
        data.AddRange(Uint32ToBytes(elementCount));
        // value
        foreach (var n in unAttr.UnboxedValue)
          data.AddRange(NodeToBytes(n));
      }
      else
      {
        uint elementCount = attr.IsSingleValue ? 1u : 0u;
        uint byteCount = elementCount;
        List<byte> unpackedValues = new List<byte>();
        if (attr.DataTypeName == "float")
        {
          if (attr.IsSingleValue)
          {
            unpackedValues.AddRange(BitConverter.GetBytes((attr as Attribute<float>).UnboxedValue));
            byteCount = 4;
            elementCount = 1;
          }
          else
          {
            var floatAttr = attr as Attribute<float[]>;
            elementCount = (uint)floatAttr.UnboxedValue.Length;
            byteCount = elementCount * 4;
            foreach (var a in floatAttr.UnboxedValue)
              unpackedValues.AddRange(BitConverter.GetBytes(a));
          }
        }
        else if (attr.DataTypeName == "unsigned int" || attr.DataTypeName == "int"
          || attr.DataTypeName == "uid")
        {
          if (attr.IsSingleValue)
          {
            if (attr.DataTypeName == "unsigned int")
              unpackedValues.AddRange(Uint32ToBytes((attr as Attribute<uint>).UnboxedValue));
            if (attr.DataTypeName == "int")
              unpackedValues.AddRange(Uint32ToBytes((uint)(attr as Attribute<int>).UnboxedValue));
            if (attr.DataTypeName == "uid")
              unpackedValues.AddRange(Uint32ToBytes((attr as Attribute<uid>).UnboxedValue));
            byteCount = 4;
            elementCount = 1;
          }
          else
          {
            var uintAttr = attr as Attribute<uint[]>;
            var intAttr = attr as Attribute<int[]>;
            var uidAttr = attr as Attribute<uid[]>;
            // Debug.LogFormat("uintattr {0} intattr {1} uidattr {2}", uintAttr, intAttr, uidAttr);
            if (uintAttr != null)
            {
              elementCount = (uint)uintAttr.UnboxedValue.Length;
              foreach (var v in uintAttr.UnboxedValue)
                unpackedValues.AddRange(Uint32ToBytes(v));
            }
            if (intAttr != null)
            {
              elementCount = (uint)intAttr.UnboxedValue.Length;
              foreach (var v in intAttr.UnboxedValue)
                unpackedValues.AddRange(Uint32ToBytes((uint)v));
            }
            if (uidAttr != null)
            {
              elementCount = (uint)uidAttr.UnboxedValue.Length;
              foreach (var v in uidAttr.UnboxedValue)
                unpackedValues.AddRange(Uint32ToBytes(v));
            }
            byteCount = elementCount * 4;
          }
        }
        else if (attr.DataTypeName == "unsigned char")
        {
          if (attr.IsSingleValue)
          {
            elementCount = byteCount = 1;
            unpackedValues.Add((attr as Attribute<byte>).UnboxedValue);
          }
          else
          {
            var byteAttr = attr as Attribute<byte[]>;
            unpackedValues.AddRange(byteAttr.UnboxedValue);
            elementCount = byteCount = (uint)byteAttr.UnboxedValue.Length;
          }
        }
        data.AddRange(Uint32ToBytes(byteCount));
        data.AddRange(Uint32ToBytes(elementCount));
        data.AddRange(unpackedValues);
      }
      // TODO add short/ushort
      data.Add(AttrEnd);
      return data.ToArray();
    }

    static byte[] NodeToBytes(Node node)
    {
      List<byte> data = new List<byte>();
      data.Add(NodeBegin);
      data.AddRange(Uint32ToBytes(node.UniqueId));
      data.Add(Convert.ToByte(node.Name.Length + 1));
      data.AddRange(StringToBytes(node.Name));
      data.Add(0);

      data.Add(Convert.ToByte(node.Attributes.Count));
      data.Add(Convert.ToByte(node.Nodes.Count));

      for (int x = 0; x < node.Attributes.Count; x++)
        data.AddRange(AttributeToBytes(node.Attributes[x]));

      for (int x = 0; x < node.Nodes.Count; x++)
        data.AddRange(NodeToBytes(node.Nodes[x]));

      data.Add(NodeEnd);
      return data.ToArray();
    }

    public static byte[] NodesToBytes(Node[] nodes)
    {
      List<byte> data = new List<byte>();
      data.AddRange(MakeByteHeader());
      data.AddRange(Uint16ToBytes((ushort)nodes.Length));
      foreach (var node in nodes)
        data.AddRange(NodeToBytes(node));
      return data.ToArray();
    }
  }
}