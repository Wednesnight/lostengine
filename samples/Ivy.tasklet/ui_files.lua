local Color = lost.common.Color
local Vec2 = lost.math.Vec2
fileListWidth = 250

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.ListView
    {
      id="fileList",
      size="small",
      sublayers={lost.guiro.layer.Rect{filled=true,color=Color(1,1,1)}},
      bounds={0,0,fileListWidth,"1"},
      delegate=fileListDelegate,
      dataSource=fileListDatasource
    },
    lost.guiro.view.View
    {
      bounds={fileListWidth,0,{"1",-fileListWidth},"1"},
      subviews=
      {
        require("ui_files_empty"),
        require("ui_files_preview")
      }
    }
  }
}