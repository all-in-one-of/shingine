namespace Shingine
{
  struct uid
  {
    public uint Value;
    public uid(uint v) { Value = v; }
    public static implicit operator uint(uid d)
    {
      return d.Value;
    }
    public static implicit operator uid(uint d)
    {
      return new uid(d);
    }
  }
}