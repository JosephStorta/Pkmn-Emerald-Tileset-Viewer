#ifndef VIEWER_GUI_TILESET_VIEW_H_
#define VIEWER_GUI_TILESET_VIEW_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

#include "data/tileset.h"

namespace viewer {
namespace gui {

/**
 * @brief Custom panel for viewing tilesets.
 */
class TilesetView : public wxPanel
{
public:
    TilesetView(wxWindow* parent);

    void load_tileset();

private:
    data::Tileset m_tileset;
    wxImage m_tileset_image { wxNullImage };

    wxStaticBoxSizer* m_tileset_view_sizer { nullptr };

    wxPanel* m_tileset_panel { nullptr };
    wxStaticBitmap* m_tileset_bitmap { nullptr };

    wxCheckBox* m_palette_check { nullptr };
    wxSpinCtrl* m_palette_spinbox { nullptr };

    // Event methods
    void on_palette_check(wxCommandEvent& event);
    void on_palette_spin(wxSpinEvent& event);

    void update_spinbox();
    void update_palette();

    // GUI creation methods
    void create_gui();
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_TILESET_VIEW_H_
