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

char szFile[300][100];
FILE *fp;
CString strPORT;
DWORD dwIP[3];
int m_clor =0;
int m_chrct =3;
SOCKET m_socket[10];
HANDLE hThreadEvent1;  //事件线程句柄2
unsigned char g_tmp[3][50];
CString display;
BOOL DL_FLAG =0;
BOOL dl =0;
BOOL dl_state =0;
DWORD ThreadProcEvent1(LPVOID pParam);

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

	for(int j = 0 ; j < TAG_NUM ; j ++)
	{
		sname[j] = new char[80];
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
    ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON5, &CLed_toolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CLed_toolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CLed_toolDlg::OnBnClickedButton7)
	ON_EN_CHANGE(IDC_Port, &CLed_toolDlg::OnEnChangePort)
	ON_EN_CHANGE(IDC_UserID, &CLed_toolDlg::OnEnChangeUserid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLed_toolDlg message handlers
#include "ini.h"
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
#if 1
	OnGetjiqima();
	License_Enable = strcmp(Get_Machine_data,  "00E18CB63528000806E9-00000000-00000000");
	//License_Enable = strcmp(Get_Machine_data,"00E04C0741E1000206A7-00000000-00000000"); //01
	if (License_Enable)
	{
		strwarning.Format("Warning:Soft Have error License\n:%s", Get_Machine_data);
		AfxMessageBox(strwarning);

		OnOK();
			
	}
#endif
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
    ConfigPathName.Format("%s\\config.ini",str_url);
	CIni nIniParam(ConfigPathName);
memset(sname[0],0x0,50);
memset(sname[1],0x0,50);
memset(sname[2],0x0,50);
memset(sname[3],0x0,50);
memset(sname[4],0x0,50);
memset(sname[5],0x0,50);
memset(sname[6], 0x0, 50);
memset(sname[7], 0x0, 50);
memset(sname[8], 0x0, 50);
memset(sname[9], 0x0, 50);
memset(store_id, 0x0, 100);
	strInfo = nIniParam.GetValue("SIS_Name", "name_1", "FFFFFFFF");
	memcpy(sname[0],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_2", "FFFFFFFF");
	memcpy(sname[1],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_3", "FFFFFFFF");
	memcpy(sname[2],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_4", "FFFFFFFF");
	memcpy(sname[3],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_5", "FFFFFFFF");
	memcpy(sname[4],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_6", "FFFFFFFF");
	memcpy(sname[5],strInfo.GetBuffer(strInfo.GetLength()),strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_7", "FFFFFFFF");
	memcpy(sname[6], strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_8", "FFFFFFFF");
	memcpy(sname[7], strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_9", "FFFFFFFF");
	memcpy(sname[8], strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("SIS_Name", "name_10", "FFFFFFFF");
	memcpy(sname[9], strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());

	strInfo = nIniParam.GetValue("StoreID", "server_ip", "FFFFFFFF");
	memcpy(store_id, strInfo.GetBuffer(strInfo.GetLength()), strInfo.GetLength());


	::GetPrivateProfileString("UseFileName","FileName","Error",OpenFilePath.GetBuffer(MAX_PATH),MAX_PATH,ConfigPathName);

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

	//hThreadEvent1=CreateThread(NULL,0,
						//  (LPTHREAD_START_ROUTINE)ThreadProcEvent1,
						//  NULL,0,NULL);  //创建事件线程
     OnBnClickedButton7();
	SetTimer(2,1000,NULL);
	SetTimer(1, 30000, NULL);
	
	//CloseHandle(hThreadEvent1);






	
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

void CLed_toolDlg::OnPaint() 
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
			MessageBox("绑定失败!");
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
   	CString str_FileExt="certlib文件 (*.*)|*.*";
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

CString CLed_toolDlg::GetTimeStr(void)						//时间转换为字符串
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


	struct sockaddr_in ser_addr;/* 服务器的地址*/
	int addrlen =0;
	CString tmp;

	/* 初始化服务器地址*/


	CString tempBuf[5];
	tempBuf[0] ="请求中...";
	tempBuf[1] ="请求中...";
	tempBuf[2] ="请求中...";


	
	while(1)
	{

		if(DL_FLAG)
		{
			display.Format("连接显示屏%s\r\n",tempBuf[0]);
			dl =1 ;
			addrlen=sizeof(struct sockaddr_in);
			
			
			
			ser_addr.sin_family=AF_INET;
			
			ser_addr.sin_addr.s_addr=htonl(dwIP[0]);
			
			 ser_addr.sin_port=htons(atoi(strPORT));
		
		if(connect(m_socket[0],(struct sockaddr*)&ser_addr,sizeof(SOCKADDR))!=0)/*请求连接*/
			
		{
			
		
				
			tempBuf[0] ="超时";
			
		}
		else
		{

		   tempBuf[0] ="成功";
		   dl_state = 1;

	
		}
		display.Format("连接显示屏%s\r\n",tempBuf[0]);
		DL_FLAG = 0;
		dl =1 ;
	

		}
		Sleep(5);
	}

}

LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //参数lpcstr类型也可是char*
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




// 连接
void CLed_toolDlg::Connect() 
{
	// TODO: Add your control notification handler code here
	char*	sips[ProxyNum];		//服务器IP	16字节
	char*	susers[ProxyNum];	//登录账号	16字节
	char*	spasss[ProxyNum];	//登录密码	16字节
	WORD	wport[ProxyNum];	//端口

	long j ;
	for( j = 0 ; j < ProxyNum ; j ++)
	{
		sips[j] = new char[16];
		susers[j] = new char[16];
		spasss[j] = new char[16];
	}

	// 设置连接参数
	//strcpy(sips[0], "10.219.109.198");
	//strcpy(sips[0], "172.18.108.110");
	strcpy(sips[0], "172.18.108.21");
	strcpy(susers[0], "admin");
	strcpy(spasss[0], "admin");
	wport[0] = 12084;

	m_dwHandle = DBPCreate(sips,susers,spasss,wport, ProxyNum); //创建对象

	//创建对象 启动连接
	DWORD ret;
	if(m_dwHandle != NULL)
	{
		ret = DBPConnect(m_dwHandle);
		if(ret == 0)
		{
			AfxMessageBox("DBPConnect");
		}
	}	

	//取连接信息
	CString szt;
	char     sip[16];
	int      pnNum;
	if(m_dwHandle != NULL )
	{
		ret = DBPGetSvrConInfo(m_dwHandle,sip,&pnNum);
	}
	//	szt.Format("连接耗时: %f ms ,连接的服务器: %s",span , sip  );
	szt.Format("连接的服务器: %s", sip  );

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


		tmp.Format("led t 0 20 6 压 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
        tmp.Format("led t 0 48 6 力 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 18 26 供汽量 \n");
		send(m_socket[0],tmp.GetBuffer(tmp.GetLength()),tmp.GetLength(),0);
		Sleep(5);
		tmp.Format("led t 0 18 46 售汽量 \n");
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
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP   ; 
	nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy(nid.szTip, "计划任务提醒 ");//信息提示条为“计划任务提醒” 
	Shell_NotifyIcon(NIM_ADD,&nid);//在托盘区添加图标 
	ShowWindow(SW_HIDE);//隐藏主窗口 
	//CDialog::OnClose();
}


 LRESULT   CLed_toolDlg::onShowTask(WPARAM   wParam,LPARAM   lParam) 
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{ 
    if(wParam!=IDR_MAINFRAME) 
    return   1; 
    switch(lParam) 
    { 
         case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭” 
         { 
              LPPOINT   lpoint=new   tagPOINT; 
              ::GetCursorPos(lpoint);//得到鼠标位置 
              CMenu   menu; 
              menu.CreatePopupMenu();//声明一个弹出式菜单 
              //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已 
              //隐藏），将程序结束。 
              menu.AppendMenu(MF_STRING,WM_DESTROY, "Exit ");   
              //确定弹出式菜单的位置 
              menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
              //资源回收 
              HMENU   hmenu=menu.Detach(); 
              menu.DestroyMenu(); 
              delete   lpoint; 
			  CreateRun();
           } 
           break; 
           case WM_LBUTTONDBLCLK://双击左键的处理 
           { 
               this-> ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
           } 
           break; 
      } 
      return   0; 
  } 

int CLed_toolDlg::CreateRun(void)
{
//添加以下代码
 HKEY   RegKey;  
 CString   sPath;  
 GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);  
 sPath.ReleaseBuffer();  
 int   nPos;  
 nPos=sPath.ReverseFind('\\');  
 sPath=sPath.Left(nPos);  
 CString   lpszFile=sPath+"\\led_tool.exe";//这里加上你要查找的执行文件名称  
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
  RegSetValueEx(RegKey,"led_tool",0,REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());//这里加上你需要在注册表中注册的内容  
  this->UpdateData(FALSE);  
 }  
 else  
 {  
  //theApp.SetMainSkin();  
  ::AfxMessageBox("没找到执行程序，自动运行失败");  
  exit(0);  
 }  
 return 0;
} 

void CLed_toolDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//Connect();
	//readdata();

	OnWebBnClickedConnect();
}


// 读基本实时
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
	
		ret =  DBPGetSnapshot( //读快照
			m_dwHandle,
			sname, //标签名数组
			ltimes,	//in/out, 时标
			snqas,	//in/out, 质量
			dblals,	//in/out, 存放double值,DT_FLOAT32,DT_FLOAT64存放区
			lvals,	//in/out, 存放Long值,DT_DIGITAL,DT_INT32,DT_INT64存放区
			ntypes,	//in/out, 数据类型,DT_INT32,DT_FLOAT32等。
			errs,		 //in/out, 错误码
			bufsize		 //in, 个数
			);//返回错误码



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
	//OnMcEvent回调不在主线程中。需再次转发到主线程中，否则会刷新界面有时会卡死 (界面系统一般都不是线程安全的)
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
   //(1)程序初始化时
   ret = mcInit(8300,(TOnMcEvent)test,1);
   
  // int m = mcFindBoxByIP("192.168.0.77");
  // ret = mcBoxIsConnect(m);
  // mcConnectBox("192.168.0.77",8100);
  // ret = mcGetBoxCount();
}

void CLed_toolDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	Mc_onConnect();
}

CString led_Status;
int      led_error = 0;
void CLed_toolDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE programLif[8] = {0xff,};
	BYTE AreaDatatLis[8] = {0,};
	BYTE TXT[80] = {'A',};
	BYTE SAT[80] = {0,};
	CString tmp;

		
	
//	sprintf(TXT,"来之二次开发");
	InitSdk(2, 2);
	
//	CreateBroadCast(char *broad_ip,UINT broad_port,bx_5k_card_type card_type);

	dwHand[0] = CreateClient("192.168.0.201", 5005, BX_5MK1, 1, 1, NULL);
#if 1
	dwHand[1] = CreateClient("192.168.0.202", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[2]  = CreateClient("192.168.0.203", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[3]  = CreateClient("192.168.0.204", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[4]  = CreateClient("192.168.0.205", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[5]  = CreateClient("192.168.0.206", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[6] = CreateClient("192.168.0.207", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[7] = CreateClient("192.168.0.208", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[8] = CreateClient("192.168.0.209", 5005, BX_5MK1, 1, 1, NULL);
	dwHand[9] = CreateClient("192.168.0.210", 5005, BX_5MK1, 1, 1, NULL);
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
	//AfxMessageBox(prompt);
//	SetTimer(1, 5000, NULL);

	 //CON_Reset(dwHand);
}



void CLed_toolDlg::OnEnChangePort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


#include   <httpext.h>   
#include   <windef.h>   
#include   <Nb30.h>

#pragma comment(lib, "Netapi32.lib")

void CLed_toolDlg::OnGetjiqima()
{
	// TODO: Add your control notification handler code here
	unsigned long s1, s2;
	unsigned char vendor_id[] = "------------";//CPU提供商ID
	CString str1, str2, str3;
	// 以下为获得CPU ID的汇编语言指令
  //   _asm    // 得到CPU提供商信息 
  //   {  
  //       xor eax,eax   // 将eax清0
  //       cpuid         // 获取CPUID的指令
  //       mov dword ptr vendor_id,ebx
  //       mov dword ptr vendor_id[+4],edx
  //       mov dword ptr vendor_id[+8],ecx  
  //   }
  //   str1.Format("%s",vendor_id);

	_asm    // 得到CPU ID的高32位 
	{
		mov eax, 01h
		xor edx, edx
		cpuid
		mov s2, eax
	}
	str2.Format("%08X-", s2);

	_asm    // 得到CPU ID的低64位
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
	// TODO: 在此添加控件通知处理程序代码
	CString strJson = _T("{\"id\":\"123456\",\"username\":\"456789\",\"password\":\"00000\",\"addr\":\"地址地址直直直起卦\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}],\"json\":{\"id\":\"0000\",\"username\":\"000001\"}}");
	Json::Reader reader;
	Json::Value root;

	USES_CONVERSION;
	char* cJson = T2A(strJson.GetBuffer(0));
	strJson.ReleaseBuffer();

	CString addr, username, cpp, jsonid;

	if (reader.parse(cJson, root))
	{
		//取得数据
		addr = root["addr"].asCString();
		username = root["username"].asCString();
		//遍历JSON数组
		const Json::Value arrayObj = root["array"];
		for (unsigned int i = 0; i < arrayObj.size(); i++)
		{
			if (!arrayObj[i].isMember("cpp"))
				continue;
			cpp = arrayObj[i]["cpp"].asCString();
		}
		//取得JSON数据
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnect)
	{
		endpoint.close();
		
	}
}


void CLed_toolDlg::OnWebBnClickedClearMsg()
{
	// TODO:  在此添加控件通知处理程序代码
//    if (m_bConnect)
	{
		m_strShowMsg = "";
		//SetDlgItemText(IDC_MSG_SHOW, m_strShowMsg);
	}
}


void CLed_toolDlg::OnWebBnClickedSend()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_bConnect)
	{
		//GetDlgItemText(IDC_MSG_SEND, m_strInputMsg);
		if (m_strInputMsg.IsEmpty())
		{
			AfxMessageBox("请先输入消息,再进行发送...");
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
		AfxMessageBox("请先进行连接后再次尝试发送...");
	}
}

void CLed_toolDlg::OnWebBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
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
void CLed_toolDlg::onWebMessage(std::string strMessage)
{
	//AfxMessageBox((char*)strMessage.c_str());
	bx_5k_area_header my_area;
	connect_web_ok = 1;
	strMessage = TranslateUTF8ToGB((char*)strMessage.c_str(), strMessage.length());
	char R_temp[200];
	int led_id = 0;
	if (m_bConnect)
	{
	
		int index = 0;
		memset(R_temp, 0x0, 200);
		memcpy(R_temp, strMessage.c_str(), strMessage.length());
		led_id = R_temp[5]-'0';
		memcpy(R_temp, strMessage.c_str(), strMessage.length());
		memset(sname[led_id], 0x0, 200);
		memcpy(sname[led_id], &R_temp[7], strMessage.length() - 7);

		if (dwHand[led_id] != 0)
		{
			//tmp.Format("        %s", sname[i]);

			my_area.AreaX = 0;
			my_area.AreaY = 0;
			my_area.AreaType = 0x07;
			my_area.AreaWidth = 24;
			my_area.AreaHeight = 64;
			my_area.Lines_sizes = 0;

			my_area.Reserved[0] = 1;
			my_area.Reserved[1] = 1;
			my_area.Reserved[2] = 1;

			my_area.DynamicAreaLoc = 0;   //定义一个动态
			my_area.RunMode = 0;//RunMode_list[rl];

			my_area.Timeout = 10;
			my_area.SingleLine = 2;
			my_area.NewLine = 2;
			my_area.DisplayMode = 1;
			my_area.ExitMode = 0x00;
			my_area.Speed = 0;
			my_area.StayTime = 0;
			my_area.DataLen = strMessage.length() - 7;// tmp.GetLength();// strlen(sname[i]);



			SCREEN_SendDynamicArea(dwHand[led_id], my_area, strMessage.length() - 7, (BYTE*)sname[led_id]);
			//SCREEN_SendDynamicArea(dwHand[i], my_area, strlen(sname[i]), (BYTE*)sname[i]);

	}



#if 0
		if (!strMessage.empty())
		{
			while ((index = strMessage.find(' ', index)) != std::string::npos)
			{
				strMessage.erase(index, 1);
			}
		}
#endif

		index = 0;
		if (!strMessage.empty())
		{
			while ((index = strMessage.find('\t', index)) != std::string::npos)
			{
				strMessage.erase(index, 1);
			}
		}

		index = 0;
		if (!strMessage.empty())
		{
			while ((index = strMessage.find('\n', index)) != std::string::npos)
			{
				strMessage.erase(index, 1);
			}
		}


		m_strShowMsg += CString(strMessage.c_str()) + L"\r\n";
		if (m_strShowMsg.GetLength() > 1024000 * 5)
		{
			int nPos = m_strShowMsg.Find(CString("\r\n"), 0);
			if (nPos > 0)
			{
				m_strShowMsg.Delete(0, nPos + 2);
			};

		}
		SetDlgItemText(IDC_UserID, m_strShowMsg);
		//AfxMessageBox(m_strShowMsg);
		m_EditView.LineScroll(m_EditView.GetLineCount(), 0);


	}
}

void CLed_toolDlg::readAndWriteConfigure(char* log, int nWrite)
{
	TCHAR* pName = NULL;
	TCHAR strPath[MAX_PATH] = { 0 };

	/* 获取当前执行程序的路径 */
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
		FILE *fp = _tfopen(strPath, _T("w"));    /*建立一个文字文件读写*/
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
		FILE* fp = _tfopen(strPath, _T("r"));    /*建立一个文字文件读写*/
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
	UTF-8是一种多字节编码的字符集，表示一个Unicode字符时，它可以是1个至多个字节，在表示上有规律：
	1字节：0xxxxxxx
	2字节：110xxxxx 10xxxxxx
	3字节：1110xxxx 10xxxxxx 10xxxxxx
	4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
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
		else if (str[index] > 0)  // 如果是GB2312的字符  
		{
			newCharBuffer[nCBIndex] = str[index];    //直接复制  
			index += 1;    //源字符串偏移量1  
			nCBIndex += 1;   //目标字符串偏移量1  
		}
		else      //如果是UTF-8的字符  
		{
			UTF_8ToUnicode(str + index, wTemp);   //先把UTF-8转成Unicode  
			UnicodeToGB2312(wTemp, &newCharBuffer[nCBIndex]); //再把Unicode 转成 GB2312  
			index += 3;    //源字符串偏移量3  
			nCBIndex += 2;   //目标字符串偏移量2  因为一个中文UTF-8占3个字节，GB2312占两个字节  
		}
	}
	newCharBuffer[nCBIndex] = '\0'; //结束符  
	strcpy(str, newCharBuffer);
	delete[] newCharBuffer;  //避免内存泄漏  
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
			if (led_error) endpoint.send(led_Status.GetBuffer(led_Status.GetLength()));


		}
	}
<<<<<<< HEAD
	if (nIDEvent ==1)//Time1 deal
=======
	if (nIDEvent == 1)//Time1 deal
>>>>>>> 50a751dfa9b963d842705431548dc263c3394b53
	{
		if (m_bConnect)
		{

			USES_CONVERSION;
			char* s_char = W2A(L"ping");
			std::string input = s_char;
			endpoint.send(input);

<<<<<<< HEAD


=======


>>>>>>> 50a751dfa9b963d842705431548dc263c3394b53
		}

	}


}
