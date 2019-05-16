//---------------------------------------------------------------------------



#include "McControl.h"


#include "time.h"
//#include "McWord.h"
//#pragma comment( lib , "McWord.lib")	//d7 test

//��Ansi�ִ�תΪunicode
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
	//OnMcEvent�ص��������߳��С����ٴ�ת�������߳��У������ˢ�½�����ʱ�Ῠ�� (����ϵͳһ�㶼�����̰߳�ȫ��)
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
	//����Ԥ��֮��BCB��Now()ʧЧ ��˫���������ͻ�������ʱ����ʾֹͣ�����Ա���Now(). ���Զ���� TimeStr
	//Memo1->Lines->Add(TimeToStr(Now())+" "+s);

	Memo1->Lines->Add(TimeStr()+" "+s);//�������

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
     //�յ����ӣ��Ѻ���IP������б���
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
		grid1->Cells[2][r]="����";
	}
	else
	{
		int r = idx+1;
		grid1->Cells[2][r]="����";
		grid1->Cells[0][r]=bf.ip;  //�ٴ����ߣ�IP���ܻ��!
		grid1->Cells[1][r]=bf.Name;
	}
}

void	TFrmMain::onDisConnect(int boxno)
{
    //�Ͽ����ӣ���������״̬
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
		ShowMessage("����ѡ�����");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("�ϴ�ʧ��");
	}
	lblUpload->Caption="�����ϴ�:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Timer1Timer(TObject *Sender)
{
	//ˢ�½���
	if (m_uploadBox)
	{

		int jd=0;
		if (m_upmode==0)
		{
			jd = mcGetUploadProgress(m_uploadBox);
			lblUpload->Caption="��������..";
		}
		if (m_upmode==1)
		{
			jd = mcGetProgressFTP(m_uploadBox);
			int state = mcGetFTPState(m_uploadBox);
			//0-�� 1-������ 2-�������� 3-������ 4-�����쳣/�޷���¼  5-ֹͣȡ�� 6-������  7-Ŀ¼����
			AnsiString s1="��FTP����";
			switch (state){
			case 1:s1="������";break;
			case 2:s1="�ѽ���";break;
			case 3:s1="������";break;
			case 4:s1="FTP�쳣";break;
			case 5:s1="��ȡ��";break;
			case 6:s1="������";break;
			case 7:s1="Ŀ¼����";break;
			}
			lblUpload->Caption=s1;
		}
		ProgressBar1->Position = jd*ProgressBar1->Max/100;
		if (jd>=100)
		{
			m_uploadBox=0;
			lblUpload->Caption="�ѽ���.";
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button1Click(TObject *Sender)
{
	AnsiString mcvname="test1";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//ָ����Ŀ���Լ����ڴ�С
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//��ӽ�Ŀ����
	//���һ����Ŀҳ
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//���һ�����ֿ�
	TMcText	text;
	mcGetDefaultText(&text);
	text.left=50;
	text.top=60;
	text.width=500;
	text.height=68;
	text.backcolor=0x80FF;
	text.alpha=0;
	wcscpy(text.fontname,L"����");
	text.fontsize=32;
	text.alignx=1;
	mcAddSingleText(pageno,L"��� SDK  !",-1,&text);

	//ͼƬ
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=50;
	img.top=170;
	img.width=160;
	img.height=120;
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));

	//��Ƶ
	TMcVideo	video;
	mcGetDefaultVideo(&video);
	video.left=250;
	video.top=170;
	video.width=160;
	video.height=120;
	int videoid=mcAddVideoBox(pageno,&video);
	mcAddVideoFile(videoid,ws(m_SamplePath+"test2.mp4"));

	//ʱ��
	TMcInfo	info;
	mcGetDefaultInfo(&info);
	info.left=50;
	info.top=15;
	info.width=400;
	info.height=32;
	mcAddInfoBoxTime(pageno,mcdate_time,&info);
	//

	//����  ��������Ҳ��Ԥ����
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,�ѱ���. ��Ԥ�� "+mcvpath);
	}

	//�ر�
	//mcCloseMcv(mcv);   //�Ȳ��رգ��Ա�Ԥ��
}

void TFrmMain::test()
{

}


