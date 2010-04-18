function rrgrad(rect, filled, radius, lineWidth, gcoord, cc)
  local result = lost.mesh.RoundedRect.create(textureManager, Vec2(rect.width, rect.height), filled, radius, lineWidth);
  result.material.blend = true
  result.material.shader = lost.common.Shaders.gradientShader()
  result.transform = MatrixTranslation(Vec3(rect.x, rect.y, 0))
  result.material.uniforms = lost.gl.UniformBlock.create()
  result.material.uniforms:setFloat("gradientCoord", gcoord)
  result.material:add(textureManager.gradientTexture)

  if cc then
    log.debug("----------- CC")
    for k,v in pairs(cc) do
      print(k.." "..tostring(v))
    end
    
    local blr = (cc.blr ~= nil) and cc.blr or true
    local brr = cc.brr and cc.brr or true
    local tlr = cc.tlr and cc.tlr or true
    local trr = cc.trr and cc.trr or true
    log.debug(tostring(blr).." "..tostring(brr).." "..tostring(tlr).." "..tostring(trr))
    result:roundCorners(cc.blr, cc.brr, cc.tlr, cc.trr)
  end

  return result
end