local Color = lost.common.Color
local Vec2 = lost.math.Vec2
fileListWidth = 200

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.ListView
    {
      sublayers={lost.guiro.layer.Rect{filled=true,color=Color(1,1,1)}},
      bounds={0,0,fileListWidth,"1"},
      delegate=fileListDelegate,
      dataSource=fileListDatasource
    },
    require("ui_files_empty")
  }
}