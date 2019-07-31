// led_toolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "led_tool.h"
#include "led_toolDlg.h"
//#include "ft2build.h"
//#include "freetype/freetype.h"
//#include "freetype/ftglyph.h "
#include <sys/stat.h>
#include <io.h>
#include "McControl.h"
#include "Led5kSDK.h"
#include <FCNTL.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL Debug_Statue = 0;
char szFile[300][100];
FILE *fp;
CString strPORT;
DWORD dwIP[3];
int m_clor =0;
int m_chrct =3;
SOCKET m_socket[10];
HANDLE hThreadEvent1;  //ÊÂ¼þÏß³Ì¾ä±ú2
unsigned char g_tmp[3][50];
CString display;
BOOL DL_FLAG =0;
BOOL dl =0;
BOOL dl_state =0;
DWORD ThreadProcEvent1(LPVOID pParam);
DWORD ThreadProcEvent_LED1(LPVOID pParam);
DWORD ThreadProcEvent_LED2(LPVOID pParam);
DWORD ThreadProcEvent_LED3(LPVOID pParam);
DWORD ThreadProcEvent_LED4(LPVOID pParam);
DWORD ThreadProcEvent_LED5(LPVOID pParam);
DWORD ThreadProcEvent_LED6(LPVOID pParam);
DWORD ThreadProcEvent_LED7(LPVOID pParam);
DWORD ThreadProcEvent_LED8(LPVOID pParam);
DWORD ThreadProcEvent_LED9(LPVOID pParam);
DWORD ThreadProcEvent_LED10(LPVOID pParam);


char*	sname[TAG_NUM];			//in/out标签名 80
CString Sis_szVal_1;
CString Sis_szVal_2;
CString Sis_szVal_3;

int dwHand[10] = { 0, };


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
public:
	
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
// CLed_toolDlg dialog

CLed_toolDlg::CLed_toolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLed_toolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLed_toolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwHandle = NULL;
	m_bConnect = FALSE;
	for(int j = 0 ; j < TAG_NUM ; j ++)
	{
		//sname[j] = new char[80];
		sdbname[j] = new char[16];
		sdbtagname[j] = new char[80];
		sdes[j] = new char[80];
		sunit[j] = new char[16];
	}
}

void CLed_toolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLed_toolDlg)
	DDX_Control(pDX, IDC_COMBO2, m_Obj_Chrct);
	DDX_Control(pDX, IDC_BUTTON2, m_login);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlPort);

	DDX_Text(pDX, IDC_UserID, m_statedisplay);
	DDX_Control(pDX, IDC_STATIC_LINK, m_link_web);
	DDX_Control(pDX, IDC_UserID, m_EditView);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLed_toolDlg, CDialog)
	//{{AFX_MSG_MAP(CLed_toolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, Onchoosefile)
	ON_BN_CLICKED(IDC_BUTTON3, Onupdate)
	ON_BN_CLICKED(IDC_BUTTON2, Ondenglu)
	ON_BN_CLICKED(IDC_BUTTON4, Onupdata)
	ON_MESSAGE(WM_SHOWTASK,onShowTask)
	ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON5, &CLed_toolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CLed_toolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CLed_toolDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_STATIC_LINK, &CLed_toolDlg::OnStaticLink)
	//ON_WM_MOUSEMOVE(IDC_STATIC, &CLed_toolDlg::OnStaticLinkMove)
	ON_EN_CHANGE(IDC_Port, &CLed_toolDlg::OnEnChangePort)
	ON_EN_CHANGE(IDC_UserID, &CLed_toolDlg::OnEnChangeUserid)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLed_toolDlg message handlers
#include "ini.h"
short __stdcall asc_hex(unsigned char *asc, unsigned char *hex, long pair_len)
{
	char src1, src2, factor1, factor2;
	long len;
	factor1 = '7';
	factor2 = '7';
	//_strupr( (char *)asc );

	for (len = 0; len < pair_len; len++)
	{
		src1 = *(asc + len * 2);
		src2 = *(asc + len * 2 + 1);
		if ((src1 >= '0') && (src1 <= '9'))
			factor1 = '0';
		else if ((src1 >= 'A') && (src1 <= 'F'))
			factor1 = '7';
		else
			return 1;
		if ((src2 >= '0') && (src2 <= '9'))
			factor2 = '0';
		else if ((src2 >= 'A') && (src2 <= 'F'))
			factor2 = '7';
		else
			return 1;
		src1 = src1 - factor1;
		src2 = src2 - factor2;
		*hex++ = (src1 << 4) | src2;
	}
	return 0;
}

BOOL CLed_toolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strInfo;
	CString strwarning;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//Get_Data_From_Json();
	
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
    SetDlgItemText(IDC_Port,"8001");
	
	SetDlgItemText(IDC_IPADDRESS1,"192.168.1.100");

	m_ctrlPort.AddString(_T("RED"));
	m_ctrlPort.AddString(_T("GRN"));
	m_ctrlPort.AddString(_T("YEL"));
	m_Obj_Chrct.AddString(_T("P10_data_l"));
	m_Obj_Chrct.AddString(_T("P10_data_h"));
	m_Obj_Chrct.AddString(_T("In375_data_l"));
	m_Obj_Chrct.AddString(_T("In375_data_h"));
	m_Obj_Chrct.AddString(_T("In500_data_l"));
	m_Obj_Chrct.AddString(_T("In500_data_h"));
   
	m_ctrlPort.SetCurSel(m_clor);
	m_Obj_Chrct.SetCurSel(m_chrct);

	//m_HyperLink.SetURL(_T("http://www.suzhouweiyao.com/"));
	LOGFONT lf;
	CFont m_Font;
	GetFont()->GetObject(sizeof(lf), &lf);
	lf.lfUnderline = TRUE;//具有下划线的文字
	m_Font.CreateFontIndirect(&lf);

	m_link_web.SetFont(&m_Font, true);
	memset(g_tmp[0],0,50);
	memset(g_tmp[1],0,50);
	memset(g_tmp[2],0,50);
	// TODO: Add extra initialization here
	//InitSocket();

	TCHAR szFilePath[MAX_PATH + 1];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0; 
    CString str_url =  szFilePath;
	CString Send_temp;
	CString info_temp;
    ConfigPathName.Format("%s\\config.ini",str_url);
	CIni nIniParam(ConfigPathName);
