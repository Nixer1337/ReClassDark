#include "stdafx.h"

#include <afxtabctrl.h>

#include "CMainFrame.h"
#include "CClassFrame.h"

#include "DialogClasses.h"
#include "DialogProcSelect.h"
#include "DialogTypes.h"

#include "DarkThemeManager.h"

UINT BASED_CODE CMainFrame::s_StatusBarPanes[2] = { 
    ID_STATUSBAR_PANE1,
    ID_STATUSBAR_PANE2
};

// CMainFrame
IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWndEx )
    ON_WM_TIMER( )
    ON_WM_CREATE( )
    ON_WM_SIZE( )
    ON_WM_SETTINGCHANGE( )
    ON_COMMAND( ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager )
    //ON_COMMAND(ID_BUTTON_SHOWCLASSES, &CMainFrame::OnButtonShowclasses)
    ON_COMMAND( ID_CHECK_ADDRESS, &CMainFrame::OnCheckAddress )
    ON_UPDATE_COMMAND_UI( ID_CHECK_ADDRESS, &CMainFrame::OnUpdateCheckAddress )
    ON_COMMAND( ID_CHECK_OFFSET, &CMainFrame::OnCheckOffset )
    ON_UPDATE_COMMAND_UI( ID_CHECK_OFFSET, &CMainFrame::OnUpdateCheckOffset )
    ON_COMMAND( ID_BUTTON_TYPEDEF, &CMainFrame::OnButtonTypedef )
    ON_COMMAND( ID_CHECK_TEXT, &CMainFrame::OnCheckText )
    ON_UPDATE_COMMAND_UI( ID_CHECK_TEXT, &CMainFrame::OnUpdateCheckText )
    ON_COMMAND( ID_CHECK_RTTI, &CMainFrame::OnCheckRtti )
    ON_UPDATE_COMMAND_UI( ID_CHECK_RTTI, &CMainFrame::OnUpdateCheckRtti )
    ON_COMMAND( ID_CHECK_RANDOM_NAME, &CMainFrame::OnCheckRandomWindowName )
    ON_UPDATE_COMMAND_UI( ID_CHECK_RANDOM_NAME, &CMainFrame::OnUpdateCheckRandomWindowName )
    //ON_COMMAND(ID_BUTTON_SELECT, &CMainFrame::OnButtonSelect)
    ON_COMMAND( ID_BUTTON_SELECTPROCESS, &CMainFrame::OnButtonSelectProcess )
    ON_COMMAND( ID_BUTTON_EDITCLASS, &CMainFrame::OnButtonEditClass )
    ON_UPDATE_COMMAND_UI( ID_BUTTON_EDITCLASS, &CMainFrame::OnUpdateButtonEditClass )
    ON_COMMAND( ID_BUTTON_DELETECLASS, &CMainFrame::OnButtonDeleteClass )
    ON_UPDATE_COMMAND_UI( ID_BUTTON_DELETECLASS, &CMainFrame::OnUpdateButtonDeleteClass )
    ON_COMMAND( ID_CHECK_TOPMOST, &CMainFrame::OnCheckTopmost )
    ON_UPDATE_COMMAND_UI( ID_CHECK_TOPMOST, &CMainFrame::OnUpdateCheckTopmost )
    ON_COMMAND( ID_CHECK_CLASSBROWSER, &CMainFrame::OnCheckClassBrowser )
    ON_UPDATE_COMMAND_UI( ID_CHECK_CLASSBROWSER, &CMainFrame::OnUpdateCheckClassBrowser )
    ON_COMMAND( ID_BUTTON_LEFT, &CMainFrame::OnButtonLeft )
    ON_COMMAND( ID_BUTTON_RIGHT, &CMainFrame::OnButtonRight )
    ON_COMMAND( ID_CHECK_FLOAT, &CMainFrame::OnCheckFloat )
    ON_UPDATE_COMMAND_UI( ID_CHECK_FLOAT, &CMainFrame::OnUpdateCheckFloat )
    ON_COMMAND( ID_CHECK_INTEGER, &CMainFrame::OnCheckInteger )
    ON_UPDATE_COMMAND_UI( ID_CHECK_INTEGER, &CMainFrame::OnUpdateCheckInteger )
    ON_COMMAND( ID_CHECK_STRING, &CMainFrame::OnCheckString )
    ON_UPDATE_COMMAND_UI( ID_CHECK_STRING, &CMainFrame::OnUpdateCheckString )
    ON_COMMAND( ID_CHECK_POINTER, &CMainFrame::OnCheckPointer )
    ON_UPDATE_COMMAND_UI( ID_CHECK_POINTER, &CMainFrame::OnUpdateCheckPointer )
    ON_COMMAND( ID_CHECK_UNSIGNEDHEX, &CMainFrame::OnCheckUnsignedHex )
    ON_UPDATE_COMMAND_UI( ID_CHECK_UNSIGNEDHEX, &CMainFrame::OnUpdateCheckUnsignedHex )
    ON_COMMAND( ID_CHECK_CLIP_COPY, &CMainFrame::OnCheckClipboardCopy )
    ON_UPDATE_COMMAND_UI( ID_CHECK_CLIP_COPY, &CMainFrame::OnUpdateCheckClipboardCopy )
    ON_COMMAND( ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnCheckPrivatePadding )
    ON_UPDATE_COMMAND_UI( ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnUpdateCheckPrivatePadding )
