// TestCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestCurve.h"

#include <string>
#include "TestCurveDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct {
    COLORREF crColour;  // color 
    TCHAR    *szName;   // color name
} ColourTableEntry;

// colors
static ColourTableEntry g_crColours[] =
{ 
    { RGB(0xFF, 0x00, 0x00),    _T("��ɫ")              },    
    { RGB(0x00, 0x93, 0x00),    _T("��ɫ")				},    
    { RGB(0x8B, 0x8B, 0x00),    _T("���")				},
    { RGB(0x80, 0x00, 0x80),    _T("��ɫ")				},    
    { RGB(0xFF, 0x68, 0x20),    _T("�ۻ�")				},    
    { RGB(0x00, 0x00, 0x8B),    _T("����")				},
    { RGB(0x00, 0x40, 0x40),    _T("�����")			},
    { RGB(0xA5, 0x2A, 0x00),    _T("��ɫ")				},
    { RGB(0x38, 0x8E, 0x8E),    _T("��ɫ")              },
    { RGB(0x00, 0xFF, 0x00),    _T("����")				},
    { RGB(0x4B, 0x00, 0x82),    _T("����")				},
    { RGB(0xFF, 0xD7, 0x00),    _T("��ɫ")              },
    { RGB(0x40, 0xE0, 0xD0),    _T("����")				},
    { RGB(0xD2, 0xB4, 0x8C),    _T("�ػ�")              },
    { RGB(0x68, 0x83, 0x8B),    _T("����")				},

/////////////////////////////////////////////////////////////////////////////
//    { RGB(0x00, 0x00, 0x5E),    _T("�����")		    },
//    { RGB(0x00, 0x00, 0xFF),    _T("��ɫ")              },
//    { RGB(0x48, 0x00, 0x48),    _T("÷��")              },
//    { RGB(0x00, 0x55, 0x00),    _T("����")				},
//    { RGB(0x7B, 0x7B, 0xC0),    _T("��-��")				},
//    { RGB(0x66, 0x66, 0x66),    _T("��-40%")			},
//    { RGB(0x7D, 0x9E, 0xC0),    _T("ǳ��")				},
//    { RGB(0x32, 0xCD, 0x32),    _T("���ɫ")            }, 
//    { RGB(0xFF, 0xAD, 0x5B),    _T("ǳ�ۻ�")			},
//    { RGB(0x7F, 0x7F, 0x7F),    _T("��-50%")			},
//	{ RGB(0x28, 0x28, 0x28),    _T("��-80%")			},
//	{ RGB(0xFF, 0xC0, 0xCB),    _T("�ۺ�")              },    
//	{ RGB(0x8B, 0x00, 0x00),    _T("���")			    },
//	{ RGB(0x00, 0x00, 0x00),    _T("��ɫ")				},
//	{ RGB(0x7F, 0xFF, 0xD4),    _T("��ʯ��")            },
//	{ RGB(0x3C, 0xB3, 0x71),    _T("����")				},
//	{ RGB(0xFF, 0xFF, 0x00),    _T("��ɫ")				},    
//	{ RGB(0xC0, 0xFF, 0xFF),    _T("����")				},
//	{ RGB(0xC0, 0xC0, 0xC0),    _T("��-25%")			},
//	{ RGB(0xFF, 0xFF, 0xE0),    _T("ǳ��")				},
//	{ RGB(0xAF, 0xEE, 0xEE),    _T("ǳ����")			},
//	{ RGB(0xE6, 0xE6, 0xFA),    _T("����")				},
//	{ RGB(0xFF, 0xE4, 0xE1),    _T("õ���")            },
//	{ RGB(0x98, 0xFB, 0x98),    _T("ǳ��")				},
//	{ RGB(0xFF, 0xFF, 0xFF),    _T("��ɫ")				}
};

const int g_nColourCount = sizeof(g_crColours) / sizeof(ColourTableEntry);

#define ID_CURVE_CONTROL   0x9999

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestCurveDlg dialog

CTestCurveDlg::CTestCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestCurveDlg)
	m_iListIndex = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCurveCtrl = NULL;
	m_nCurveCount = 0;
}

CTestCurveDlg::~CTestCurveDlg()
{
	delete m_pCurveCtrl;
}

void CTestCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestCurveDlg)
	DDX_Control(pDX, IDC_STATIC_CURVE, m_StcCurve);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iListIndex);
	DDX_Control(pDX, IDC_EDIT1, editTextExpr);
	DDX_Control(pDX, IDC_EDIT2, editTextOnpExpr);
	DDX_Control(pDX, IDC_EDIT3, editTextResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestCurveDlg, CDialog)
	//{{AFX_MSG_MAP(CTestCurveDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_ADD, OnBtnAdd)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_ADDDATA, OnAdddata)
	//}}AFX_MSG_MAP
	ON_NOTIFY(CVN_MVALUE_ADD, ID_CURVE_CONTROL, OnCurveMouseAdd)
	ON_NOTIFY(CVN_MVALUE_CHANG, ID_CURVE_CONTROL, OnCurveMouseModify)
	ON_NOTIFY(CVN_MVALUE_DELETE, ID_CURVE_CONTROL, OnCurveMouseDelete)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestCurveDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CTestCurveDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestCurveDlg message handlers

