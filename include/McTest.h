//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTimer *Timer1;
	TTimer *Timer2;
	TTimer *TimerDisplay;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TStringGrid *grid1;
	TMemo *Memo1;
	TTabSheet *TabSheet2;
	TLabel *lblUpload;
	TButton *Button1;
	TButton *Button2;
	TProgressBar *ProgressBar1;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TTabSheet *TabSheet3;
	TPanel *pnlScreen;
	TButton *btnPlay;
	TButton *btnStop;
	TTabSheet *TabSheet4;
	TLabel *Label1;
	TEdit *txtIP;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TLabel *lblLedRem;
	TLabel *Label2;
	TComboBox *cbDisplayResize;
	TTabSheet *TabSheet5;
	TButton *btnGridMcv;
	TButton *btnGridData;
	TButton *Button11;
	TButton *btnGridClear;
	TButton *btnAddRow;
	TTabSheet *TabSheet6;
	TButton *btnUploadImage;
	TProgressBar *ProgressBar2;
	TTimer *Timer3;
	TButton *btnChangeImg;
	TButton *Button10;
	TButton *Button12;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TTabSheet *TabSheet7;
	TButton *btnSendImgMcv;
	TButton *btnSetRGB;
	TButton *Button18;
	TTabSheet *TabSheet8;
	TButton *Button16;
	TButton *Button17;
	TButton *Button19;
	TButton *Button20;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Timer2Timer(TObject *Sender);
	void __fastcall btnPlayClick(TObject *Sender);
	void __fastcall TimerDisplayTimer(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall grid1Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall cbDisplayResizeClick(TObject *Sender);
	void __fastcall btnGridMcvClick(TObject *Sender);
	void __fastcall btnGridDataClick(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall btnGridClearClick(TObject *Sender);
	void __fastcall btnAddRowClick(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall btnUploadImageClick(TObject *Sender);
	void __fastcall Timer3Timer(TObject *Sender);
	void __fastcall btnChangeImgClick(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall Button14Click(TObject *Sender);
	void __fastcall Button15Click(TObject *Sender);
	void __fastcall btnSendImgMcvClick(TObject *Sender);
	void __fastcall btnSetRGBClick(TObject *Sender);
	void __fastcall Button18Click(TObject *Sender);
	void __fastcall Button16Click(TObject *Sender);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
private:	// User declarations
	vector<int>		m_idlist;
	AnsiString		m_sExePath;
	AnsiString		m_SamplePath;
	//所有临时节目根路径
	AnsiString		m_sMcPath;
	//当前测试节目路径
	AnsiString		m_sCurMcvPath;
	//FTP根目录
	AnsiString		m_sFtpRoot;
	//获得box所在grid行号
	int		GetRow(int boxno);
	//由行号获得boxon
	int		GetBox(int gridrow);
	//当前正在上传的盒子(用于测试)
	int			m_uploadBox;
	int			m_upmode;
	int			m_displayWidth0;
	int			m_displayHeight0;
	bool		m_IsInitDisplay;
	void		test();

	//利用windows消息机制，把OnMcEvent回调，转到主线程中执行。
BEGIN_MESSAGE_MAP
MESSAGE_HANDLER(WM_MCEVENT, TMessage, OnMcEventMsg)
END_MESSAGE_MAP(TForm)

	void __fastcall OnMcEventMsg(TMessage& Message);

public:		// User declarations
	__fastcall TFrmMain(TComponent* Owner);
	void	ps(AnsiString s);
	void	onConnect(int boxno);
	void	onDisConnect(int boxno);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