END_MESSAGE_MAP( )

// CMainFrame construction/destruction
CMainFrame::CMainFrame( )
{
}

CMainFrame::~CMainFrame( )
{
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if (CMDIFrameWndEx::OnCreate( lpCreateStruct ) == -1)
        return -1;

    //
    // Create tabs
    //
    CMDITabInfo MdiTabParams;
    MdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;
    MdiTabParams.m_tabLocation = CMFCTabCtrl::LOCATION_TOP;
    MdiTabParams.m_bActiveTabCloseButton = TRUE;
    MdiTabParams.m_bTabCloseButton = TRUE;
    MdiTabParams.m_nTabBorderSize = 2;
    MdiTabParams.m_bTabIcons = FALSE;
    MdiTabParams.m_bAutoColor = FALSE;
    MdiTabParams.m_bDocumentMenu = TRUE;
    MdiTabParams.m_bEnableTabSwap = TRUE;
    MdiTabParams.m_bFlatFrame = TRUE;
    EnableMDITabbedGroups( TRUE, MdiTabParams );

    //
    // Create ribbon bar
    //
    m_RibbonBar.Create( this );
    m_RibbonBar.LoadFromResource( IDR_RIBBON );

    //
    // Create status bar
    //
    m_StatusBar.Create( this );
    m_StatusBar.SetIndicators( s_StatusBarPanes, 2 );
    m_StatusBar.SetPaneInfo( 0, ID_STATUSBAR_PANE1, SBPS_NORMAL, 0 );
    m_StatusBar.SetPaneInfo( 1, ID_STATUSBAR_PANE2, SBPS_STRETCH, 0 );
    m_StatusBar.SetPaneBackgroundColor(0, RGB(31, 31, 31));
    m_StatusBar.SetPaneBackgroundColor(1, RGB(31, 31, 31));

    //
    // Enable Visual Studio 2005 style docking window behavior
    //
    CDockingManager::SetDockingMode( DT_SMART );
    EnableAutoHidePanes( CBRS_ALIGN_TOP );

    //
    // Create docking windows
    //
    if (!CreateDockingWindows( ))
    {
        PrintOut( _T( "Failed to create docking windows\n" ) );
        return -1;
    }

    //
    // Generate random window name
    //
    if (g_bRandomName)
    {
        TCHAR tcsRandomTitle[16];
        Utils::GenerateRandomString( tcsRandomTitle, ARRAYSIZE( tcsRandomTitle ) );
        SetTitle( tcsRandomTitle );
    }
    else
    {
        SetTitle( _T( "ReClassEx" ) );
    }

    //
    // Enable enhanced windows management dialog
    //
    EnableWindowsDialog( ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE );

    //
    // Switch the order of document name and application name on the window title bar. 
    // This improves the usability of the taskbar because the document name is visible with the thumbnail.
    //
    ModifyStyle( 0, FWS_PREFIXTITLE );

    SetTimer( TIMER_MEMORYMAP_UPDATE, 30, NULL );

    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCDarkThemeManager));

    return 0;
}

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
    CMDIFrameWndEx::OnSize( nType, cx, cy );

    RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_STATUSBAR_PANE2 );

    if (m_StatusBar.GetSafeHwnd( ) && cx > 250)
        m_StatusBar.SetPaneInfo( 0, ID_STATUSBAR_PANE1, SBPS_NORMAL, cx - 250 );
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
    if (!CMDIFrameWndEx::PreCreateWindow( cs ))
        return FALSE;

    if (g_bTop)
        cs.dwExStyle |= WS_EX_TOPMOST;

    return TRUE;
}

