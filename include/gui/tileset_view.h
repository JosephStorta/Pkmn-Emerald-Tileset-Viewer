#ifndef VIEWER_GUI_TILESET_VIEW_H_
#define VIEWER_GUI_TILESET_VIEW_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "data/tileset.h"

namespace viewer {
namespace gui {

class TilesetView : public wxStaticBoxSizer
{
public:
    TilesetView(wxStaticBox* box, int orient);

    void load_tileset();

private:
    data::Tileset m_tileset;

    wxScrolledWindow* m_tileset_scroll { nullptr };
    wxStaticBitmap* m_tileset_bitmap { nullptr };

    void create_gui();
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_TILESET_VIEW_H_
