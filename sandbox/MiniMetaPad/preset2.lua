local Vec3 = lost.math.Vec3


-- you must specify a type if the value is a float/int/bool, all others can be derived automatically (vec2/3/4,matrix)
return {
  name="preset 2",
  {"drama",80,"float"},
  {"ambience",1,"float"},
  {"pos1",Vec3(.2,.2,.2)},
  {"rad1",.2,"float"},
  {"pos2",Vec3(.7,.7,.7)},
  {"rad2",.045,"float"},  
}
