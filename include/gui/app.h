#ifndef VIEWER_GUI_APP_H_
#define VIEWER_GUI_APP_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

namespace viewer {
namespace gui {

class App : public wxApp
{
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
    MainFrame();
 
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

} // namespace gui
} // namespace viewer

#endif // VIEWER_GUI_APP_H_
