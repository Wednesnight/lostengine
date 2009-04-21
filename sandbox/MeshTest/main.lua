function initShaders(loader)
  log.debug("initShaders")
  shader = lost.gl.loadShader(loader, "light")
  return shader
end