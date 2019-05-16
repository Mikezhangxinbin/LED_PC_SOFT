//---------------------------------------------------------------------------



#include "McControl.h"


#include "time.h"
//#include "McWord.h"
//#pragma comment( lib , "McWord.lib")	//d7 test

//把Ansi字串转为unicode
const wchar_t* ws(AnsiString s)
{
	static unsigned int s_idx=0;
	static WideString s_tmp[8];
	s_idx++;
	if (s_idx>=8) s_idx=0;
	s_tmp[s_idx]=s;
	return s_tmp[s_idx].c_bstr();
}

void __stdcall OnMcEvent(int event,HBOX boxno,int para1,int para2)
{
	//OnMcEvent回调不在主线程中。需再次转发到主线程中，否则会刷新界面有时会卡死 (界面系统一般都不是线程安全的)
	int w,l;
	w = (event<<16)|(boxno);//boxno<65535
	l = (para1<<16)| para2;
	::PostMessageA(FrmMain->Handle,WM_MCEVENT,w,l);
}

//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
	mcInit(8300,OnMcEvent,1);
	m_sExePath=ExtractFilePath(Application->ExeName);
	m_sMcPath = m_sExePath + "mcdata";
	m_SamplePath = m_sExePath+"sample\\";
	m_sFtpRoot = "d:\\ftp";
	m_sCurMcvPath="";
	m_uploadBox=0;
	if (!DirectoryExists(m_sMcPath))
	{
		MkDir(m_sMcPath);
	}
	m_sMcPath = m_sMcPath+"\\";
	//
	m_IsInitDisplay=false;
	if (mcDisplayInit(pnlScreen->Handle,pnlScreen->Color))
	{
		m_IsInitDisplay=true;
	}

	m_displayWidth0 = pnlScreen->Width;
	m_displayHeight0 = pnlScreen->Height;

}
//---------------------------------------------------------------------------
AnsiString TimeStr(time_t t=0)
{
	if (t==0)
	{
		t=time(NULL);
	}
	tm* t1=localtime(&t);
	char st[40];
	sprintf(st,"%02d:%02d:%02d",
		t1->tm_hour,
		t1->tm_min,
		t1->tm_sec
		);
	return st;
}

void	TFrmMain::ps(AnsiString s)
{
	//开启预览之后，BCB的Now()失效 （双精度运算冲突），造成时间显示停止。所以避用Now(). 用自定义的 TimeStr
	//Memo1->Lines->Add(TimeToStr(Now())+" "+s);

	Memo1->Lines->Add(TimeStr()+" "+s);//改用这句

}

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
	mcClose();
}
//---------------------------------------------------------------------------
int		TFrmMain::GetRow(int boxno)
{
	for (int i=0;i<m_idlist.size();i++)
	{
		if (m_idlist[i]==boxno)
		{
			return i;
		}
	}
	return -1;
}

int		TFrmMain::GetBox(int row)
{
	if (row<1 || row>m_idlist.size())  return 0;
	return m_idlist[row-1];
}


void	TFrmMain::onConnect(int boxno)
{
     //收到连接，把盒子IP名字填到列表中
	int idx=GetRow(boxno);
	TMcBoxInfo bf;
	if (!mcGetBoxInfo(boxno,&bf))
	{
		return;
	}
	if (idx<0)
	{
		m_idlist.push_back(boxno);
		idx = m_idlist.size()-1;
		if (idx+2>=grid1->RowCount)
		{
			grid1->RowCount=idx+2;
		}
		int r = idx+1;
		grid1->Cells[0][r]=bf.ip;
		grid1->Cells[1][r]=bf.Name;
		grid1->Cells[2][r]="在线";
	}
	else
	{
		int r = idx+1;
		grid1->Cells[2][r]="在线";
		grid1->Cells[0][r]=bf.ip;  //再次上线，IP可能会变!
		grid1->Cells[1][r]=bf.Name;
	}
}