memset(led_sname[0].IP,0x0,50);
memset(led_sname[1].IP,0x0,50);
memset(led_sname[2].IP,0x0,50);
memset(led_sname[3].IP,0x0,50);
memset(led_sname[4].IP,0x0,50);
memset(led_sname[5].IP,0x0,50);
memset(led_sname[6].IP, 0x0, 50);
memset(led_sname[7].IP, 0x0, 50);
memset(led_sname[8].IP, 0x0, 50);
memset(led_sname[9].IP, 0x0, 50);
memset(store_id, 0x0, 100);
	strInfo = nIniParam.GetValue("SIS_Name1", "IP", "FFFFFFFF");
	memcpy(led_sname[0].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name2", "IP", "FFFFFFFF");
	memcpy(led_sname[1].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name3", "IP", "FFFFFFFF");
	memcpy(led_sname[2].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name4", "IP", "FFFFFFFF");
	memcpy(led_sname[3].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name5", "IP", "FFFFFFFF");
	memcpy(led_sname[4].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name6", "IP", "FFFFFFFF");
	memcpy(led_sname[5].IP,strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name7", "IP", "FFFFFFFF");
	memcpy(led_sname[6].IP, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name8", "IP", "FFFFFFFF");
	memcpy(led_sname[7].IP, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name9", "IP", "FFFFFFFF");
	memcpy(led_sname[8].IP, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name10", "IP", "FFFFFFFF");
	memcpy(led_sname[9].IP, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name1", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[0].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name2", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[1].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name3", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[2].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name4", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[3].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name5", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[4].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name6", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[5].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name7", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[6].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name8", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[7].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name9", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[8].id, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name10", "ID", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[9].id, strInfo.GetLength());



	strInfo = nIniParam.GetValue("SIS_Name1", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[0].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name2", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[1].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name3", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[2].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name4", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[3].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name5", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[4].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name6", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[5].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name7", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[6].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name8", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[7].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name9", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[8].w, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name10", "W", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[9].w, strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name1", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[0].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name2", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[1].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name3", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[2].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name4", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[3].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name5", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[4].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name6", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[5].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name7", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[6].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name8", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[7].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name9", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[8].h, strInfo.GetLength());
	strInfo = nIniParam.GetValue("SIS_Name10", "H", "FFFFFFFF");
	asc_hex((unsigned char*)strInfo.GetBuffer(strInfo.GetLength()), (unsigned char*)&led_sname[9].h, strInfo.GetLength());

	strInfo = nIniParam.GetValue("StoreID", "server_ip", "FFFFFFFF");
	memcpy(store_id, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("control", "debug", "FFFFFFFF");
	Debug_Statue = atoi(strInfo);

	::GetPrivateProfileString("UseFileName","FileName","Error",OpenFilePath.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);


#if 1
	if (Debug_Statue == 0)
	{
		OnGetjiqima();
		License_Enable = strcmp(Get_Machine_data, "B831B5A0F8DA000806EA-00000000-00000000");
		//License_Enable = strcmp(Get_Machine_data, "00E18CB63528000806E9-00000000-00000000");
		//License_Enable = strcmp(Get_Machine_data, "00E04C0741E1000206A7-00000000-00000000"); //01 广州
		//  License_Enable = strcmp(Get_Machine_data, "00E04C680B8B000306A9-00000000-00000000"); //02 天津
		//  License_Enable = strcmp(Get_Machine_data, "00F1F523EA7E000206A7-00000000-00000000"); //03 深圳
		if (License_Enable)
		{

			strwarning.Format("Warning:Soft Have error License\n:%s", Get_Machine_data);
			AfxMessageBox(strwarning);
			OnOK();
		}

		
	}

	for (int j = 0; j < TAG_NUM; j++)
	{
		sname[j] = new char[80];
	
	}

#endif

#if 0
  if( memcmp("Error",OpenFilePath.GetBuffer(3),3) == 0)
         AfxMessageBox(OpenFilePath+"please choose a file for first time");
  else
  {
	    ::GetPrivateProfileString("IP_address","ip","Error",m_strip.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);
        ::GetPrivateProfileString("IP_address","port","Error",strPORT.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);
		::GetPrivateProfileString("IP_address","str_ip","Error",str_ip.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);
		
		SetDlgItemText(IDC_IPADDRESS1,str_ip);
		
	//	dwIP[0]=atoi(m_strip);

        ::GetPrivateProfileString("control","clor","Error",m_strip.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);
		m_clor=atoi(m_strip);
		m_ctrlPort.SetCurSel(m_clor);

		::GetPrivateProfileString("control","chrct","Error",m_strip.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);
		if( memcmp("Error",m_strip.GetBuffer(3),3) != 0)
		{
			m_chrct=atoi(m_strip);
			m_Obj_Chrct.SetCurSel(m_chrct);
		}
		

		//Ondenglu();
	//	DL_FLAG =1;
		

  }
#endif

//Connect();
  
	hThreadEvent1=CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED1,
		NULL,0,NULL);  
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED2,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED3,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED4,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED5,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED6,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED7,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED8,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED9,
		NULL, 0, NULL);
	hThreadEvent1 = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent_LED10,
		NULL, 0, NULL);
	
  if (Debug_Statue == 0)
  {
	  OnBnClickedButton7();
	  SetTimer(2, 5000, NULL);
	  SetTimer(1, 30000, NULL);
  }
	
	//CloseHandle(hThreadEvent1);


  // m_font.CreatePointFont(30, "华文行楷");//代表15号字体，华文行楷
     m_brush.CreateSolidBrush(RGB(165, 165, 165));//画刷为绿色




	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLed_toolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
int firt_i = 1;
void CLed_toolDlg::OnPaint() 
{
	
	if (firt_i)
	{
		firt_i = 0;
		OnMouseMove(0, NULL);
		mouse_flag = 0;
		CBitmap bitmap;  // CBitmap对象，用于加载位图   
		HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

		
	}
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
HCURSOR CLed_toolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CLed_toolDlg::InitSocket()
{   
//	UpdateData(false);
    int try_port =3001;

fix_port:

	client_socket=socket(AF_INET,SOCK_STREAM,0);
	m_socket[0]=socket(AF_INET,SOCK_STREAM,0);
	m_socket[1]=socket(AF_INET,SOCK_STREAM,0);
	m_socket[2]=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSock;
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(try_port);
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);


	int retval;
	retval=bind(client_socket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR));

	if(SOCKET_ERROR==retval)
	{
		closesocket(client_socket);
		try_port++;
		if(try_port>65535)
		{
			MessageBox("°ó¶¨Ê§°Ü!");
			return FALSE;
		}
		goto fix_port;
	}
	  UpdateData(true);
	return TRUE;

}

void CLed_toolDlg::Onchoosefile() 
{
	// TODO: Add your control notification handler code here
	
	char indata[1024]={0};
	char outdata[1024]={0};
	CString tmp;
	CString tmp1;
   	CString str_FileExt="certlibÎÄ¼þ (*.*)|*.*";
	CFileDialog OpenCertFile(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,str_FileExt,NULL);
		
	
	if (OpenCertFile.DoModal() == IDOK)
	{
		OpenFilePath = OpenCertFile.GetPathName();


	CString strTemp,strTempA;
	int nCount=6;

	::WritePrivateProfileString("UseFileName","FileName"+strTemp,OpenFilePath,ConfigPathName);


	
	}
	else
	{
		
		goto RETURN;
		
	}



	
RETURN:
	
	
	return;



}

CString CLed_toolDlg::GetTimeStr(void)						//Ê±¼ä×ª»»Îª×Ö·û´®
{
	CString szret = "";
	SYSTEMTIME sys; 
	GetLocalTime( &sys );
	szret.Format("%04d-%02d-%02d %02d:%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute);
		
		
	return szret;
}
unsigned int count =1;

void CLed_toolDlg::OnBtnSend2() 
{
	UpdateData(true);
	static char first_forap=0;
	char *p="adbcd";
	SYSTEMTIME sys; 
	GetLocalTime( &sys );
	
	CString tmp;
	DWORD dwIP[3];

	CString strPORT;

	GetDlgItemText(IDC_Port,strPORT);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP[0]);

   if(dl_state) 
   {
	   GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE); 
       if(first_forap ==0)
	   {
		   first_forap=1;
	       OnClose() ;
	   }
   }

	



		readdata();

		//tmp.Format("led 32t 0 128 0 %s \r\n",Sis_szVal_1);
		tmp.Format("led 32t 0 130 0 %s \r\n",Sis_szVal_1);
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0	);
			Sleep(1000);
			//tmp.Format("led 32t 0 315 0 %s \r\n",Sis_szVal_2);
			tmp.Format("led 32t 0 340 0 %s \r\n",Sis_szVal_2);
		
		
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0	);
		Sleep(1000);
#if 0
		if(sys.wMonth<10)
		{
			if(sys.wDay<10)
			tmp.Format("led 32t 0 436 0 %04d-%02d-%02d\r\n",sys.wYear,sys.wMonth,sys.wDay);
			else
			tmp.Format("led 32t 0 436 0 %04d-%02d-%02d\r\n",sys.wYear,sys.wMonth,sys.wDay);

		}
		else
		{
			if(sys.wDay<10)
			tmp.Format("led 32t 0 436 0 %04d-%02d-%02d\r\n",sys.wYear,sys.wMonth,sys.wDay);
			else
			tmp.Format("led 32t 0 436 0 %04d-%02d-%02d\r\n",sys.wYear,sys.wMonth,sys.wDay);
		}
#endif
		tmp.Format("led 32t 0 557 0 %s \r\n",Sis_szVal_3);
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0	);
				Sleep(1000);

		//	tmp.Format("led 32t 0 610 0  %02d:%02d\r\n",sys.wHour,sys.wMinute);
	//	tmp.Format("led 32t 0 556 0 %d \r\n",count++);
		count = count%10;
	//	send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0	);
		//Sleep(1000);
	
		

  
RETURN:
 
	return ;
	UpdateData(false);
}

DWORD ThreadProcEvent1(LPVOID pParam)
{  
//	HWND hwnd=((RECVPARAM*)pParam)->hwnd;


	struct sockaddr_in ser_addr;/* ·þÎñÆ÷µÄµØÖ·*/
	int addrlen =0;
	CString tmp;

	/* ³õÊ¼»¯·þÎñÆ÷µØÖ·*/


	CString tempBuf[5];
	tempBuf[0] ="ÇëÇóÖÐ...";
	tempBuf[1] ="ÇëÇóÖÐ...";
	tempBuf[2] ="ÇëÇóÖÐ...";

	while (1)
	{

		Sleep(5);
	}
	
	while(1)
	{

		if(DL_FLAG)
		{
			display.Format("Á¬½ÓÏÔÊ¾ÆÁ%s\r\n",tempBuf[0]);
			dl =1 ;
			addrlen=sizeof(struct sockaddr_in);
			
			
			
			ser_addr.sin_family=AF_INET;
			
			ser_addr.sin_addr.s_addr=htonl(dwIP[0]);
			
			 ser_addr.sin_port=htons(atoi(strPORT));
		
		if(connect(m_socket[0],(struct sockaddr*)&ser_addr,sizeof(SOCKADDR))!=0)/*ÇëÇóÁ¬½Ó*/
			
		{
			
		
				
			tempBuf[0] ="³¬Ê±";
			
		}
		else
		{

		   tempBuf[0] ="³É¹¦";
		   dl_state = 1;

	
		}
		display.Format("Á¬½ÓÏÔÊ¾ÆÁ%s\r\n",tempBuf[0]);
		DL_FLAG = 0;
		dl =1 ;
	

		}
		Sleep(5);
	}

}

LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //²ÎÊýlpcstrÀàÐÍÒ²¿ÉÊÇchar*
{ 
	LPWSTR Pwstr;
	int  i;
	i=MultiByteToWideChar(CP_ACP,0,lpcstr,-1,NULL,0);
	Pwstr=new WCHAR[i]; 
	MultiByteToWideChar(CP_ACP,0,lpcstr,-1,Pwstr,i);
	return (Pwstr);
}


void CLed_toolDlg::find(char * lpPath)
{
    char szFind[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
	int i=0;
    strcpy(szFind,lpPath);
    strcat(szFind,"\\*.*");
	
    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)    return;
    
    while(TRUE)
    {
        if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if(FindFileData.cFileName[0]!='.')
            {
                strcpy(szFile[i],lpPath);
                strcat(szFile[i],"\\");
                strcat(szFile[i],FindFileData.cFileName);
                find(szFile[i]);
				//	m_fontstyle.AddString(FindFileData.cFileName);
				i++;
            }
        }
       
        if(!FindNextFile(hFind,&FindFileData))    break;
    }
    FindClose(hFind);
}




// Á¬½Ó
void CLed_toolDlg::Connect() 
{
	// TODO: Add your control notification handler code here
	char*	sips[ProxyNum];		//·þÎñÆ÷IP	16×Ö½Ú
	char*	susers[ProxyNum];	//µÇÂ¼ÕËºÅ	16×Ö½Ú
	char*	spasss[ProxyNum];	//µÇÂ¼ÃÜÂë	16×Ö½Ú
	WORD	wport[ProxyNum];	//¶Ë¿Ú

	long j ;
	for( j = 0 ; j < ProxyNum ; j ++)
	{
		sips[j] = new char[16];
		susers[j] = new char[16];
		spasss[j] = new char[16];
	}

	// ÉèÖÃÁ¬½Ó²ÎÊý
	//strcpy(sips[0], "10.219.109.198");
	//strcpy(sips[0], "172.18.108.110");
	strcpy(sips[0], "172.18.108.21");
	strcpy(susers[0], "admin");
	strcpy(spasss[0], "admin");
	wport[0] = 12084;

	m_dwHandle = DBPCreate(sips,susers,spasss,wport, ProxyNum); //´´½¨¶ÔÏó

	//´´½¨¶ÔÏó Æô¶¯Á¬½Ó
	DWORD ret;
	if(m_dwHandle != NULL)
	{
		ret = DBPConnect(m_dwHandle);
		if(ret == 0)
		{
			AfxMessageBox("DBPConnect");
		}
	}	

	//È¡Á¬½ÓÐÅÏ¢
	CString szt;
	char     sip[16];
	int      pnNum;
	if(m_dwHandle != NULL )
	{
		ret = DBPGetSvrConInfo(m_dwHandle,sip,&pnNum);
	}
	//	szt.Format("Á¬½ÓºÄÊ±: %f ms ,Á¬½ÓµÄ·þÎñÆ÷: %s",span , sip  );
	szt.Format("Á¬½ÓµÄ·þÎñÆ÷: %s", sip  );

	for( j = 0 ; j < ProxyNum ; j ++)
	{
		delete[] sips[j];
		delete[] susers[j];
		delete[] spasss[j];
	}

	

}



void CLed_toolDlg::Onupdate() 
{
	CString tmp;

   


    m_chrct =m_Obj_Chrct.GetCurSel();

	if(dl_state) 
	{
		switch (m_chrct)
		{
	    	case 0 :
			  tmp.Format("set chrct p10\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			case 1 :
			  tmp.Format("set chrct p10\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			case 2 :
    			tmp.Format("set chrct in375\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			case 3 :
			  tmp.Format("set chrct in375_h\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			case 4 :
			  tmp.Format("set chrct in500\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			case 5 :
			  tmp.Format("set chrct in500_h\n");
				send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
				Sleep(5);
		    break;
			default :
				break;


		}
      






		tmp.Format("led clearall\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
      
		tmp.Format("led r 0 1 1 190 62 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);


		tmp.Format("led l 0 1 22 190 22 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led l 0 1 43 190 43 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led l 0 80 1 80 62 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);


		tmp.Format("led t 0 20 6 Ñ¹ \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
        tmp.Format("led t 0 48 6 Á¦ \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 18 26 ¹©ÆûÁ¿ \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 18 46 ÊÛÆûÁ¿ \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);



        tmp.Format("led t 0 164 6 MPa\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 164 26  T/H\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 164 46  T/H\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);

		tmp.Format("file save\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);

        tmp.Format("reset\n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);

	}


}

void CLed_toolDlg::Ondenglu() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	DL_FLAG =1;

	GetDlgItemText(IDC_Port,strPORT);
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP[0]);
//	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS2))->GetAddress(dwIP[1]);
//	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS3))->GetAddress(dwIP[2]);

    

    CString strTemp; 
	m_clor =m_ctrlPort.GetCurSel();
	strTemp.Format("%d",m_clor);
     ::WritePrivateProfileString("control","clor",strTemp,ConfigPathName);

	m_chrct =m_Obj_Chrct.GetCurSel();
	strTemp.Format("%d",m_chrct);
     ::WritePrivateProfileString("control","chrct",strTemp,ConfigPathName);


	strTemp.Format("%d",dwIP[0]);
     ::WritePrivateProfileString("IP_address","ip",strTemp,ConfigPathName);
     ::WritePrivateProfileString("IP_address","port",strPORT,ConfigPathName);
	 

	  GetDlgItemText(IDC_IPADDRESS1,strTemp);
	  ::WritePrivateProfileString("IP_address","str_ip",strTemp,ConfigPathName);
	  UpdateData(false);
	 // AfxMessageBox(strTemp);
}

void CLed_toolDlg::Onupdata() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    memset(g_tmp[0],0,50);
	memset(g_tmp[1],0,50);
	memset(g_tmp[2],0,50);


    CString strTemp; 
	m_clor =m_ctrlPort.GetCurSel();
	strTemp.Format("%d",m_clor);
     ::WritePrivateProfileString("control","clor",strTemp,ConfigPathName);

	m_chrct =m_Obj_Chrct.GetCurSel();
	strTemp.Format("%d",m_chrct);
     ::WritePrivateProfileString("control","chrct",strTemp,ConfigPathName);

	 strTemp.Format("%d",dwIP[0]);
     ::WritePrivateProfileString("IP_address","ip",strTemp,ConfigPathName);

     ::WritePrivateProfileString("IP_address","port",strPORT,ConfigPathName);
	 

	  GetDlgItemText(IDC_IPADDRESS1,strTemp);
	  ::WritePrivateProfileString("IP_address","str_ip",strTemp,ConfigPathName);

     UpdateData(false);

}
 

void CLed_toolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	NOTIFYICONDATA   nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this-> m_hWnd; 
	nid.uID= IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP   ; 
	nid.uCallbackMessage=WM_SHOWTASK;//×Ô¶¨ÒåµÄÏûÏ¢Ãû³Æ 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip, "¼Æ»®ÈÎÎñÌáÐÑ ");//ÐÅÏ¢ÌáÊ¾ÌõÎª¡°¼Æ»®ÈÎÎñÌáÐÑ¡± 
	Shell_NotifyIcon(NIM_ADD,&nid);//ÔÚÍÐÅÌÇøÌí¼ÓÍ¼±ê 
	ShowWindow(SW_HIDE);//Òþ²ØÖ÷´°¿Ú 
	//CDialog::OnClose();
}

void CLed_toolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString temp;
	LOGFONT lf;
	CFont m_Font;
	if (mouse_flag ==1&&(point.x >= 1050 && point.x <= 1490 && point.y >= 400 && point.y <= 490))
	{
		mouse_flag = 0;
		temp.Format("%d %d", point.x, point.y);
	
		GetFont()->GetObject(sizeof(lf), &lf);
		lf.lfUnderline = TRUE;//具有下划线的文字
		lf.lfHeight = 32;
		m_Font.CreateFontIndirect(&lf);

		m_link_web.SetFont(&m_Font,true);

		//AfxMessageBox(temp);
	}
	else if(mouse_flag == 0)
	{
		mouse_flag = 1;
		GetFont()->GetObject(sizeof(lf), &lf);
		lf.lfUnderline = FALSE;//具 有下划线的文字
		lf.lfHeight = 32;
		m_Font.CreateFontIndirect(&lf);

		m_link_web.SetFont(&m_Font, true);

	}
	
}


 LRESULT   CLed_toolDlg::onShowTask(WPARAM   wParam,LPARAM   lParam) 
//wParam½ÓÊÕµÄÊÇÍ¼±êµÄID£¬¶ølParam½ÓÊÕµÄÊÇÊó±êµÄÐÐÎª 
{ 
    if(wParam!= IDR_MAINFRAME)
    return   1; 
    switch(lParam) 
    { 
         case WM_RBUTTONUP://ÓÒ¼üÆðÀ´Ê±µ¯³ö¿ì½Ý²Ëµ¥£¬ÕâÀïÖ»ÓÐÒ»¸ö¡°¹Ø±Õ¡± 
         { 
              LPPOINT   lpoint=new   tagPOINT; 
              ::GetCursorPos(lpoint);//µÃµ½Êó±êÎ»ÖÃ 
              CMenu   menu; 
              menu.CreatePopupMenu();//ÉùÃ÷Ò»¸öµ¯³öÊ½²Ëµ¥ 
              //Ôö¼Ó²Ëµ¥Ïî¡°¹Ø±Õ¡±£¬µã»÷Ôò·¢ËÍÏûÏ¢WM_DESTROY¸øÖ÷´°¿Ú£¨ÒÑ 
              //Òþ²Ø£©£¬½«³ÌÐò½áÊø¡£ 
              menu.AppendMenu(MF_STRING,WM_DESTROY, "Exit ");   
              //È·¶¨µ¯³öÊ½²Ëµ¥µÄÎ»ÖÃ 
              menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
              //×ÊÔ´»ØÊÕ 
              HMENU   hmenu=menu.Detach(); 
              menu.DestroyMenu(); 
              delete   lpoint; 
			 // CreateRun();
           } 
           break; 
           case WM_LBUTTONDBLCLK://Ë«»÷×ó¼üµÄ´¦Àí 
           { 
               this-> ShowWindow(SW_SHOW);//¼òµ¥µÄÏÔÊ¾Ö÷´°¿ÚÍêÊÂ¶ù 
           } 
           break; 
      } 
      return   0; 
  } 

int CLed_toolDlg::CreateRun(void)
{
//Ìí¼ÓÒÔÏÂ´úÂë
 HKEY   RegKey;  
 CString   sPath;  
 GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);  
 sPath.ReleaseBuffer();  
 int   nPos;  
 nPos=sPath.ReverseFind('\\');  
 sPath=sPath.Left(nPos);  
 CString   lpszFile=sPath+"\\led_tool.exe";//ÕâÀï¼ÓÉÏÄãÒª²éÕÒµÄÖ´ÐÐÎÄ¼þÃû³Æ  
 CFileFind   fFind;  
 BOOL   bSuccess;  
 bSuccess=fFind.FindFile(lpszFile);  
 fFind.Close();  
 if(bSuccess)  
 {  
  CString   fullName;  
  fullName=lpszFile;  
  RegKey=NULL;  
  RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&RegKey);  
  RegSetValueEx(RegKey,"led_tool",0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());//ÕâÀï¼ÓÉÏÄãÐèÒªÔÚ×¢²á±íÖÐ×¢²áµÄÄÚÈÝ  
  this->UpdateData(FALSE);  
 }  
 else  
 {  
  //theApp.SetMainSkin();  
  ::AfxMessageBox("Ã»ÕÒµ½Ö´ÐÐ³ÌÐò£¬×Ô¶¯ÔËÐÐÊ§°Ü");  
  exit(0);  
 }  
 return 0;
} 

void CLed_toolDlg::OnBnClickedButton5()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	//Connect();
	//readdata();

	OnWebBnClickedConnect();
}


// ¶Á»ù±¾ÊµÊ±
void CLed_toolDlg::readdata()
{
	// TODO: Add your control notification handler code here
	long	ltimes[TAG_NUM];
	short	snqas[TAG_NUM];
	double	dblals[TAG_NUM];
	long	lvals[TAG_NUM];
	int		ntypes[TAG_NUM];
	short	errs[TAG_NUM];

	int		bufsize = 3;
	long j = 0 ;

	int count = 0 ,nIndex=0; 
	CString szVal,szt,szTime;

	DWORD ret;
	if(m_dwHandle != NULL )
	{
	
		ret =  DBPGetSnapshot( //¶Á¿ìÕÕ
			m_dwHandle,
			sname, //±êÇ©ÃûÊý×é
			ltimes,	//in/out, Ê±±ê
			snqas,	//in/out, ÖÊÁ¿
			dblals,	//in/out, ´æ·ÅdoubleÖµ,DT_FLOAT32,DT_FLOAT64´æ·ÅÇø
			lvals,	//in/out, ´æ·ÅLongÖµ,DT_DIGITAL,DT_INT32,DT_INT64´æ·ÅÇø
			ntypes,	//in/out, Êý¾ÝÀàÐÍ,DT_INT32,DT_FLOAT32µÈ¡£
			errs,		 //in/out, ´íÎóÂë
			bufsize		 //in, ¸öÊý
			);//·µ»Ø´íÎóÂë



#if 0		
		    Sis_szVal_1.Format("%3.1f",51.8);
		
			//Sis_szVal_2.Format("%4.1f",204.1);
			Sis_szVal_2.Format("%4.2f",50.00);
		
			Sis_szVal_3.Format("%3.2f",1.08);
#else
			Sis_szVal_1.Format("%3.1f",dblals[0]);
			Sis_szVal_2.Format("%4.1f",dblals[1]);
			
			//Sis_szVal_2.Format("%4.2f",dblals[1]);
			Sis_szVal_3.Format("%3.2f",dblals[2]);
#endif


		//	fwrite(szVal,1,szVal.GetLength(),tem_file);
		//	fwrite("\r\n",1,2,tem_file);

	}
}


void __stdcall test(int event,HBOX boxno,int para1,int para2)
{
	//OnMcEvent»Øµ÷²»ÔÚÖ÷Ïß³ÌÖÐ¡£ÐèÔÙ´Î×ª·¢µ½Ö÷Ïß³ÌÖÐ£¬·ñÔò»áË¢ÐÂ½çÃæÓÐÊ±»á¿¨ËÀ (½çÃæÏµÍ³Ò»°ã¶¼²»ÊÇÏß³Ì°²È«µÄ)
	int w,l;
	w = event;
	w = boxno;
	w = (event<<16)|(boxno);//boxno<65535
	l = (para1<<16)| para2;
	//::PostMessageA(FrmMain->Handle,WM_MCEVENT,w,l);
   
}

void  CLed_toolDlg::Mc_onConnect(void)
{
   int ret =0;
   //(1)³ÌÐò³õÊ¼»¯Ê±
   ret = mcInit(8300,(TOnMcEvent)test,1);
   
  // int m = mcFindBoxByIP("192.168.0.77");
  // ret = mcBoxIsConnect(m);
  // mcConnectBox("192.168.0.77",8100);
  // ret = mcGetBoxCount();
}

void CLed_toolDlg::OnBnClickedButton6()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	Mc_onConnect();
}

CString led_Status;
int      led_error = 0;
CString Test_Value;
bx_5k_area_header my_area;
void CLed_toolDlg::OnBnClickedButton7()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	BYTE programLif[8] = {0xff,};
	BYTE AreaDatatLis[8] = {0,};
	BYTE TXT[80] = {'A',};
	BYTE SAT[80] = {0,};
	CString tmp;
	unsigned char ret = 0;

		
	
//	sprintf(TXT,"À´Ö®¶þ´Î¿ª·¢");
	InitSdk(2, 2);
	
//	CreateBroadCast(char *broad_ip,UINT broad_port,bx_5k_card_type card_type);

	dwHand[0] = CreateClient(led_sname[0].IP, 5005, (bx_5k_card_type)led_sname[0].id, 1, 1, NULL);
#if 1
	dwHand[1] = CreateClient(led_sname[1].IP, 5005, (bx_5k_card_type)led_sname[1].id, 1, 1, NULL);
	dwHand[2]  = CreateClient(led_sname[2].IP, 5005, (bx_5k_card_type)led_sname[2].id, 1, 1, NULL);
	dwHand[3]  = CreateClient(led_sname[3].IP, 5005, (bx_5k_card_type)led_sname[3].id, 1, 1, NULL);
	dwHand[4]  = CreateClient(led_sname[4].IP, 5005, (bx_5k_card_type)led_sname[4].id, 1, 1, NULL);
	dwHand[5]  = CreateClient(led_sname[5].IP, 5005, (bx_5k_card_type)led_sname[5].id, 1, 1, NULL);
	dwHand[6] = CreateClient(led_sname[6].IP, 5005, (bx_5k_card_type)led_sname[6].id, 1, 1, NULL);
	dwHand[7] = CreateClient(led_sname[7].IP, 5005, (bx_5k_card_type)led_sname[7].id, 1, 1, NULL);
	dwHand[8] = CreateClient(led_sname[8].IP, 5005, (bx_5k_card_type)led_sname[8].id, 1, 1, NULL);
	dwHand[9] = CreateClient(led_sname[9].IP, 5005, (bx_5k_card_type)led_sname[9].id, 1, 1, NULL);
#endif
	if (dwHand[0] & dwHand[1] & dwHand[2] & dwHand[3] & dwHand[4] & dwHand[5] & dwHand[6] & dwHand[7] & dwHand[8] & dwHand[9])
	{
		led_error = 0;
		led_Status.Format("led success");
	}
	else
	{
		led_error = 1;
		led_Status.Format("12001-%X,%X,%X,%X,%X,%X,%X,%X,%X,%X\r\n", dwHand[0], dwHand[1], dwHand[2], dwHand[3], dwHand[4], dwHand[5], dwHand[6], dwHand[7], dwHand[8], dwHand[9]);
	}
	
	if (Debug_Statue == 1)
	{
		//AfxMessageBox(led_Status);
			int index = 0;
			for (int led_id = 0; led_id < 10; led_id++)
			{

				if (dwHand[led_id] != 0)
				{
					Test_Value.Format("IP: %s", led_sname[led_id].IP);

					my_area.AreaX = 0;
					my_area.AreaY = 0;
					my_area.AreaType = 0x07;
					my_area.AreaWidth = led_sname[led_id].w;//    24;
					my_area.AreaHeight = led_sname[led_id].h;// 64;
					my_area.Lines_sizes = 1;

					my_area.Reserved[0] = 0;
					my_area.Reserved[1] = 0;
					my_area.Reserved[2] = 0;

					my_area.DynamicAreaLoc = 0;   //¶¨ÒåÒ»¸ö¶¯Ì¬
					my_area.RunMode = 0;//RunMode_list[rl];

					my_area.Timeout = 10;
					my_area.SingleLine = 2;
					my_area.NewLine = 2;
					my_area.DisplayMode = 1;
					my_area.ExitMode = 0x00;
					my_area.Speed = 0;
					my_area.StayTime = 0;
					my_area.DataLen = Test_Value.GetLength();// tmp.GetLength();// strlen(sname[i]);



					ret = SCREEN_SendDynamicArea(dwHand[led_id], my_area, Test_Value.GetLength(), (BYTE*)Test_Value.GetBuffer(Test_Value.GetLength()));
					
																							 //SCREEN_SendDynamicArea(dwHand[i], my_area, strlen(sname[i]), (BYTE*)sname[i]);
					ret = 1;
				}
			}

	}
//	SetTimer(1, 5000, NULL);

	 //CON_Reset(dwHand);
}



void CLed_toolDlg::OnEnChangePort()
{
	// TODO:  Èç¹û¸Ã¿Ø¼þÊÇ RICHEDIT ¿Ø¼þ£¬Ëü½«²»
	// ·¢ËÍ´ËÍ¨Öª£¬³ý·ÇÖØÐ´ CDialog::OnInitDialog()
	// º¯Êý²¢µ÷ÓÃ CRichEditCtrl().SetEventMask()£¬
	// Í¬Ê±½« ENM_CHANGE ±êÖ¾¡°»ò¡±ÔËËãµ½ÑÚÂëÖÐ¡£

	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
}


#include   <httpext.h>   
#include   <windef.h>   
#include   <Nb30.h>

#pragma comment(lib, "Netapi32.lib")

void CLed_toolDlg::OnGetjiqima()
{
	// TODO: Add your control notification handler code here
	unsigned long s1, s2;
	unsigned char vendor_id[] = "------------";//CPUÌá¹©ÉÌID
	CString str1, str2, str3;
	// ÒÔÏÂÎª»ñµÃCPU IDµÄ»ã±àÓïÑÔÖ¸Áî
  //   _asm    // µÃµ½CPUÌá¹©ÉÌÐÅÏ¢ 
  //   {  
  //       xor eax,eax   // ½«eaxÇå0
  //       cpuid         // »ñÈ¡CPUIDµÄÖ¸Áî
  //       mov dword ptr vendor_id,ebx
  //       mov dword ptr vendor_id[+4],edx
  //       mov dword ptr vendor_id[+8],ecx  
  //   }
  //   str1.Format("%s",vendor_id);

	_asm    // µÃµ½CPU IDµÄ¸ß32Î» 
	{
		mov eax, 01h
		xor edx, edx
		cpuid
		mov s2, eax
	}
	str2.Format("%08X-", s2);

	_asm    // µÃµ½CPU IDµÄµÍ64Î»
	{
		mov eax, 03h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, ecx
	}

	str3.Format("%08X-%08X", s1, s2);

	char   mac[200];
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, *PASTAT;
	ASTAT Adapter;

	typedef struct _LANA_ENUM
	{
		UCHAR   length;
		UCHAR   lana[MAX_LANA];
	}LANA_ENUM;
	LANA_ENUM lana_enum;

	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));

	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);


	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb);
		if (uRetCode == NRC_GOODRET)
			break;
	}


	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);

	sprintf(mac, "%02X%02X%02X%02X%02X%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]
	);
	CString m_str1;
	CString str;
	m_str1.Format("%s", mac);

	str2 = str2 + str3;
	str = m_str1 + str2;
	memset(Get_Machine_data, 0x0, 200);
	memcpy(Get_Machine_data, str.GetBuffer(str.GetLength()), str.GetLength());

}

