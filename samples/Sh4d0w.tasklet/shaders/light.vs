float calculateLightIntensity(in vec4 pos, in vec4 normal, in mat4 modelViewMatrix, in vec3 lightPosition)
{
  vec3 ecpos = vec3(modelViewMatrix * pos);
  vec3 lightVec = normalize(lightPosition - ecpos);
  vec3 tnorm = vec3(normalize(modelViewMatrix * normal));
  return max(dot(lightVec, tnorm), 0.0);
}
