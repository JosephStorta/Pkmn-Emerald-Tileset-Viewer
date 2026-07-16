#ifndef VIEWER_GUI_METATILE_VIEW_H_
#define VIEWER_GUI_METATILE_VIEW_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "data/tileset.h"

namespace viewer {
namespace gui {

/**
 * @brief Custom panel for viewing tilesets.
 */
class MetatileView : public wxPanel
{
public:
    MetatileView(wxWindow* parent);

    void load_metatiles(const data::Tileset* tileset);

private:
    wxImage m_metatile_image { wxNullImage };

    wxStaticBoxSizer* m_metatile_view_sizer { nullptr };

    wxPanel* m_bitmap_panel { nullptr };
    wxStaticBitmap* m_metatile_bitmap { nullptr };

    // GUI creation methods
    void create_gui();
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_METATILE_VIEW_H_