//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button3Click(TObject *Sender)
{
	//�ٶ�test1��Ŀ�Ѿ������浽m_sFtpRoot��
	AnsiString svrpath="/test1";
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
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
		ShowMessage("�ϴ�ʧ��");
	}
	lblUpload->Caption="��FTP����:"+svrpath;
	m_upmode=1;
	m_uploadBox=boxno;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button4Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
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
	//ָ����Ŀ���Լ����ڴ�С
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);

	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//��ӽ�Ŀ����
	//���һ����Ŀҳ
	int pageno=mcAddPage(0,pm_Normal,1,0);

	TMcNotice	notice;
	mcGetDefaultNotice(&notice);
	notice.left=0;
	notice.top=0;
	notice.width=640;
	notice.height=360;
	int noticeid=mcAddNoticeBox(pageno,&notice);
	mcAddNoticeRTF(noticeid,ws(m_SamplePath+"ͨ��.rtf"),ani_XiangShangYiDong);
	//����
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,�ѱ���. ��Ԥ��"+mcvpath);
	}
	////�ر�
	//mcCloseMcv(mcv);  ///�Ȳ��رգ��Ա�Ԥ��
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
		ShowMessage("����ѡ�����");
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
		swprintf(ws1,L"��̬���� %d",tno++);
		mcSendSingleText(boxno,0,L"���ֿ�1",ws1);
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
		ShowMessage("�ú���������");
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
			FrmMain->ps("���ӳ���:"+IntToStr(para1));
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
		FrmMain->ps("������:"+srem);
		FrmMain->onConnect(boxno);
	}
	break;
	case MCEVENT_DISCONNECT:
	{
		FrmMain->ps("�����ж�:"+srem);
		FrmMain->onDisConnect(boxno);
	}
	break;
	case MCEVENT_ERROR:
	{
		FrmMain->ps("���ӳ���:"+IntToStr(para1)+" "+srem);
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
	//�Ȱ�Ԥ�����ڽ�������
	pnlScreen->Width = m_displayWidth0*zoom;
	pnlScreen->Height = m_displayHeight0*zoom;
	//Ȼ��֪ͨmcԤ������
	mcDisplayResize();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridMcvClick(TObject *Sender)
{
	AnsiString mcvname="testgrid1";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//ָ����Ŀ���Լ����ڴ�С
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//��ӽ�Ŀ����
	//���һ����Ŀҳ
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//���߿��
	int bw=1;
	TMcText	text;

	int col=4;
	int row=7;

	//��Ӷ�����ֿ��������
	int x,y;
	int x1=30;//��߾�
	int y1=60;//�ϱ߾�
	int colwidth[4]={180,120,120,120};
    wchar_t* head[4]={L"����",L"����",L"��ѧ",L"Ӣ��"};


	//����
	mcGetDefaultText(&text);
	wcscpy(text.fontname,L"΢���ź�");
	text.fontsize=20;
	text.alignx=1;
	text.left=150;
	text.top=4;
	text.width=300;
	text.height=60;
	text.drawmode=0;
	mcAddSingleText(pageno,L"���ʾ��",-1,&text);

	int w,h;//cell����

	int bordercolor=0xffa0a0a0;//����ɫ
	//��ӱ�ͷ
	h=48;
	x=x1;
	y=y1;
	for (int c=0;c<col;c++)
	{
		mcGetDefaultText(&text);
		wcscpy(text.fontname,L"΢���ź�");
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

	//��Ӷ��text��Ϊ���Ԫ

	y=y1+h;
	h=32;
	for (int r=0;r<row;r++)	//��row��
	{
		x=x1;
		for (int c=0;c<col;c++)
		{
			mcGetDefaultText(&text);
			wcscpy(text.fontname,L"΢���ź�");
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

	//��������ͼ
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
		ShowMessage("���������!");
	}
	//

	//����  ��������Ҳ��Ԥ����
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		ShowMessage("ok,�ѱ���. ��Ԥ�� "+mcvpath);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridDataClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	mcGridData(boxno,1,4,
		L"��Сë|41|63|32|"
		L"����Ӣ|92|87|83|"
		L"����|86|88|92|"
		L"����|96|49|76|"
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
		ShowMessage("����ѡ�����");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("�ϴ�ʧ��");
	}
	lblUpload->Caption="�����ϴ�:"+ExtractFileName(mcvpath);
	m_upmode=0;
	m_uploadBox=boxno;	
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnGridClearClick(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
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
		ShowMessage("����ѡ�����");
		return;
	}
	mcGridAddRow(boxno,1,
		L"��ƽ|61|95|78|"
	);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button10Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	TMessageBox msg;
	memset(&msg,0,sizeof(msg));
	msg.width=180;
	msg.height=120;
	wcscpy(msg.title,L"A2014 �׾�");
	msg.fontsize = 20;
	msg.fontcolor=0xFFFFFF;
	wcscpy(msg.title2,L"�뵽3��������");
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
		ShowMessage("����ѡ�����");
		return;
	}
	mcClearSendFile(boxno);

	WideString sfile;
	sfile = m_SamplePath+"panel1.png";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"���.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"������.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"���.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"������.jpg";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"sample1.mp4";
	mcAddSendFile(boxno,sfile.c_bstr());
	sfile = m_SamplePath+"sample2.mp4";
	mcAddSendFile(boxno,sfile.c_bstr());

	if (!mcStartSendFile(boxno))
	{
		ShowMessage("�ϴ�ʧ��!");
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
		ShowMessage("����ѡ�����");
		return;
	}
	TMcBoxInfo boxinfo;
	mcGetBoxInfo(boxno,&boxinfo);

	if (wcscmp(boxinfo.mcv,L"testcard"))
	{
		ShowMessage("�����ֶ��ϴ�������sample/testcard��Ŀ");
		return;
	}

	static t=0;
	if (t==0)
	{
		t=1;
		//mcSetImageBox(boxno,L"ͼƬ��1",L"���.jpg");
		//mcSendTexts(boxno,0,L"����:���|ְҵ:ҽʦ|����:�����");
		mcSetImageBox(boxno,L"��Ƭ1",L"���.jpg");
		mcSendTexts(boxno,1,L"���1:���|ְ��1:ҽʦ|������1:���³�");

	}
	else
	{
		t=0;
		//mcSetImageBox(boxno,L"ͼƬ��1",L"���.jpg");
		//mcSendTexts(boxno,0,L"����:���|ְҵ:ҽʦ|����:�ڿ�");
		mcSetImageBox(boxno,L"��Ƭ1",L"���.jpg");
		mcSendTexts(boxno,1,L"���1:���|ְ��1:������ҽʦ|������1:�����Լ�����С������");
	}
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::Button12Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	static int state=0;
	if (state==0)
	{
		state=1;
		mcSendSingleText(boxno,0,L"���ֿ�1",L"���һ�ӭ��λ�쵼ݰ�ٱ���˾ָ��������",1);
	}
	else
	{
		state=0;
		mcSendSingleText(boxno,0,L"���ֿ�1",L"������",1);
    }

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button13Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	mcSendSingleText(boxno,0,L"���ֿ�1",L"���һ�ӭ��λ�쵼ݰ�ٱ���˾ָ��������\n����Ա������10:20��������Ҳμӱ��ô��!",2);

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button14Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	static t=0;
	if (t==0)
	{
		t=1;
		mcSetVideoBox(boxno,L"Ӱ�ӿ�1",L"sample2.mp4");
	}
	else
	{
		t=0;
		mcSetVideoBox(boxno,L"Ӱ�ӿ�1",L"sample1.mp4");
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button15Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
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
	//ָ����Ŀ���Լ����ڴ�С
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//��ӽ�Ŀ����
	//���һ����Ŀҳ
	int pageno=mcAddPage(0,pm_Normal,1,0);

	//���һ�����ֿ�
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
	mcAddSingleText(pageno,L"��̬��ͼ��ʾ",-1,&text);

	//ͼƬ
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=50;
	img.top=170;
	img.width=160;
	img.height=120;
	wcscpy(img.name,L"ͼƬ��1");
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));


	//����  ��������Ҳ��Ԥ����
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
		//ShowMessage("ok,�ѱ���. ��Ԥ�� "+mcvpath);
	}

	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("�ϴ�ʧ��");
	}
	lblUpload->Caption="�����ϴ�:"+ExtractFileName(mcvpath);
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
	//����һ�����ϵ��½���ĺ�ɫ
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
		ShowMessage("����ѡ�����");
		return;
	}
	mcSetImageBoxData(boxno,L"ͼƬ��1",0,IMGDATA_RGB,buf,buflen,imgw,imgh);
	delete[] buf;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button18Click(TObject *Sender)
{
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}

	AnsiString sfile = m_SamplePath+"������.jpg";
	FILE* f=fopen(sfile.c_str(),"rb");
	if (f==NULL) return;
	fseek(f,0,SEEK_END);
	int len = ftell(f);
	fseek(f,0,SEEK_SET);
	char* buf = new char[len];
	fread(buf,len,1,f);
	fclose(f);
	mcSetImageBoxData(boxno,L"ͼƬ��1",0,IMGDATA_JPG,buf,len,0,0);
	delete[] buf;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::Button16Click(TObject *Sender)
{
	//mcStartSendMcv()
	int boxno=GetBox(grid1->Row);
	if (boxno==0)
	{
		ShowMessage("����ѡ�����");
		return;
	}

	//----����һ����Ŀ------------------
	AnsiString mcvname="test3";
	AnsiString mcvpath=m_sMcPath+mcvname;
	//ָ����Ŀ���Լ����ڴ�С
	HMCV mcv = mcCreateMcv(ws(m_sMcPath),ws(mcvname),640,360);
	if (mcv==NULL) return;
	mcSetCurMcv(mcv);
	//��ӽ�Ŀ����
	//���һ����Ŀҳ
	int pageno=mcAddPage(0,pm_Normal,1,0x602000);


	//ͼƬ
	TMcImage	img;
	mcGetDefaultImg(&img);
	img.left=0;
	wcscpy(img.name,L"ͼƬ��1");
	img.top=0;
	img.width=320;
	img.height=200;
	int imgid=mcAddImageBox(pageno,&img);
	mcAddImageFile(imgid,ws(m_SamplePath+"test1.jpg"));
	mcSetObjVisible(pageno,imgid,0);//��ͼƬ���ɼ�

	//���һ�����ֿ�
	TMcText	text;
	mcGetDefaultText(&text);
	wcscpy(text.name,L"����1");
	text.left=50;
	text.top=60;
	text.width=500;
	text.height=68;
	text.fontcolor=0x0080FF;
	text.alpha=0;
	text.fontsize=32;
	text.alignx=1;
	mcAddSingleText(pageno,L"��̬��ɫ��ʾ",-1,&text);

	mcGetDefaultText(&text);
	wcscpy(text.name,L"����2");
	text.left=240;
	text.top=160;
	text.width=300;
	text.height=68;
	text.fontcolor=0xFFFFFF;
	text.alpha=0;
	text.fontsize=24;
	text.alignx=1;
	int id2=mcAddSingleText(pageno,L"��������1234",-1,&text);
	mcSetObjVisible(pageno,id2,0);//�������ֲ��ɼ�





	//����  ��������Ҳ��Ԥ����
	if (mcSaveMcv(mcv))
	{
		m_sCurMcvPath=mcvpath;
	}
	else
	{
		return;
	}

	//----------------------
	//�ϴ���
	ProgressBar1->Position=0;
	WideString ws1;
	ws1=mcvpath;
	if (!mcStartSendMcv(boxno,ws1.c_bstr()))
	{
		ShowMessage("�ϴ�ʧ��");
	}
	lblUpload->Caption="�����ϴ�:"+ExtractFileName(mcvpath);
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
	mcSendObjVisible(boxno,0,L"����2",1);
	mcSendObjVisible(boxno,0,L"ͼƬ��1",1);
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
		mcSendTextStyle(boxno,0,L"����1",L"fontcolor=#FFFFFFFF;backcolor=#00000000;");
	}
	else
	{
		mcSendTextStyle(boxno,0,L"����1",L"fontcolor=#FFFFFF00;backcolor=#80FF0000;");
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
		mcSendTextStyle(boxno,0,L"����2",L"fontcolor=#FFFFFF00;bordercolor=#FF00FF00;borderwidth=4;fontsize=20;");
	}
	else
	{
		mcSendTextStyle(boxno,0,L"����2",L"fontcolor=#FFFF0000;borderwidth=0;fontsize=28;");
	}
	b1=!b1;
}
//---------------------------------------------------------------------------

