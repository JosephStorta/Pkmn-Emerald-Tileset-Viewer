#ifndef VIEWER_GUI_APP_H_
#define VIEWER_GUI_APP_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

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
    wxScrolledWindow* m_tileset_scroll { nullptr };
    wxStaticBitmap* m_tileset_bitmap { nullptr };

    void create_menu_bar();

    // Menu bar item methods
    void on_open(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_APP_H_
