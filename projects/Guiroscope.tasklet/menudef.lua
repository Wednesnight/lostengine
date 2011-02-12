-- menu items
return
  {
    {
      title="File", 
      menu={ -- menu constructor 
        id="m1",
        items=
        {
          {title="m1 First"},
          {title="m1 Second"},
          "-",
          {title="m1 Third"},              
          {title="m1 Fourth"},              
        }
      }
    },
    {
      title="Edit", 
      menu={
        id="m2",
        items=
        {
          {
            title="m2 First",
            menu=
            {
              items=
              {
               {title="sm1 First"},
               {title="sm1 Second"},
               {
                 title="sm1 Third",
                 menu=
                 {
                   items=
                   {
                     {title="sm11 First"},
                     {title="sm11 Second"},                       
                     {title="sm11 Third"},                       
                   }
                 }
               },              
               {
                 title="sm1 Fourth",
                 menu=
                 {
                   items=
                   {
                     {title="sm12 First"},
                     {title="sm12 Second"},                       
                   }
                 }                     
               },              
               {title="sm1 Fifth"},
              }
            }                             
          },
          {title="m2 Second"},
          "-",
          {title="m2 Third"},              
          {title="m2 Fourth"},              
          "-",
          {title="m2 Fifth"},              
        },
      }
    },
    {
      title="Long Menu Name", 
      menu={
        id="m3",
        items=
        {
          {title="m3 First"},
          {title="m3 Second"},
          "-",
          {title="m3 Third"},              
          {title="m3 Fourth"},              
          "-",
          {title="m3 Fifth"},              
          {title="m3 Sixth"},              
          {title="m3 Seventh"},              
        }
      }
    },
    {
      title="Short Again", 
      menu={
        id="m4",        
        items=
        {
          {title="m4 First"},
          {title="m4 Second"},
          {title="m4 Third"},              
        }
      }
    },
  }