#if 0

#pragma comment(lib,"json.lib")
#include "json.h"


void CLed_toolDlg::Get_Data_From_Json()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	CString strJson = _T("{\"id\":\"123456\",\"username\":\"456789\",\"password\":\"00000\",\"addr\":\"µØÖ·µØÖ·Ö±Ö±Ö±ÆðØÔ\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}],\"json\":{\"id\":\"0000\",\"username\":\"000001\"}}");
	Json::Reader reader;
	Json::Value root;

	USES_CONVERSION;
	char* cJson = T2A(strJson.GetBuffer(0));
	strJson.ReleaseBuffer();

	CString addr, username, cpp, jsonid;

	if (reader.parse(cJson, root))
	{
		//È¡µÃÊý¾Ý
		addr = root["addr"].asCString();
		username = root["username"].asCString();
		//±éÀúJSONÊý×é
		const Json::Value arrayObj = root["array"];
		for (unsigned int i = 0; i < arrayObj.size(); i++)
		{
			if (!arrayObj[i].isMember("cpp"))
				continue;
			cpp = arrayObj[i]["cpp"].asCString();
		}
		//È¡µÃJSONÊý¾Ý
		const Json::Value jsonObj = root["json"];
		jsonid = jsonObj["id"].asCString();
	}
	CString str1;
	str1.Format(_T("addr:%s,username:%s,cpp:%s,jsonid:%s"), addr, username, cpp, jsonid);
	AfxMessageBox(str1);
}
#endif
#include "afxdialogex.h"
#include <iostream>  
#include <string>  
#include <sstream> 
#include <io.h>
#include <direct.h>


