#pragma once
class ISystem {
public:
  virtual ~ISystem(){};
  virtual bool Initialize() = 0;
  virtual bool Update() = 0;

  virtual void SetActive(bool active) = 0;
  virtual bool IsActive() = 0;
};
