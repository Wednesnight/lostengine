return lost.guiro.view.View
{
  subviews=
  {
    lost.guiro.view.TabView
    {    
      bounds = {0,0,"1",{"1",-8}},
      style="candyRecess",
      size="mini",
      buttonSizeAdjust = 20,
      items={
             {"Label",require("views_label")},
             {"Button",require("views_button")},
             {"TabBar",require("views_tabbar")},
             {"TabView",require("views_tabview")},
             {"Image",require("views_image")},
             {"Window",require("views_window")},
             {"Box", require("views_box")},
             {"RadioGroup",require("views_radiogroup")},
             {"Slider",require("views_slider")},
             {"ColorPicker",require("views_colorpicker")},
             {"TextInput",require("views_textinput")},
             {"MenuBar",require("views_menus")},             
             },
    }
  }
}