void CLed_toolDlg::OnWebBnClickedConnect()
{
	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
//    GetDlgItem(IDC_ADDRESS)->UpdateData(TRUE);
	GetDlgItemText(IDC_IPADDRESS1, m_strAddress);

	bool done = false;

	readAndWriteConfigure(store_id, 1);
	if (0 == endpoint.connect(store_id, this))
	{
		//m_bConnect = 1;
		//AfxMessageBox("WebSocket Connect Success!");
		
		;// GetDlgItem(ID_CONNECT)->EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox("WebSocket Connect Fail!");
	}
	
}


void CLed_toolDlg::OnWebBnClickedDisconnect()
{
	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if (m_bConnect)
	{
		endpoint.close();
		
	}
}


void CLed_toolDlg::OnWebBnClickedClearMsg()
{
	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
//    if (m_bConnect)
	{
		m_strShowMsg = "";
		//SetDlgItemText(IDC_MSG_SHOW, m_strShowMsg);
	}
}


void CLed_toolDlg::OnWebBnClickedSend()
{
	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if (m_bConnect)
	{
		//GetDlgItemText(IDC_MSG_SEND, m_strInputMsg);
		if (m_strInputMsg.IsEmpty())
		{
			AfxMessageBox("ÇëÏÈÊäÈëÏûÏ¢,ÔÙ½øÐÐ·¢ËÍ...");
			return;
		}

		USES_CONVERSION;
		       char* s_char  = W2A(L"ping");
		std::string input = s_char;
		endpoint.send(input);
		m_strShowMsg += m_strInputMsg + L"\r\n";
		AfxMessageBox(m_strShowMsg);
		//m_EditView.LineScroll(m_EditView.GetLineCount(), 0);
	}
	else
	{
		AfxMessageBox("ÇëÏÈ½øÐÐÁ¬½ÓºóÔÙ´Î³¢ÊÔ·¢ËÍ...");
	}
}

