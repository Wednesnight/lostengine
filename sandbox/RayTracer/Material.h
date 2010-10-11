#ifndef MATERIAL_H
#define MATERIAL_H

#include "lost/common/Color.h"

using namespace lost::common;

struct Material {

protected:

  Color m_color;
  float m_diffuse;
  float m_specular;
  float m_reflection;
  float m_refraction;
  float m_refractionIndex;

public:

  Material()
  : m_color(0.2f, 0.2f, 0.2f),
    m_diffuse(0.2f),
    m_specular(0.8f),
    m_reflection(0.0f),
    m_refraction(0.0f),
    m_refractionIndex(1.5f)
  {}

  void color(Color color) {
    m_color = color;
  }

  Color color() {
    return m_color;
  }

  void diffuse(float diffuse) {
    m_diffuse = diffuse;
  }

  float diffuse() {
    return m_diffuse;
  }

  void reflection(float reflection) {
    m_reflection = reflection;
  }

  float reflection() {
    return m_reflection;
  }

  void refraction(float refraction) {
    m_refraction = refraction;
  }
  
  float refraction() {
    return m_refraction;
  }
  
  void refractionIndex(float refractionIndex) {
    m_refractionIndex = refractionIndex;
  }
  
  float refractionIndex() {
    return m_refractionIndex;
  }
  
  void specular(float specular) {
    m_specular = specular;
  }
  
  float specular() {
    return m_specular;
  }

};

#endif
