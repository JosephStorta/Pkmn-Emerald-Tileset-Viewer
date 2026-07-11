#include "gui/app.h"

#include <logger/logger.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

namespace viewer {
namespace gui {

SET_LOG_MODULE("GUI");

bool App::OnInit()
{
    LOG_INFO("Initializing GUI...");

    MainFrame* frame { new MainFrame() };
    frame->Show();
    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    LOG_DEBUG("Creating main frame...");
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(1, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, &MainFrame::OnHello, this, 1);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}
 
void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

} // namespace gui
} // namespace viewer
