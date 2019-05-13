#ifndef __OBJECT_BASE_H__
#define __OBJECT_BASE_H__

#include <ArduinoSTL.h>


/*
  @class ObjectBase
  @brief Classe abstraite definissant les objets du sysème  
*/
class ObjectBase
{
private:

protected:
  String  m_name;                     // Nom de l'objet

public:
  // Constructeur
  ObjectBase(const String& name) :
    m_name(name)
    {}

  // Destructeur
  virtual ~ObjectBase() {}

  // Lire le nom de l'objet
  String GetName() const
    { return m_name; }
};

#endif  // __OBJECT_BASE_H__