void CLed_toolDlg::OnWebBnClickedCancel()
{
	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	endpoint.close();
	m_bConnect = FALSE;
	
}

void CLed_toolDlg::onWebOpen()
{
	if (!m_bConnect)
	{
		
		m_bConnect = TRUE;
	}
}

void CLed_toolDlg::onWebFail(std::string strReason)
{
	if (m_bConnect)
	{
		//AfxMessageBox(CString(strReason.c_str()));
		//GetDlgItem(ID_CONNECT)->EnableWindow(TRUE);
		//GetDlgItem(ID_DISCONNECT)->EnableWindow(FALSE);
		m_bConnect = FALSE;
	}
}

void CLed_toolDlg::onWebClose(int nCode, std::string strCode, std::string strReason)
{
	//     CString strClose;
	//     strClose.Format(_T("close code:%d"), nCode);
	//     strClose += CString(_T("(")) + CString(strCode.c_str()) + CString(_T(")")) + CString(_T("\nclose reason:")) + CString(strReason.c_str());
	//     AfxMessageBox(strClose);

	if (m_bConnect)
	{
	

		m_bConnect = FALSE;
	}
}
int connect_web_ok = 0;
char R_temp[400];
int global_led_id = 10;
int led_ready = 10;
void CLed_toolDlg::onWebMessage(std::string strMessage)
{
	//AfxMessageBox((char*)strMessage.c_str());
	int ret;
	
	connect_web_ok = 1;
	strMessage = TranslateUTF8ToGB((char*)strMessage.c_str(), strMessage.length());
	


	m_strShowMsg += CString(strMessage.c_str()) + L"\n";
	if (m_strShowMsg.GetLength() > 1024000 * 5)
	{
		int nPos = m_strShowMsg.Find(CString("\n"), 0);
		if (nPos > 0)
		{
			m_strShowMsg.Delete(0, nPos + 2);
		};

	}
	SetDlgItemText(IDC_UserID, m_strShowMsg);
	//AfxMessageBox(m_strShowMsg);
	m_EditView.LineScroll(m_EditView.GetLineCount(), 0);

	
	if (m_bConnect)
	{
	
		int index = 0;
		memset(R_temp, 0x0, 400);
		memcpy(R_temp, strMessage.c_str(), strMessage.length());
		//Name_5
		if (R_temp[0] == 'N'&R_temp[1] == 'a')
		{
			if (R_temp[5] >= '0'&&R_temp[5] <= '9') global_led_id = R_temp[5] - '0';
			else global_led_id = 10;

			memcpy(R_temp, strMessage.c_str(), strMessage.length());
			memset(sname[global_led_id], 0x0, 200);
			memcpy(sname[global_led_id], &R_temp[7], strMessage.length() - 7);

			//if (dwHand[global_led_id] != 0)
			{
				//tmp.Format("        %s", sname[i]);

				my_area.AreaX = 0;
				my_area.AreaY = 0;
				my_area.AreaType = 0x07;
				my_area.AreaWidth = led_sname[global_led_id].w;//    24;
				my_area.AreaHeight = led_sname[global_led_id].h;// 64;
				my_area.Lines_sizes = 0;

				my_area.Reserved[0] = 0;
				my_area.Reserved[1] = 0;
				my_area.Reserved[2] = 0;

				my_area.DynamicAreaLoc = 0;   //¶¨ÒåÒ»¸ö¶¯Ì¬
				my_area.RunMode = 0;//RunMode_list[rl];

				my_area.Timeout = 1;
				my_area.SingleLine = 2;
				my_area.NewLine = 2;
				my_area.DisplayMode = 1;
				my_area.ExitMode = 0x00;
				my_area.Speed = 0;
				my_area.StayTime = 0;
				my_area.DataLen = strMessage.length() - 7;// tmp.GetLength();// strlen(sname[i]);

				led_ready = global_led_id;


				//ret = SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, strMessage.length() - 7, (BYTE*)sname[global_led_id]);
				//if (ret != 0) AfxMessageBox("error:\r\n");

				

			}
		}





		

	}
}