BOOL CTestCurveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CRect rect;
	m_StcCurve.GetWindowRect(rect);
	ScreenToClient(rect);

	if (NULL == m_pCurveCtrl)
	{
		m_pCurveCtrl = new CCurveCtrl;
		m_pCurveCtrl->Create(rect, this, ID_CURVE_CONTROL);
		m_pCurveCtrl->SetGridLineStyle(PS_DOT);
		m_pCurveCtrl->SetMargin(CRect(70, 50, 50, 50));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestCurveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestCurveDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestCurveDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestCurveDlg::OnBtnAdd() 
{
	m_nCurveCount++;

	float fX = 100;
	float fY = 0;
	CString strTitle;
	strTitle.Format("TestOneCurve%d", m_nCurveCount);
	int index = m_pCurveCtrl->AddCurve(strTitle, g_crColours[m_nCurveCount % g_nColourCount].crColour);
	m_pCurveCtrl->GetCurve(index)->ShowCurve();
	
	CString buffer;
	editTextExpr.GetWindowTextA(buffer);
	string exp = buffer.GetString();
	int i = 0;

	float leftBound = -7, downBound = -7, widthOfChart = 14, righBound = leftBound + widthOfChart, upBound = downBound + widthOfChart;

	while(i < exp.length())
	{	
		if(isdigit(exp[i]) && exp[i+1] == 'x')
		{
			exp = exp.substr(0,i+1) + '*' + exp.substr(i+1);
		}
		i++;
	}
	for (float f = leftBound; f < righBound; f += 0.1f)
	{
		fX = f;
		fY = evaluate(exp,f);
		if(fY > downBound && fY < upBound)
		{
			m_pCurveCtrl->AddData(strTitle,  fX, fY);
		}
	}
	//fX = 6.28f;
	//fY = 100 * m_nCurveCount * float(sin(6.28));
 //	m_pCurveCtrl->AddData(strTitle, (m_nCurveCount) * fX, fY);
	
	m_pCurveCtrl->Invalidate();
}


double CTestCurveDlg::evaluate(string infix_expr, double x)
{
	char buf[256];
	//editTextExpr.GetWindowTextA(buffer);
	std::string onp_ex, wyn;

	long double ld_result;

	replaceAll(infix_expr,"x", "(" + to_string(x) + ")");

	if(onp.infixToONP(infix_expr,onp_ex) == -1)
	{
		//buffer = "Nieprawidlowe lub nieobslugiwane wyrazenie";
		//editTextExpr.SetWindowTextA(buffer);
		//editTextOnpExpr.SetWindowTextA(buffer);
		//return ;
	}
	else
	{
		//onp_ex = onp.convert(onp_ex,CONP::dec,CONP::dec);
		editTextOnpExpr.SetWindowTextA(onp_ex.c_str());
		//onp_ex = onp.convert(onp_ex,this->actSys,CONP::dec);
		if(onp.evaluateONP(onp_ex,ld_result) == -1)
		{
			editTextResult.SetWindowTextA("Nie mozna obliczyc wyrazenia");
		//return ;
		}
		else
		{
			sprintf(buf,"%.15Lf",ld_result);
			wyn = buf;
			//wyn = onp.convert(wyn,CONP::dec,this->actSys);
			//editTextResult.SetWindowTextA(wyn.c_str());
		}
	}

	return ld_result;
}

void CTestCurveDlg::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void CTestCurveDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
		
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);

	CRect ctrrect;
	CWnd* pWnd = GetDlgItem(ID_BTN_ADD);

	if (pWnd)
	{
		pWnd->GetWindowRect(ctrrect);
		ScreenToClient(ctrrect);

		CRect newrect(rect.right - ctrrect.Width() - 10, rect.bottom - ctrrect.Height() - 40, rect.right - 10, rect.bottom - 40);
		pWnd->MoveWindow(newrect);
	}

	pWnd = GetDlgItem(IDOK);
	if (pWnd)
	{
		pWnd->GetWindowRect(ctrrect);
		ScreenToClient(ctrrect);

		CRect newrect(rect.right - ctrrect.Width() - 10, rect.bottom - ctrrect.Height() - 15, rect.right - 10, rect.bottom - 15);
		pWnd->MoveWindow(newrect);
	}
	
	pWnd = GetDlgItem(IDC_ADDDATA);
	if (pWnd)
	{
		pWnd->GetWindowRect(ctrrect);
		ScreenToClient(ctrrect);

		CRect newrect(rect.right - ctrrect.Width() - 10, rect.bottom - ctrrect.Height() - 65, rect.right - 10, rect.bottom - 65);
		pWnd->MoveWindow(newrect);
	}
	
	pWnd = GetDlgItem(IDC_COMBO1);
	if (pWnd)
	{
		pWnd->GetWindowRect(ctrrect);
		ScreenToClient(ctrrect);

		CRect newrect(rect.right - ctrrect.Width() - 10, ctrrect.top, rect.right - 10, ctrrect.bottom);
		pWnd->MoveWindow(newrect);
	}

	if (m_pCurveCtrl)
	{
		m_pCurveCtrl->GetWindowRect(ctrrect);
		ScreenToClient(ctrrect);

		CRect newrect(ctrrect.left, ctrrect.top, rect.right - 95,  rect.bottom - 15);
		m_pCurveCtrl->MoveWindow(newrect);
	}

	Invalidate();
}

