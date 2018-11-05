using System;
using System.Collections.Generic;

namespace Shingine
{
  public interface IAttribute
  {
    string Name { get; }
    string DataTypeName { get; }
    object Value { get; }
    bool IsSingleValue { get; }
  }
  public class Attribute<T> : IAttribute
  {
    public Attribute(string name, T value, bool singleElement = false)
    {
      _name = name;
      _value = value;
      _singleElement = singleElement;
      _dataType = GetTypeName(typeof(T));
    }
    public string Name { get { return _name; } }
    public string DataTypeName { get { return _dataType; } }
    public object Value { get { return (object)_value; } }
    public T UnboxedValue { get { return _value; } }
    public bool IsSingleValue { get { return _singleElement; } }
    string _name;
    string _dataType;
    bool _singleElement;
    T _value;
    static readonly Dictionary<Type, string> _typeDict = new Dictionary<Type, string>() {
      {typeof(string), "char"},
      {typeof(string[]), "char"},
      {typeof(byte), "unsigned char"},
      {typeof(int), "int"},
      {typeof(uint), "unsigned int"},
      {typeof(uint[]), "unsigned int"},
      {typeof(ushort), "unsigned short"},
      {typeof(short), "short"},
      {typeof(float), "float"},
      {typeof(float[]), "float"},
      {typeof(uid), "uid"},
      {typeof(uid[]), "uid"},
      {typeof(Node[]), "SerializedClass"}
    };
    static string GetTypeName(System.Type type)
    {
      if (!_typeDict.ContainsKey(type))
        throw new TypeLoadException();
      return _typeDict[type];
    }
  }
}