void CLed_toolDlg::readAndWriteConfigure(char* log, int nWrite)
{
	TCHAR* pName = NULL;
	TCHAR strPath[MAX_PATH] = { 0 };

	/* »ñÈ¡µ±Ç°Ö´ÐÐ³ÌÐòµÄÂ·¾¶ */
	if (GetModuleFileName(NULL, strPath, MAX_PATH))
	{
		pName = _tcsrchr(strPath, '\\');
		if (NULL != pName)
		{
			memset(pName + 1, 0, _tcslen(pName));
			_tcscat(strPath, _T("websocket.ini"));
		}
	}

	if (nWrite)
	{
		FILE *fp = _tfopen(strPath, _T("w"));    /*½¨Á¢Ò»¸öÎÄ×ÖÎÄ¼þ¶ÁÐ´*/
		if (NULL != fp)
		{
			fputs(log, fp);
			fputs("\n", fp);
			fflush(fp);
			fclose(fp);
		}
	}
	else
	{
		FILE* fp = _tfopen(strPath, _T("r"));    /*½¨Á¢Ò»¸öÎÄ×ÖÎÄ¼þ¶ÁÐ´*/
		if (NULL != fp)
		{
			fgets(log, 256, fp);
			fclose(fp);
		}
	}
}

char * CLed_toolDlg::UnicodeToGB2312(WCHAR uData, char buffer[2])
{
	WideCharToMultiByte(CP_ACP, NULL, &uData, 1, buffer, sizeof(WCHAR), NULL, NULL);
	return buffer;
}