BOOL CMainFrame::CreateDockingWindows( )
{
    SetDockingWindowIcons( g_ReClassApp.m_bHiColorIcons );

    return TRUE;
}

void CMainFrame::SetDockingWindowIcons( BOOL bHiColorIcons )
{
    UpdateMDITabbedBarsIcons( );
}

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid( ) const
{
    CMDIFrameWndEx::AssertValid( );
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
    CMDIFrameWndEx::Dump( dc );
}
#endif //_DEBUG

// CMainFrame message handlers
void CMainFrame::OnWindowManager( )
{
    ShowWindowsDialog( );
}

void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
    CMDIFrameWndEx::OnSettingChange( uFlags, lpszSection );
}

BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
    CCmdUI* pCmdUi;
    CClassFrame *pChildClassFrame;
    CNodeClass *pClass;

    if (nCode == CN_UPDATE_COMMAND_UI)
    {
        pCmdUi = (CCmdUI*)pExtra;
        if (nID >= WM_CLASSMENU && nID < (WM_CLASSMENU + WM_MAXITEMS))
        {
            pCmdUi->Enable( TRUE );
            return TRUE;
        }
        else if (nID >= WM_PROCESSMENU && nID < (WM_PROCESSMENU + WM_MAXITEMS))
        {
            pCmdUi->Enable( TRUE );
            return TRUE;
        }
        else if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS))
        {
            pCmdUi->Enable( TRUE );
            return TRUE;
        }
    }

    if (nCode == CN_COMMAND)
    {
        if ((nID >= WM_CLASSMENU) && (nID < (WM_CLASSMENU + WM_MAXITEMS)))
        {       
            pChildClassFrame = STATIC_DOWNCAST( CClassFrame, 
                CreateNewChild( RUNTIME_CLASS( CClassFrame ), IDR_ReClassExTYPE, g_ReClassApp.m_hMdiMenu, g_ReClassApp.m_hMdiAccel ) );

            pClass = g_ReClassApp.m_Classes[nID - WM_CLASSMENU];

            pChildClassFrame->SetTitle( pClass->GetName( ) );
            pChildClassFrame->SetWindowText( pClass->GetName( ) );
            pChildClassFrame->SetClass( pClass );

            pClass->SetChildClassFrame( pChildClassFrame );

            UpdateFrameTitleForDocument( pClass->GetName( ) );

            return TRUE;
        }

        if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS))
        {
            pClass = g_ReClassApp.m_Classes[nID - WM_DELETECLASSMENU];

            g_ReClassApp.DeleteClass( pClass );

            return TRUE;
        }
    }

    return CMDIFrameWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CMainFrame::OnCheckAddress( )
{
    g_bAddress = !g_bAddress;
}

