#ifndef SPHERE_H
#define SPHERE_H


inline void zerocheck(float& v)
{
  static const float zerothresh = 0.0001;
  if((v<zerothresh) && (v>-zerothresh))
  {
    v = 0.0f;
  }
}

struct Sphere
{
    lost::math::Vec3 pos;
    float            radius;    
    
    bool intersects(const Ray& r, float& t) const
    {
      lost::math::Vec3 sc = pos - r.pos;
      float v = sc * r.dir;
      // FIXME: keep radiussquared as precalculated memeber to save mult
      float disc = (radius *radius) - ((sc*sc) - v*v);

      if(!(disc < 0.0f))
      {
        float d = sqrtf(disc);
        float d1 = v-d;
        float d2 = v+d;
        
        zerocheck(d1);
        zerocheck(d2);
        
        if(d1 > 0.0f)
        {
          t = d1;
        }
        else if (d2 > 0.0f)
        {
          t = d2;
        }
        return true;
      }
      else
      {
        return false;
      }    
    }
    
    lost::math::Vec3 normalAt(const lost::math::Vec3& inPos) const
    {
      lost::math::Vec3 result = inPos - pos;
      normalise(result);
      return result;
    }
    
};

#endif