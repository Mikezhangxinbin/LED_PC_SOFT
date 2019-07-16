// led_toolDlg.h : header file
//

#if !defined(AFX_LED_TOOLDLG_H__C0A057E1_5B72_497B_9D3D_350F5F642D37__INCLUDED_)
#define AFX_LED_TOOLDLG_H__C0A057E1_5B72_497B_9D3D_350F5F642D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SHOWTASK (WM_USER+10)

#define TAG_NUM 500  //点数
#define ProxyNum 1 //代理服务器数目
#include "dbpapi_j.h"
#include "McControl.h"
#include "websocket_endpoint.h"
#include "afxwin.h"
#include "Resource.h"
/*
获取程序运行时间
*/
class   CTimeCount   
{   
public:   
	   CTimeCount(){};   
	   ~CTimeCount(){};   
	   void		start()
	   {
		   QueryPerformanceFrequency(&litmp);   
		   dfFreq   =   (double)litmp.QuadPart;//   获得计数器的时钟频率   
		   QueryPerformanceCounter(&litmp);   
		   QPart1   =   litmp.QuadPart;//   获得初始值   
	   };     //开始计时   
	   void		end()
	   {
		   QueryPerformanceCounter(&litmp);   
		   QPart2   =   litmp.QuadPart;//获得中止值   
		   dfMinus   =   (double)(QPart2-QPart1);   
		   dfTim   =   dfMinus   /   dfFreq;//   获得对应的时间值，单位为秒 
	   };         //停止计时   
	   
	   double	time_milli(){
		   return dfTim * 1000.0;
	   };       //以毫秒精度输出所用时间   
	  
private:   
	LARGE_INTEGER   litmp;     
    LONGLONG   QPart1,QPart2;   
    double   dfMinus,   dfFreq,   dfTim;     
}; 

/////////////////////////////////////////////////////////////////////////////
// CLed_toolDlg dialog



typedef  struct _Led_Screen
{
	char IP[50];
	unsigned int x;
	unsigned int y;
	unsigned char id;
	unsigned char w;
	unsigned char h;
}Led_Screen;

class CLed_toolDlg : public CDialog
{
// Construction
public:
	CLed_toolDlg(CWnd* pParent = NULL);	// standard constructor
    BOOL  InitSocket();
	void  OnBtnSend2();
    SOCKET client_socket;
	CString m_strip;
    CString	str_ip;
	CString OpenFilePath;
	CString GetTimeStr(void);
    afx_msg void OnTimer(UINT nIDEvent);
	int CLed_toolDlg::CreateRun(void);
    afx_msg   LRESULT   onShowTask(WPARAM   wParam,LPARAM   lParam); 
	void find(char * lpPath);
	//void LedString_display(unsigned char clor,unsigned int x,unsigned char y,char *p);
    CString ConfigPathName;
    void Mc_onConnect(void);
	DWORD m_dwHandle;
	char Get_Machine_data[200];
	Led_Screen led_sname[50];
	
	char*	sdbname[TAG_NUM];		//in/out数据库实例名 16
	char*	sdbtagname[TAG_NUM];	//in/out数据库标签名 80
	char*	sdes[TAG_NUM];			//in/out描述 80
	char*	sunit[TAG_NUM];			//in/out单位 16
	DWORD   dwid[TAG_NUM];			//in/out标签ID
	WORD	wtype[TAG_NUM];			//in/out数据类型

	char	store_id[200];			//in/out标签名 80
	BOOL License_Enable;
	void Connect();
	void readdata();
// Dialog Data
	//{{AFX_DATA(CLed_toolDlg)
	enum { IDD = IDD_LED_TOOL_DIALOG };
	CComboBox	m_Obj_Chrct;
	CButton	m_login;
	CComboBox	m_ctrlPort;
	CString	m_statedisplay;
	void OnGetjiqima();
	void Get_Data_From_Json();
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLed_toolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CString m_strAddress;
	// 发送消息
	CString m_strInputMsg;
	// 消息显示
	CString m_strShowMsg;

	BOOL m_bConnect;

	void onWebOpen();
	void onWebFail(std::string strReason);
	void onWebClose(int nCode, std::string strCode, std::string strReason);
	void onWebMessage(std::string strMessage);
	afx_msg void OnWebBnClickedCancel();
	CEdit m_EditView;
	afx_msg void OnWebBnClickedConnect();
	afx_msg void OnWebBnClickedDisconnect();
	afx_msg void OnWebBnClickedClearMsg();
	afx_msg void OnWebBnClickedSend();
private:
	kagula::websocket_endpoint endpoint;

	void readAndWriteConfigure(char* log, int nWrite = 1);
	char* UnicodeToGB2312(WCHAR uData, char buffer[2]);
	WCHAR* UTF_8ToUnicode(char *pText, WCHAR &unicode);
	char* TranslateUTF8ToGB(char *str, size_t len);
// Implementation
protected:
	HICON m_hIcon;
   

	// Generated message map functions
	//{{AFX_MSG(CLed_toolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onchoosefile();
	afx_msg void Onupdate();
	afx_msg void Ondenglu();
	afx_msg void Onupdata();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnBnClickedButton7();
	afx_msg void OnEnChangePort();
	afx_msg void OnEnChangeUserid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LED_TOOLDLG_H__C0A057E1_5B72_497B_9D3D_350F5F642D37__INCLUDED_)