void CMainFrame::OnUpdateCheckAddress( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bAddress );
}

void CMainFrame::OnCheckOffset( )
{
    g_bOffset = !g_bOffset;
}

void CMainFrame::OnUpdateCheckOffset( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bOffset );
}

void CMainFrame::OnCheckText( )
{
    g_bText = !g_bText;
}

void CMainFrame::OnUpdateCheckText( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bText );
}

void CMainFrame::OnCheckRtti( )
{
    g_bRTTI = !g_bRTTI;
}

void CMainFrame::OnUpdateCheckRtti( CCmdUI *pCmdUI )
{
    if (!g_bPointers) 
    {
        pCmdUI->Enable( FALSE );
    }
    else 
    {
        pCmdUI->Enable( TRUE );
        pCmdUI->SetCheck( g_bRTTI );
    }
}

void CMainFrame::OnCheckRandomWindowName( )
{
    g_bRandomName = !g_bRandomName;

    if (g_bRandomName) 
    {
        TCHAR tcsRandomTitle[16];
        Utils::GenerateRandomString( tcsRandomTitle, ARRAYSIZE( tcsRandomTitle ) );
        SetTitle( tcsRandomTitle );
    } 
    else 
    {
        SetTitle( _T( "ReClassEx" ) );
    }

    //
    // Invalidate window to force a change to the window text
    //
    Invalidate( );
}

void CMainFrame::OnUpdateCheckRandomWindowName( CCmdUI * pCmdUI )
{
    pCmdUI->SetCheck( g_bRandomName );
}

void CMainFrame::OnButtonTypedef( )
{
    CDialogTypes dlg( this );
    dlg.DoModal( );
}

void CMainFrame::OnButtonSelectProcess( )
{
    CDialogProcSelect ProcSelectDialog( this );
    ProcSelectDialog.DoModal( );
}

void CMainFrame::OnButtonEditClass( )
{
    if (g_bClassBrowser)
    {
        CDialogClasses ClassesDialog( this );
        ClassesDialog.DoModal( );
    }
    else
    {
        CMFCRibbonBaseElement* pButton = m_RibbonBar.FindByID( ID_BUTTON_EDITCLASS );
        CRect pos = pButton->GetRect( );
        ClientToScreen( &pos );

        CMenu menu;
        menu.CreatePopupMenu( );

        for (size_t i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
        {
            CString MenuItem;
            MenuItem.Format( _T( "%i - %s" ), i, g_ReClassApp.m_Classes[i]->GetName( ).GetString( ) );
            menu.AppendMenu( MF_STRING | MF_ENABLED, WM_CLASSMENU + i, MenuItem );
        }

        menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this );
    }
}

void CMainFrame::OnUpdateButtonEditClass( CCmdUI *pCmdUI )
{
    pCmdUI->Enable( (g_ReClassApp.m_Classes.size( ) > 0) );
}

void CMainFrame::OnButtonDeleteClass( )
{
    CMenu Menu;
    CMFCRibbonBaseElement* pDeleteButton = m_RibbonBar.FindByID( ID_BUTTON_DELETECLASS );
    CRect DeleteButtonPos = pDeleteButton->GetRect( );
    
    ClientToScreen( &DeleteButtonPos );

    Menu.CreatePopupMenu( );

    for (size_t i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
    {
        Menu.AppendMenu( MF_STRING | MF_ENABLED, WM_DELETECLASSMENU + i, 
            g_ReClassApp.m_Classes[i]->GetName( ) );
    }

    Menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, 
        DeleteButtonPos.left, DeleteButtonPos.bottom, this );
}

void CMainFrame::OnUpdateButtonDeleteClass( CCmdUI *pCmdUI )
{
    pCmdUI->Enable( (g_ReClassApp.m_Classes.size( ) > 0) ? TRUE : FALSE );
}