WCHAR * CLed_toolDlg::UTF_8ToUnicode(char *pText, WCHAR &unicode)
{
	/*
	UTF-8ÊÇÒ»ÖÖ¶à×Ö½Ú±àÂëµÄ×Ö·û¼¯£¬±íÊ¾Ò»¸öUnicode×Ö·ûÊ±£¬Ëü¿ÉÒÔÊÇ1¸öÖÁ¶à¸ö×Ö½Ú£¬ÔÚ±íÊ¾ÉÏÓÐ¹æÂÉ£º
	1×Ö½Ú£º0xxxxxxx
	2×Ö½Ú£º110xxxxx 10xxxxxx
	3×Ö½Ú£º1110xxxx 10xxxxxx 10xxxxxx
	4×Ö½Ú£º11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	char *uchar = (char *)&unicode;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	return &unicode;
}

char * CLed_toolDlg::TranslateUTF8ToGB(char *str, size_t len)
{
	char* newCharBuffer = new char[len + 1];
	int index = 0;
	int nCBIndex = 0;
	WCHAR wTemp = 0;
	char cTemp[2] = " ";
	while (index < len)
	{
		if (str[index] == 0)
			break;
		else if (str[index] > 0)  // Èç¹ûÊÇGB2312µÄ×Ö·û  
		{
			newCharBuffer[nCBIndex] = str[index];    //Ö±½Ó¸´ÖÆ  
			index += 1;    //Ô´×Ö·û´®Æ«ÒÆÁ¿1  
			nCBIndex += 1;   //Ä¿±ê×Ö·û´®Æ«ÒÆÁ¿1  
		}
		else      //Èç¹ûÊÇUTF-8µÄ×Ö·û  
		{
			UTF_8ToUnicode(str + index, wTemp);   //ÏÈ°ÑUTF-8×ª³ÉUnicode  
			UnicodeToGB2312(wTemp, &newCharBuffer[nCBIndex]); //ÔÙ°ÑUnicode ×ª³É GB2312  
			index += 3;    //Ô´×Ö·û´®Æ«ÒÆÁ¿3  
			nCBIndex += 2;   //Ä¿±ê×Ö·û´®Æ«ÒÆÁ¿2  ÒòÎªÒ»¸öÖÐÎÄUTF-8Õ¼3¸ö×Ö½Ú£¬GB2312Õ¼Á½¸ö×Ö½Ú  
		}
	}
	newCharBuffer[nCBIndex] = '\0'; //½áÊø·û  
	strcpy(str, newCharBuffer);
	delete[] newCharBuffer;  //±ÜÃâÄÚ´æÐ¹Â©  
	newCharBuffer = NULL;
	return str;
}


void CLed_toolDlg::OnEnChangeUserid()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CLed_toolDlg::OnTimer(UINT nIDEvent)
{
	// TODO: A
	CString tmp;
	USHORT len;
	CString prompt;
	bx_5k_area_header my_area;
	int i = 0;

	if (nIDEvent == 2) //Time2 deal
	{
		if (m_bConnect == FALSE)
		{
			OnWebBnClickedConnect();
			USES_CONVERSION;
			//	char* s_char = W2A(led_Status.GetBuffer(led_Status.GetLength());
			//	std::string input = s_char;
				//char* s_char = W2A(L"");
				//std::string input = s_char;
			Sleep(1000);
			//endpoint.send(input);
			if (led_error)
			{
				//led_error = 0;
				endpoint.send(led_Status.GetBuffer(led_Status.GetLength()));
			}


		}
	}
	if (nIDEvent ==1)//Time1 deal
	{
		if (m_bConnect)
		{

			USES_CONVERSION;
			char* s_char = W2A(L"ping");
			std::string input = s_char;
			endpoint.send(input);

		}

	}


}


DWORD ThreadProcEvent_LED1(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 0)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED2(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 1)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED3(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 2)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED4(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 3)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED5(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 4)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED6(LPVOID pParam)
{


	while (1)
	{
		if (led_ready == 5)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED7(LPVOID pParam)
{



	while (1)
	{
		if (led_ready == 6)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED8(LPVOID pParam)
{
	


	while (1)
	{
		if (led_ready == 7)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED9(LPVOID pParam)
{
	


	while (1)
	{
		if (led_ready == 8)
		{
			if (dwHand[global_led_id] != 0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

DWORD ThreadProcEvent_LED10(LPVOID pParam)
{



	while (1)
	{
		if(led_ready ==9)
		{
			if(dwHand[global_led_id]!=0) SCREEN_SendDynamicArea(dwHand[global_led_id], my_area, my_area.DataLen, (BYTE*)sname[global_led_id]);
			led_ready = 10;
		}
		Sleep(5);
	}
}

void CLed_toolDlg::OnStaticLink()
{
	// TODO: Add your command handler code here
	ShellExecute(0, NULL, _T("http://www.suzhouweiyao.com/"), NULL, NULL, SW_NORMAL);
}

void CLed_toolDlg::OnStaticLinkMove()
{
	// TODO: Add your command handler code here
	//ShellExecute(0, NULL, _T("http://www.suzhouweiyao.com/"), NULL, NULL, SW_NORMAL);
	AfxMessageBox("M");
}


void CLed_toolDlg::OnStnClickedStaticPort2()
{
	// TODO: Add your control notification handler code here
}


HBRUSH CLed_toolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_LINK)
	{
		//pDC->SetBkColor(RGB(0, 255, 0));//背景色为绿色
		if(mouse_flag ==1)pDC->SetTextColor(RGB(0, 0, 255));//文字
		else              pDC->SetTextColor(RGB(255, 0, 0));//文字
		//pDC->SelectObject(&m_font);//文字为15号字体，华文行楷
		return m_brush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
