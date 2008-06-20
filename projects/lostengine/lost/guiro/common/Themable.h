#ifndef LOST_GUIRO_COMMON_THEMABLE_H
#define LOST_GUIRO_COMMON_THEMABLE_H

#include "lost/object/ObjectReference.h"
#include "lost/guiro/common/Theme.h"

namespace lost
{
  namespace guiro
  {
    namespace common
    {

      struct Themable
      {
      protected:
        object::ObjectReference<common::Theme> mTheme;
      public:
        object::ObjectReference<common::Theme>& theme()
        {
          return mTheme;
        }

      };

    }
  } // namespace guiro
} // namespace lost

#endif