void CMainFrame::OnTimer( UINT_PTR nIDEvent )
{
    if (nIDEvent == TIMER_MEMORYMAP_UPDATE)
        UpdateMemoryMap( );

    CMDIFrameWndEx::OnTimer( nIDEvent );
}

void CMainFrame::OnCheckTopmost( )
{
    g_bTop = !g_bTop;
    SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, 
        0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
}

void CMainFrame::OnUpdateCheckTopmost( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bTop );
}

void CMainFrame::OnCheckClassBrowser( )
{
    g_bClassBrowser = !g_bClassBrowser;
}

void CMainFrame::OnUpdateCheckClassBrowser( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bClassBrowser );
}

void CMainFrame::OnCheckPrivatePadding( )
{
    g_bPrivatePadding = !g_bPrivatePadding;
}

void CMainFrame::OnUpdateCheckPrivatePadding( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bPrivatePadding );
}

void CMainFrame::OnCheckClipboardCopy( )
{
    g_bClipboardCopy = !g_bClipboardCopy;
}

void CMainFrame::OnUpdateCheckClipboardCopy( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bClipboardCopy );
}

// Multi monitor support. Thanks timboy67678
void CMainFrame::OnButtonLeft( )
{
    RECT WindowRect;
    LONG MonitorWidth, MonitorHeight;
    HMONITOR Monitor;
    MONITORINFO MonitorInfo;

    ZeroMemory( &MonitorInfo, sizeof( MONITORINFO ) );
    MonitorInfo.cbSize = sizeof( MONITORINFO );

    ::GetWindowRect( GetSafeHwnd( ), &WindowRect );

    Monitor = ::MonitorFromRect( &WindowRect, MONITOR_DEFAULTTONEAREST );
    ::GetMonitorInfo( Monitor, &MonitorInfo );

    MonitorWidth = MonitorInfo.rcWork.right - MonitorInfo.rcWork.left;
    MonitorHeight = MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top;

    SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, 
        MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top, MonitorWidth >> 1, MonitorHeight, SWP_NOZORDER );
}

void CMainFrame::OnButtonRight( )
{
    RECT WindowRect;
    LONG MonitorWidth, MonitorHeight;
    HMONITOR Monitor;
    MONITORINFO MonitorInfo;

    ZeroMemory( &MonitorInfo, sizeof( MONITORINFO ) );
    MonitorInfo.cbSize = sizeof( MONITORINFO );

    ::GetWindowRect( GetSafeHwnd( ), &WindowRect );

    Monitor = ::MonitorFromRect( &WindowRect, MONITOR_DEFAULTTONEAREST );
    ::GetMonitorInfo( Monitor, &MonitorInfo );

    MonitorWidth = MonitorInfo.rcWork.right - MonitorInfo.rcWork.left;
    MonitorHeight = MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top;

    SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, 
        MonitorInfo.rcMonitor.left + (MonitorWidth >> 1), 0, MonitorWidth >> 1, MonitorHeight, SWP_NOZORDER );
}

void CMainFrame::OnCheckFloat( )
{
    g_bFloat = !g_bFloat;
}

void CMainFrame::OnUpdateCheckFloat( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bFloat );
}

void CMainFrame::OnCheckInteger( )
{
    g_bInt = !g_bInt;
}

void CMainFrame::OnUpdateCheckInteger( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bInt );
}

void CMainFrame::OnCheckString( )
{
    g_bString = !g_bString;
}

void CMainFrame::OnUpdateCheckString( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bString );
}

void CMainFrame::OnCheckPointer( )
{
    g_bPointers = !g_bPointers;
}

void CMainFrame::OnUpdateCheckPointer( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bPointers );
}

void CMainFrame::OnCheckUnsignedHex( )
{
    g_bUnsignedHex = !g_bUnsignedHex;
}

void CMainFrame::OnUpdateCheckUnsignedHex( CCmdUI *pCmdUI )
{
    pCmdUI->SetCheck( g_bUnsignedHex );
}