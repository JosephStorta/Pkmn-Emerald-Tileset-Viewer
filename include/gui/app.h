#ifndef VIEWER_GUI_APP_H_
#define VIEWER_GUI_APP_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "gui/tileset_view.h"

namespace viewer {
namespace gui {

/**
 * @brief Serves as the base for the GUI.
 */
class App : public wxApp
{
public:
    virtual bool OnInit();
};

/**
 * @brief Acts as the main GUI window.
 */
class MainFrame : public wxFrame
{
public:
    MainFrame();
 
private:
    TilesetView* m_tileset_view { nullptr };

    // Event methods
    void on_open(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);

    // GUI creation methods
    void create_gui();
    void create_menu_bar();
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_APP_H_