void CTestCurveDlg::OnSelchangeCombo1() 
{
	UpdateData();
/*
MoveRight
MoveLeft
CurveRight
CurveLeft
Restore
ShowMark
ShowCross
EditCurve
MovePageRight
MovePageLeft
ZoomOut
ZoomIn
MirrorHori
MirrorVert
*/
	switch (m_iListIndex)
	{
		case 0:
			m_pCurveCtrl->Move(FALSE);
			break;
		case 1:
			m_pCurveCtrl->Move(TRUE);
			break;
		case 2:
			m_pCurveCtrl->CurveRight(m_pCurveCtrl->GetCurve(0), 0.5);
			break;
		case 3:
			m_pCurveCtrl->CurveLeft(m_pCurveCtrl->GetCurve(0), 0.5);
			break;
		case 4:
			m_pCurveCtrl->Restore();
			break;
		case 5:
			{
				if (!m_pCurveCtrl->GetCurveCount())
					return;

				CCurve* pCur = m_pCurveCtrl->GetCurve(0);
				pCur->Select(!pCur->IsSelected());
				
				m_pCurveCtrl->Invalidate();
			}
			break;
		case 6:
			m_pCurveCtrl->ShowCross(!m_pCurveCtrl->IsShowCross());
			break;
		case 7:
			{
				m_pCurveCtrl->EnableEdit(!m_pCurveCtrl->CanEditCurve());
			}
			break;
		case 8:
			m_pCurveCtrl->MovePage(FALSE);
			break;
		case 9:
			m_pCurveCtrl->MovePage(TRUE);
			break;
		case 10:
			m_pCurveCtrl->Zoom(TRUE);
			break;
		case 11:
			m_pCurveCtrl->Zoom(FALSE);
			break;
		case 12:
			m_pCurveCtrl->MirrorHori(m_pCurveCtrl->GetCurve(0), 2.0f);
			break;
		case 13:
			m_pCurveCtrl->MirrorVert(m_pCurveCtrl->GetCurve(0), 5.0f);	
		default:
			break;
	}	
}

void CTestCurveDlg::OnAdddata() 
{
	static int nAdd = 0;

	float	fX = 6.28f + 0.1f * nAdd++;
	float	fY = 100 * float(sin(fX));
	m_pCurveCtrl->AddData("TestOne Curve1", fX, fY);
	m_pCurveCtrl->Invalidate();
}

void CTestCurveDlg::OnCurveMouseAdd(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_CURVECTRL* pItem = (NM_CURVECTRL*)pNotifyStruct;
	CCurve* pCurve = (CCurve*)pItem->pCurve;
	CString str;
	str.Format("%s: index: %d, %.1f, %.1f", pCurve->GetCurveName(), pItem->iIndex, pItem->fHori, pItem->fVert);
	TRACE(str);
}

void CTestCurveDlg::OnCurveMouseModify(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_CURVECTRL* pItem = (NM_CURVECTRL*)pNotifyStruct;
	CCurve* pCurve = (CCurve*)pItem->pCurve;
	CString str;
	str.Format("%s: index: %d, %.1f, %.1f", pCurve->GetCurveName(), pItem->iIndex, pItem->fHori, pItem->fVert);
	TRACE(str);

}

void CTestCurveDlg::OnCurveMouseDelete(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_CURVECTRL* pItem = (NM_CURVECTRL*)pNotifyStruct;
	CCurve* pCurve = (CCurve*)pItem->pCurve;
	CString str;
	str.Format("%s: index: %d, %.1f, %.1f", pCurve->GetCurveName(), pItem->iIndex, pItem->fHori, pItem->fVert);
	TRACE(str);

}



void CTestCurveDlg::OnBnClickedButton1()
{
	CString csdcs;
	GetDlgItemText(IDC_EDIT1, csdcs) ;
}


void CTestCurveDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	//CString csdcs;
	//GetDlgItemText(IDC_EDIT1, csdcs) ;

	// TODO:  Add your control notification handler code here
}
