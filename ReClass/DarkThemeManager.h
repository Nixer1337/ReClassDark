#pragma once

class CMFCDarkThemeManager : public CMFCVisualManagerOffice2007
{
    DECLARE_DYNCREATE(CMFCDarkThemeManager);

    static void Init(CWnd* pWnd, bool fDarkMode);

private:
    CMFCDarkThemeManager();

    virtual void OnUpdateSystemColors();

    virtual void OnDrawRibbonCategory(CDC* pDC, CMFCRibbonCategory* pCategory, CRect rectCategory)
    {
        pDC->FillRect(rectCategory, &(GetGlobalData()->brBarFace));
    }

    virtual BOOL DrawTextOnGlass(CDC* pDC, CString strText, CRect rect, DWORD dwFlags, int nGlowSize, COLORREF clrText)
    {
        return CMFCVisualManagerOffice2007::DrawTextOnGlass(pDC, strText, rect, dwFlags, nGlowSize, RGB(220, 220, 220));
    }

    virtual void OnFillButtonInterior(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
    {
        CMFCVisualManagerOffice2003::OnFillButtonInterior(pDC, pButton, rect, state);
    }

    virtual void OnHighlightMenuItem(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText)
    {
        CMFCVisualManagerOffice2003::OnHighlightMenuItem(pDC, pButton, rect, clrText);
    }

    virtual void OnDrawTab(CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd)
    {
        CMFCVisualManagerOffice2003::OnDrawTab(pDC, rectTab, iTab, bIsActive, pTabWnd);
    }

    virtual void OnFillTab(CDC* pDC, CRect rectFill, CBrush* pbrFill, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd)
    {
        CMFCVisualManagerOfficeXP::OnFillTab(pDC, rectFill, pbrFill, iTab, bIsActive, pTabWnd);
    }

    virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE)
    {
        CMFCVisualManagerOffice2003::OnFillBarBackground(pDC, pBar, rectClient, rectClip, bNCArea);
    }

    virtual void OnEraseTabsArea(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd)
    {
        CMFCVisualManagerOffice2003::OnEraseTabsArea(pDC, rect, pTabWnd);
    }

    virtual BOOL OnEraseMDIClientArea(CDC* pDC, CRect rectClient)
    {
        pDC->FillRect(rectClient, &m_brMainClientArea);
        return TRUE;
    }

    virtual void OnDrawStatusBarPaneBorder(CDC* pDC, CMFCStatusBar* /*pBar*/, CRect rectPane, UINT /*uiID*/, UINT nStyle)
    {
        pDC->Draw3dRect(rectPane, m_clrPaneBorder, m_clrPaneBorder);
    }
};