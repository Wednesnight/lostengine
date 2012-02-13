local t = require("lost/fontconfig")
-- only support ttf fonts for now
for k,relativeFontBundlePath in pairs(t) do
  log.debug(relativeFontBundlePath)
  local fontbundle = tasklet.applicationBundle:subBundle(lost.fs.Path(relativeFontBundlePath))
  local fontmeta = fontbundle:require(lost.fs.Path("meta.lua"))
  for idx, font in pairs(fontmeta.fonts) do
--    log.debug(font.name)
    tasklet.fontManager:addEntry(font.name,fontbundle,font.ttfile)
--    log.debug((fontbundle.repo.rootDirectory / font.ttfile):string())
--    log.debug(fontbundle.repo.rootDirectory)
  end
end

