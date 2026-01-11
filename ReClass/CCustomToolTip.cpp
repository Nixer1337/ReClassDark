#include "stdafx.h"

#include "CCustomToolTip.h"

BEGIN_MESSAGE_MAP( CCustomToolTip, CWnd )
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP( )

CCustomToolTip::CCustomToolTip()
    : m_bUseFormattedText( false )
    , m_brBackGnd( NULL )
    , m_pParentWnd( NULL )
{
}

CCustomToolTip::~CCustomToolTip()
{
    if (m_brBackGnd)
        DeleteObject( m_brBackGnd );
}

BOOL CCustomToolTip::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
    m_brBackGnd = CreateSolidBrush( g_clrBackground );
    m_pParentWnd = pParentWnd;
    
    LPCTSTR lpszClassName = AfxRegisterWndClass( 
        CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS, 
        ::LoadCursor( NULL, IDC_ARROW ),
        (HBRUSH)m_brBackGnd,
        NULL );
    
    // Create as popup window to avoid z-order conflicts with scrollbars
    return CWnd::CreateEx( 
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        lpszClassName, 
        NULL, 
        WS_POPUP | WS_BORDER, 
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        pParentWnd->GetSafeHwnd(), 
        NULL );
}

void CCustomToolTip::SetWindowText( LPCTSTR lpszString )
{
    m_SimpleText = lpszString;
    m_bUseFormattedText = false;
    if (IsWindow( m_hWnd ))
        Invalidate();
}

void CCustomToolTip::SetFormattedText( const std::vector<TooltipLine>& lines )
{
    m_Lines = lines;
    m_bUseFormattedText = true;
    if (IsWindow( m_hWnd ))
        Invalidate();
}

void CCustomToolTip::ClearFormattedText()
{
    m_Lines.clear();
    m_bUseFormattedText = false;
}

void CCustomToolTip::SetPositionAndSize( int clientX, int clientY, int width, int height )
{
    if (m_pParentWnd == NULL)
        return;
        
    // Convert client coordinates to screen coordinates
    CPoint screenPt( clientX, clientY );
    m_pParentWnd->ClientToScreen( &screenPt );
    
    SetWindowPos( &wndTopMost, screenPt.x, screenPt.y, width, height, SWP_NOACTIVATE );
}

BOOL CCustomToolTip::OnEraseBkgnd( CDC* pDC )
{
    // Don't erase - we'll draw everything in OnPaint with double buffering
    return TRUE;
}

void CCustomToolTip::OnPaint()
{
    CPaintDC dc( this );
    
    CRect clientRect;
    GetClientRect( &clientRect );
    
    // Double buffering to prevent flickering
    CDC memDC;
    memDC.CreateCompatibleDC( &dc );
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap( &dc, clientRect.Width(), clientRect.Height() );
    CBitmap* pOldBitmap = memDC.SelectObject( &memBitmap );
    
    // Draw background
    memDC.FillSolidRect( &clientRect, g_clrBackground );
    
    memDC.SetBkMode( TRANSPARENT );
    CFont* pOldFont = memDC.SelectObject( &g_ViewFont );
    
    int x = 4;
    int y = 2;
    
    if (m_bUseFormattedText)
    {
        for (size_t i = 0; i < m_Lines.size(); i++)
        {
            x = 4;
            for (size_t j = 0; j < m_Lines[i].segments.size(); j++)
            {
                memDC.SetTextColor( m_Lines[i].segments[j].color );
                memDC.TextOut( x, y, m_Lines[i].segments[j].text );
                x += m_Lines[i].segments[j].text.GetLength() * g_FontWidth;
            }
            y += g_FontHeight;
        }
    }
    else
    {
        // Simple text fallback
        memDC.SetTextColor( g_clrHex );
        
        CString text = m_SimpleText;
        text.Replace( _T("\r\n"), _T("\n") );
        
        int start = 0;
        int len = text.GetLength();
        
        for (int i = 0; i <= len; i++)
        {
            if (i == len || text[i] == _T('\n'))
            {
                CString line = text.Mid( start, i - start );
                memDC.TextOut( 4, y, line );
                y += g_FontHeight;
                start = i + 1;
            }
        }
    }
    
    memDC.SelectObject( pOldFont );
    
    // Copy buffer to screen
    dc.BitBlt( 0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY );
    
    memDC.SelectObject( pOldBitmap );
}