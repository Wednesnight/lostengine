local Color = lost.common.Color

return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.ListView
    {
      sublayers={lost.guiro.layer.Rect{filled=true,color=Color(1,1,1)}},
      bounds={0,0,200,"1"},
      delegate=fileListDelegate,
      dataSource=fileListDatasource
    }
  }
}