void	TFrmMain::onDisConnect(int boxno)
{
    //断开连接，更新在线状态
	int idx=GetRow(boxno);
	if (idx<0)
	{
		return;
	}
	int r = idx+1;
	grid1->Cells[2][r]="";
}


void __fastcall TFrmMain::Button2Click(TObject *Sender)
{
	AnsiString mcvname="test1";
	AnsiString mcvpath=m_sMcPath+mcvname;//mcdata/test1
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("上传失败");
	}
	lblUpload->Caption="正在上传:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Timer1Timer(TObject *Sender)
{
	//刷新进度
	if (m_uploadBox)
	{

		int jd=0;
		if (m_upmode==0)
		{
			jd = mcGetUploadProgress(m_uploadBox);
			lblUpload->Caption="正在下载..";
		}
		if (m_upmode==1)
		{
			jd = mcGetProgressFTP(m_uploadBox);
			int state = mcGetFTPState(m_uploadBox);
			//0-无 1-正下载 2-正常结束 3-连结中 4-连接异常/无法登录  5-停止取消 6-磁盘满  7-目录错误
			AnsiString s1="无FTP下载";
			switch (state){
			case 1:s1="正下载";break;
			case 2:s1="已结束";break;
			case 3:s1="连结中";break;
			case 4:s1="FTP异常";break;
			case 5:s1="已取消";break;
			case 6:s1="磁盘满";break;
			case 7:s1="目录错误";break;
			}
			lblUpload->Caption=s1;
		}
		ProgressBar1->Position = jd*ProgressBar1->Max/100;
		if (jd>=100)
		{
			m_uploadBox=0;
			lblUpload->Caption="已结束.";
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button1Click(TObject *Sender)
{
	AnsiString mcvname="test1";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//指定节目名以及窗口大小
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//添加节目内容
	//添加一个节目页
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//添加一个文字框
	TMcText	text;
	mcGetDefaultText(&text);
	text.left=50;
	text.top=60;
	text.width=500;
	text.height=68;
	text.backcolor=0x80FF;
	text.alpha=0;
	wcscpy(text.fontname,L"楷体");
	text.fontsize=32;
	text.alignx=1;
	mcAddSingleText(pageno,L"你好 SDK  !",-1,&text);

	//图片
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=50;
	img.top=170;
	img.width=160;
	img.height=120;
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));

	//视频
	TMcVideo	video;
	mcGetDefaultVideo(&video);
	video.left=250;
	video.top=170;
	video.width=160;
	video.height=120;
	int videoid=mcAddVideoBox(pageno,&video);
	mcAddVideoFile(videoid,ws(m_SamplePath+"test2.mp4"));

	//时间
	TMcInfo	info;
	mcGetDefaultInfo(&info);
	info.left=50;
	info.top=15;
	info.width=400;
	info.height=32;
	mcAddInfoBoxTime(pageno,mcdate_time,&info);
	//

	//保存  （不保存也可预览）
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,已保存. 可预览 "+mcvpath);
	}

	//关闭
	//mcCloseMcv(mcv);   //先不关闭，以便预览
}

void TFrmMain::test()
{

}


