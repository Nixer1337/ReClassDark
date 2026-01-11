#pragma once

#include <afxwin.h>
#include <vector>

struct TooltipTextSegment {
    CString text;
    COLORREF color;
    
    TooltipTextSegment() : color(0) {}
    TooltipTextSegment( const CString& t, COLORREF c ) : text(t), color(c) {}
    TooltipTextSegment( LPCTSTR t, COLORREF c ) : text(t), color(c) {}
};

struct TooltipLine {
    std::vector<TooltipTextSegment> segments;
    
    void Add( const CString& text, COLORREF color ) {
        segments.push_back( TooltipTextSegment( text, color ) );
    }
    void Add( LPCTSTR text, COLORREF color ) {
        segments.push_back( TooltipTextSegment( text, color ) );
    }
};

class CCustomToolTip : public CWnd {
public:
    CCustomToolTip();
    virtual ~CCustomToolTip();

    virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

    // Legacy method for simple text (for compatibility)
    void SetWindowText( LPCTSTR lpszString );
    
    // New method for colored text
    void SetFormattedText( const std::vector<TooltipLine>& lines );
    void ClearFormattedText();
    
    // Position tooltip at client coordinates (converts to screen internally)
    void SetPositionAndSize( int clientX, int clientY, int width, int height );

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd( CDC* pDC );

private:
    std::vector<TooltipLine> m_Lines;
    CString m_SimpleText;
    bool m_bUseFormattedText;
    HBRUSH m_brBackGnd;
    CWnd* m_pParentWnd;
};