//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button3Click(TObject *Sender)
{
	//假定test1节目已经经保存到m_sFtpRoot下
	AnsiString svrpath="/test1";
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}

	WideString ws1=svrpath;
	TMcFTP ftp;
	wcscpy(ftp.svrip,L"192.168.0.8");
	ftp.port=21;
	wcscpy(ftp.user,L"user");
	wcscpy(ftp.pwd,L"8888");
	wcscpy(ftp.charset,L"GBK");
	//
	ProgressBar1->Position=0;
	if (!mcStartFTP(boxno,ws1.c_bstr(),NULL,1,&ftp))
	{
		ShowMessage("上传失败");
	}
	lblUpload->Caption="由FTP下载:"+svrpath;
	m_upmode=1;
	m_uploadBox=boxno;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button4Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcStopUpload(boxno);
	mcStopFTP(boxno);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button5Click(TObject *Sender)
{
	AnsiString mcvname="test2";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//指定节目名以及窗口大小
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);

	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//添加节目内容
	//添加一个节目页
	int pageno=mcAddPage(0,pm_Normal,1,0);

	TMcNotice	notice;
	mcGetDefaultNotice(&notice);
	notice.left=0;
	notice.top=0;
	notice.width=640;
	notice.height=360;
	int noticeid=mcAddNoticeBox(pageno,&notice);
	mcAddNoticeRTF(noticeid,ws(m_SamplePath+"通告.rtf"),ani_XiangShangYiDong);
	//保存
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,已保存. 可预览"+mcvpath);
	}
	////关闭
	//mcCloseMcv(mcv);  ///先不关闭，以便预览
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button6Click(TObject *Sender)
{
	if (Timer2->Enabled)
	{
		Timer2->Enabled=false;
		return;
	}
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	Timer2->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Timer2Timer(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno>0)
	{
		wchar_t ws1[200];
		static int  tno=1;
		swprintf(ws1,L"动态文字 %d",tno++);
		mcSendSingleText(boxno,0,L"文字框1",ws1);
	}

}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btnPlayClick(TObject *Sender)
{

	if (m_IsInitDisplay)
	{
		mcDisplayShow();
		btnStop->Enabled=true;
		TimerDisplay->Enabled=true;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::TimerDisplayTimer(TObject *Sender)
{
	mcDisplayDraw();
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::btnStopClick(TObject *Sender)
{
	btnStop->Enabled=false;
	TimerDisplay->Enabled=false;
	mcDisplayStop();

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button7Click(TObject *Sender)
{
	AnsiString ip=txtIP->Text;
	HBOX boxno=mcFindBoxByIP(ip.c_str());
	if (boxno>0 && mcBoxIsConnect(boxno))
	{
		ShowMessage("该盒子已连接");
		return;
    }
	mcConnectBox(ip.c_str(),8100);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button8Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno>0)
	{
		mcBoxReboot(boxno);
	}
	//
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::grid1Click(TObject *Sender)
{
	lblLedRem->Caption=grid1->Cells[0][grid1->Row];
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button9Click(TObject *Sender)
{
	TDateTime t1=Now();
	ShortDateFormat="yyyy-MM-dd";
	LongTimeFormat="hh:mm:ss";
	AnsiString s1=t1.DateTimeString();
	int boxno=GetBox(grid1->Row);
	if (boxno>0)
	{
		mcBoxSetTime(boxno,s1.c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::OnMcEventMsg(TMessage& Message)
{
	int event = Message.WParamHi;
	HBOX boxno = Message.WParamLo;
	int para1 = Message.LParamHi;
	int para2 = Message.LParamLo;

	if (boxno==0)
	{
	   if (event==MCEVENT_ERROR)
	   {
			FrmMain->ps("连接出错:"+IntToStr(para1));
	   }
	   return;
	}
	TMcBoxInfo bf;
	if (!mcGetBoxInfo(boxno,&bf))
	{
		return;
	}
	WideString ws1;
	ws1=bf.Name;
	AnsiString srem=ws1;
	srem=srem+" "+bf.ip;
	//
	switch (event) {
	case MCEVENT_CONNECT:
	{
		FrmMain->ps("有连接:"+srem);
		FrmMain->onConnect(boxno);
	}
	break;
	case MCEVENT_DISCONNECT:
	{
		FrmMain->ps("连接中断:"+srem);
		FrmMain->onDisConnect(boxno);
	}
	break;
	case MCEVENT_ERROR:
	{
		FrmMain->ps("连接出错:"+IntToStr(para1)+" "+srem);
	}
	break;
	default:;
	}
}
void __fastcall TFrmMain::cbDisplayResizeClick(TObject *Sender)
{
	int idx = cbDisplayResize->ItemIndex;
	float zoom = 1.0f;
	zoom  = (idx+1);
	//先把预览窗口进行缩放
	pnlScreen->Width = m_displayWidth0*zoom;
	pnlScreen->Height = m_displayHeight0*zoom;
	//然后通知mc预览引擎
	mcDisplayResize();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridMcvClick(TObject *Sender)
{
	AnsiString mcvname="testgrid1";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//指定节目名以及窗口大小
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//添加节目内容
	//添加一个节目页
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//表线宽度
	int bw=1;
	TMcText	text;

	int col=4;
	int row=7;

	//添加多个文字框，组成阵列
	int x,y;
	int x1=30;//左边距
	int y1=60;//上边距
	int colwidth[4]={180,120,120,120};
    wchar_t* head[4]={L"姓名",L"语文",L"数学",L"英语"};


	//标题
	mcGetDefaultText(&text);
	wcscpy(text.fontname,L"微软雅黑");
	text.fontsize=20;
	text.alignx=1;
	text.left=150;
	text.top=4;
	text.width=300;
	text.height=60;
	text.drawmode=0;
	mcAddSingleText(pageno,L"表格示例",-1,&text);

	int w,h;//cell宽，高

	int bordercolor=0xffa0a0a0;//表线色
	//添加表头
	h=48;
	x=x1;
	y=y1;
	for (int c=0;c<col;c++)
	{
		mcGetDefaultText(&text);
		wcscpy(text.fontname,L"微软雅黑");
		text.fontsize=18;

		w=colwidth[c];
		text.left=x;
		text.top=y;
		text.width=w;
		text.height=h;
		text.backcolor=0x8000FF;
		text.alpha=80;
		text.borderwidth=bw;
		text.bordercolor=bordercolor;
		text.drawmode=0;
		mcAddSingleText(pageno,head[c],-1,&text);
		//
		x += w;
	}

	//添加多个text作为表格单元

	y=y1+h;
	h=32;
	for (int r=0;r<row;r++)	//共row行
	{
		x=x1;
		for (int c=0;c<col;c++)
		{
			mcGetDefaultText(&text);
			wcscpy(text.fontname,L"微软雅黑");
			text.fontsize=14;
			//
			w=colwidth[c];
			swprintf(text.name,L"T%d%d",r,c);
			OutputDebugStringW(text.name);
			OutputDebugStringW(L"\n");
			text.left=x;
			text.top=y;
			text.width=w;
			text.height=h;
			text.backcolor=0;
			text.alpha=80;
			text.borderwidth=bw;
			text.bordercolor=bordercolor;
			text.drawmode=0;
			mcAddSingleText(pageno,L"",-1,&text);
			//
			x += w;
		}
		y += h;
	}

	//定义表格视图
	wchar_t  namelist[2000];
	namelist[0]=0;
	for (int r=0;r<row;r++)
	{
		for (int c=0;c<col;c++)
		{
			wchar_t  txtname[40];
			swprintf(txtname,L"T%d%d|",r,c);
			wcscat(namelist,txtname);
		}
    }
	int ret=mcAddGrid(pageno,1,col,namelist);
	if (ret!=row*col)
	{
		ShowMessage("表格定义有误!");
	}
	//

	//保存  （不保存也可预览）
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,已保存. 可预览 "+mcvpath);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridDataClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcGridData(boxno,1,4,
		L"王小毛|41|63|32|"
		L"余清英|92|87|83|"
		L"美云|86|88|92|"
		L"刘娟|96|49|76|"
	);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button11Click(TObject *Sender)
{
	AnsiString mcvname="testgrid1";
	AnsiString mcvpath=m_sMcPath+mcvname;//mcdata/test1
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("上传失败");
	}
	lblUpload->Caption="正在上传:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;	
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridClearClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcGridClear(boxno,1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnAddRowClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcGridAddRow(boxno,1,
		L"张平|61|95|78|"
	);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button10Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	TMessageBox msg;
	memset(&msg,0,sizeof(msg));
	msg.width=180;
	msg.height=120;
	wcscpy(msg.title,L"A2014 雷军");
	msg.fontsize = 20;
	msg.fontcolor=0xFFFFFF;
	wcscpy(msg.title2,L"请到3号门诊室");
	msg.fontsize2=16;
	msg.fontcolor2=0xFFFFFF;
	msg.backcolor = 0x80FF;
	msg.timeout=8000;
	wcscpy(msg.backimg,L"panel1.png");
	mcShowMessage(boxno,&msg);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnUploadImageClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcClearSendFile(boxno);

	WideString sfile;
	sfile = m_SamplePath+"panel1.png";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"李洁.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"张智明.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"李建新.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"周文文.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"sample1.mp4";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"sample2.mp4";
	mcAddSendFile(boxno,sfile.c_bstr());

	if (!mcStartSendFile(boxno))
	{
		ShowMessage("上传失败!");
		return;
	}
	m_uploadBox=boxno;
	Timer3->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Timer3Timer(TObject *Sender)
{
	float jd = mcGetUploadProgress(m_uploadBox)*0.01f;
	ProgressBar2->Position = jd*ProgressBar2->Max;
	if (jd>=1.0f)
	{
		Timer3->Enabled=false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnChangeImgClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	TMcBoxInfo boxinfo;
	mcGetBoxInfo(boxno,&boxinfo);

	if (wcscmp(boxinfo.mcv,L"testcard"))
	{
		ShowMessage("请先手动上传并播放sample/testcard节目");
		return;
	}

	static t=0;
	if (t==0)
	{
		t=1;
		//mcSetImageBox(boxno,L"图片框1",L"李洁.jpg");
		//mcSendTexts(boxno,0,L"姓名:李洁|职业:医师|科室:骨外科");
		mcSetImageBox(boxno,L"照片1",L"李洁.jpg");
		mcSendTexts(boxno,1,L"大夫1:李洁|职称1:医师|就诊者1:张新城");

	}
	else
	{
		t=0;
		//mcSetImageBox(boxno,L"图片框1",L"李建新.jpg");
		//mcSendTexts(boxno,0,L"姓名:李建新|职业:医师|科室:内科");
		mcSetImageBox(boxno,L"照片1",L"李建新.jpg");
		mcSendTexts(boxno,1,L"大夫1:李建新|职称1:副主任医师|就诊者1:刘丽以及她的小朋友们");
	}
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button12Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	static int state=0;
	if (state==0)
	{
		state=1;
		mcSendSingleText(boxno,0,L"文字框1",L"热烈欢迎各位领导莅临本公司指导工作！",1);
	}
	else
	{
		state=0;
		mcSendSingleText(boxno,0,L"文字框1",L"开饭了",1);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button13Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcSendSingleText(boxno,0,L"文字框1",L"热烈欢迎各位领导莅临本公司指导工作！\n所有员工请于10:20到大会议室参加表彰大会!",2);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button14Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	static t=0;
	if (t==0)
	{
		t=1;
		mcSetVideoBox(boxno,L"影视框1",L"sample2.mp4");
	}
	else
	{
		t=0;
		mcSetVideoBox(boxno,L"影视框1",L"sample1.mp4");
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button15Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	//mcDelUserFile(boxno,L"sample1.*");
	//mcDelUserFile(boxno,L"*.jpg");
	//mcDelUserFile(boxno,L"sample*");
	mcDelUserFile(boxno,L"*.*");
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnSendImgMcvClick(TObject *Sender)
{
	AnsiString mcvname="testimg1";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//指定节目名以及窗口大小
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//添加节目内容
	//添加一个节目页
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//添加一个文字框
	TMcText	text;
	mcGetDefaultText(&text);
	text.left=50;
	text.top=60;
	text.width=500;
	text.height=68;
	text.backcolor=0x80FF;
	text.alpha=0;
	text.fontsize=32;
	text.alignx=1;
	mcAddSingleText(pageno,L"动态换图演示",-1,&text);

	//图片
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=50;
	img.top=170;
	img.width=160;
	img.height=120;
	wcscpy(img.name,L"图片框1");
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));


	//保存  （不保存也可预览）
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		//ShowMessage("ok,已保存. 可预览 "+mcvpath);
	}

	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("上传失败");
	}
	lblUpload->Caption="正在上传:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnSetRGBClick(TObject *Sender)
{
	int imgw=160;
	int imgh=120;
	int buflen=imgw*imgh*3;
	char* buf=new char[buflen];
	//生成一个从上到下渐变的红色
	char* d=buf;
	for (int y=0;y<imgh;y++)
	{
		int c = y+32;
		for (int x=0;x<imgw;x++)
		{
			d[0]=c;//r
			d[1]=0;//g
			d[2]=0; //b
			d += 3;
		}
	}
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}
	mcSetImageBoxData(boxno,L"图片框1",0,IMGDATA_RGB,buf,buflen,imgw,imgh);
	delete[] buf;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button18Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}

	AnsiString sfile = m_SamplePath+"周文文.jpg";
	FILE* f=fopen(sfile.c_str(),"rb");
	if (f==NULL) return;
	fseek(f,0,SEEK_END);
	int len = ftell(f);
	fseek(f,0,SEEK_SET);
	char* buf = new char[len];
	fread(buf,len,1,f);
	fclose(f);
	mcSetImageBoxData(boxno,L"图片框1",0,IMGDATA_JPG,buf,len,0,0);
	delete[] buf;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button16Click(TObject *Sender)
{
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("请先选择盒子");
		return;
	}

	//----生成一个节目------------------
	AnsiString mcvname="test3";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//指定节目名以及窗口大小
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//添加节目内容
	//添加一个节目页
	int pageno=mcAddPage(0,pm_Normal,1,0x602000);


	//图片
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=0;
	wcscpy(img.name,L"图片框1");
	img.top=0;
	img.width=320;
	img.height=200;
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));
	mcSetObjVisible(pageno,imgid,0);//让图片不可见

	//添加一个文字框
	TMcText	text;
	mcGetDefaultText(&text);
	wcscpy(text.name,L"文字1");
	text.left=50;
	text.top=60;
	text.width=500;
	text.height=68;
	text.fontcolor=0x0080FF;
	text.alpha=0;
	text.fontsize=32;
	text.alignx=1;
	mcAddSingleText(pageno,L"动态字色演示",-1,&text);

	mcGetDefaultText(&text);
	wcscpy(text.name,L"文字2");
	text.left=240;
	text.top=160;
	text.width=300;
	text.height=68;
	text.fontcolor=0xFFFFFF;
	text.alpha=0;
	text.fontsize=24;
	text.alignx=1;
	int id2=mcAddSingleText(pageno,L"测试文字1234",-1,&text);
	mcSetObjVisible(pageno,id2,0);//先让文字不可见





	//保存  （不保存也可预览）
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
	}
	else
	{
		return;
	}

	//----------------------
	//上传它
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("上传失败");
	}
	lblUpload->Caption="正在上传:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button17Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		return;
	}
	mcSendObjVisible(boxno,0,L"文字2",1);
	mcSendObjVisible(boxno,0,L"图片框1",1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button19Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		return;
	}
	static bool b1=false;
	if (b1)
	{
		mcSendTextStyle(boxno,0,L"文字1",L"fontcolor=#FFFFFFFF;backcolor=#00000000;");
	}
	else
	{
		mcSendTextStyle(boxno,0,L"文字1",L"fontcolor=#FFFFFF00;backcolor=#80FF0000;");
	}
	b1=!b1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button20Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		return;
	}
	static bool b1=false;
	if (b1)
	{
		mcSendTextStyle(boxno,0,L"文字2",L"fontcolor=#FFFFFF00;bordercolor=#FF00FF00;borderwidth=4;fontsize=20;");
	}
	else
	{
		mcSendTextStyle(boxno,0,L"文字2",L"fontcolor=#FFFF0000;borderwidth=0;fontsize=28;");
	}
	b1=!b1;
}
//---------------------------------------------------------------------------

