//===================================================================
/**\file  
 *  Filename:   McControl.h
 *  Desc:       MC100���ź� ���ݷ���/���ƽӿ�
 *		
 *				���Cʾ�����룬���Ķ�TestSdk/main.cpp
 *      
 *  His:      __stdcall mcsoft  create @  2015-7-10   14:44
 */
//===================================================================
#ifndef McControl_h__
#define McControl_h__

#include "McSdkBase.h"

#define MAXLEN_BOXNAME	40
#define MAXLEN_MCVNAME	100

//������ʱ��ţ��������ļ��������������ʱ���ֵ���䡣ֻҪ���������رգ��������������������������ţ�
typedef		int		HBOX;
typedef		long long MCID;


#ifdef MCCONTROL_EXPORTS
#define _DLLFUNC_MCCONTROL  __declspec(dllexport) 
#else
#define _DLLFUNC_MCCONTROL  __declspec(dllimport) 
#endif

#define		MCEVENT_CONNECT			100	//�к�������
#define		MCEVENT_DISCONNECT		101	//�к��ӶϿ�����
#define		MCEVENT_ERROR			102	//�����ӳ���(boxno=0����ʾ��������ʱ���ӳ���)


//������ɫ 0xAABBGGRR
#define MC_COLOR_RED	0xFF0000FF	//��
#define MC_COLOR_GREEN	0xFF00FF00
#define MC_COLOR_BLUE	0xFFFF0000
#define MC_COLOR_WHITE	0xFFFFFFFF	//��
#define MC_COLOR_BLACK	0xFF000000	//��
#define MC_COLOR_YELLOW	0xFF00FFFF	//��
#define MC_COLOR_RED2	0x800000FF	//��͸����
#define MC_COLOR_NULL	0x00000000	//͸��

//�ص����������������¼�֪ͨ
//-----------------------------------
// �ص�����: OnMcEvent
// ����: ��McControl.dll�ص��ϲ����֪ͨ���ӵ�������Ͽ����¼�
// ��ע������ص����������߳��У����� ���������������ˢ��������ͨ���ǲ���ȫ�ģ���ʱ����Ῠ�������Խ�������������У������Ҫˢ�������棬Ҫ������¼��ٴ�post message�����߳��С�
// ����: 
//      event��			�¼����ͣ��μ�MCEVENT_CONNECT��
//		boxno			���źд���
//		para1,para2		���ò���
//-----------------------------------
typedef void (__stdcall *TOnMcEvent)(int event,HBOX boxno,int para1,int para2);



#define  MCMSG_COMMON		21
#define  MCMSG_FAIL			22
//�ص����������Ӻ�������ִ�й�����һЩ��Ϣ֪ͨ
//-----------------------------------
// �ص�����: OnMcMessage
// ����: ����McControl������һЩ��Ҫ����Ϣ������԰�_mess��ӡ����־�����У��Ա���ԡ���Щ��������ʧ�ܷ���false,��ͬʱ�ڴ�����쳣��Ϣ.�磺 "�ú����貥�ź�������1.998���ϰ汾" "������ʽ��������"
// ��ע����mcSetMessageCallBack()�������趨��Ļص��� onMcMessage�ص��󲿷ַ����ڵ����ߵ����߳��С�
// ����: 
//      msgid ��Ϣ����:  ����ִ��ʧ��ʱ���󲿷ַ���MCMSG_FAIL
// ����: 
//-----------------------------------
typedef void (__stdcall *TOnMcMessage)(int msgid,const wchar_t* mess);

//------------- DLL -----------------//
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 

//---------------------------------------------//
//��������
//---------------------------------------------//
//
//--------------------------------------
// ����: mcInit
// ����: ��������ʱ��ʼ��McControl
// ����: SvrPort	��ǰSVR����ļ����˿ں�
// ����: _onevent	�ص����������к������ӻ�Ͽ��¼�ʱ��֪ͨ�ϲ����
// ����: _debugmode	����ģʽ��Ĭ��0
// ����: bool
//-------------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcInit(int SvrPort,TOnMcEvent _onevent,int _debugmode=0);

//-----------------------------------
// ����: mcClose
// ����: �����˳�ʱ�ر�McControl����
// ����: ��
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcClose();


//-----------------------------------
// ����: mcSetMessageCallBack
// ����: �����������֪ͨ��Ϣ������
// ��ע������McControl������һЩ��Ҫ����Ϣ������԰��յ���message��ӡ����־�����У��Ա���ԣ��鿴�쳣����ԭ��
// ����: 
//       TOnMcMessage _onmess	�ص��������μ�����TOnMcMessage˵����
// ����: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcSetMessageCallBack(TOnMcMessage _onmess);

//-----------------------------------
// ����: mcSdkVersion
// ����: ��ȡ��ǰ�ӿڰ汾��
// ��ע��
// ����: 
// ����: �汾��
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcSdkVersion();



//---------------------------------------------//
//����״��
//---------------------------------------------//


//-----------------------------------
// ����: mcGetBoxCount
// ����: �õ���ǰ�����߲��źи���
// ����: ���ߵĺ�����
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcGetBoxCount();

//-----------------------------------
// ����: mcGetFirstBox
// ����: ���ص�һ�����ź�
// ��ע�����ڱ������к���
// ����: ������ʱ��ţ������ض��ĺ��ӣ��Ժ��ӽ��к�̲���ʱҪ�õ�����û�У�����0
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcGetFirstBox();
//-----------------------------------
// ����: mcGetNextBox
// ����: �����һ�����Ӵ���
// ��ע�����ڱ������к���
// ����: curbox	��һ�����Ӵ���
// ����: _DLLFUNC_MCCONTROL HBOX		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcGetNextBox(HBOX curbox);
//
//-----------------------------------
// ����: mcFindBox
// ����: ���ݺ������֣����Һ�����ʱ����
// ��ע��
// ����: _szBoxName	�������ƣ�����������LedName,�ɳ�ʼ���ø�����
// ����: ���Ҳ���������0
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcFindBox(const wchar_t* _szBoxName);

//---------------------------------------------//
//���õ�ǰҪ�����ĺ���, ����
_DLLFUNC_MCCONTROL bool	__stdcall mcSetCurrentBox(HBOX boxno);



//���ӻ�����Ϣ
struct TMcBoxInfo 
{
	//�û��Զ�������(�ֽ�LedName��BoxName);
	wchar_t	Name[MAXLEN_BOXNAME];
	//��ǰip��ַ
	wchar_t	ip[30];
	//LED��С
	int		width,height;
	//LED����ʣ��ռ�(M)
	int		diskspace;
	//��ǰ��Ŀ��
	wchar_t mcv[MAXLEN_MCVNAME];
	//���Ӱ汾��
	int		ver;
};


//-----------------------------------
// ����: mcGetBoxInfo
// ����: �鿴���ӵĻ�����Ϣ(����,��Ļ��С,����״̬����ǰ��Ŀ��)
// ��ע��
// ����: boxno	���ӱ��
//		 info	���ز��������غ�����Ϣ
// ����: �Ƿ��ҵ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcGetBoxInfo(HBOX boxno,TMcBoxInfo* info);

//---------------------------------------------//
//	��̬������Ŀ
//---------------------------------------------//
typedef void*	HMCV;		//��Ŀ���

//-----------------------------------
// ����: mcCreateMcv
// ����: �½�һ����Ŀ
// ��ע�������������FTP������������ֱ�Ӱ�FTP�ļ�����Ϊ��Ŀ�ĸ�Ŀ¼���Ͳ���ר���ϴ�FTP��
// ����: 
//       _szMcvBasePath		ר�����ڱ����Ŀ�ı����ļ��У����н�Ŀ�����浽�˴�����E:\\McData
//       _szMcvName			��Ŀ����40�ַ����ڣ��ɺ��֣� ����ʱ�������Ŀ���½�һ���ļ��У��������н�Ŀ���ݡ�
//       screenwidth		��Ŀ���Ŵ��ڴ�С��һ��Ҫ����Ļ��С��ͬ
//       screenheight		
//       _IsDeleteOld		������������Ŀ���Ƿ�ǿ��ɾ����ǰ��Ŀ��Ĭ��ɾ��
//       _IsCopySrcFile		�Ƿ�ѽ�Ŀ�õ���ͼƬ����Ƶ���ļ����Ƶ���Ŀ�ļ����¼��������������ֶ��ϴ�������FTP������
// ����: HMCV	��Ŀ����������½��Ľ�Ŀ	ʧ�ܷ��� 0
//-----------------------------------
_DLLFUNC_MCCONTROL HMCV	__stdcall mcCreateMcv(const wchar_t* _szMcvBasePath,const wchar_t* _szMcvName,int screenwidth,int screenheight,bool _IsDeleteOld=true,bool _IsCopySrcFile=true);

//-----------------------------------
// ����: mcLoadMcv
// ����: ��һ����Ŀ���Ա����������
// ��ע��
// ����: 
//       _szMcvPath	��Ŀ�����ļ��У���E:\\McData\\�½�Ŀ1
// ����: HMCV	��Ŀ��� ʧ�ܷ��� 0
//-----------------------------------
_DLLFUNC_MCCONTROL HMCV	__stdcall mcLoadMcv(const wchar_t* _szMcvPath);

//-----------------------------------
// ����: mcSaveMcv
// ����: �����Ŀ
// ��ע�����ͻ��ϴ�֮ǰ��Ҫ����
// ����:  hmcv	��Ŀ���
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSaveMcv(HMCV hmcv);

//-----------------------------------
// ����: mcSetCurMcv
// ����: ���õ�ǰ��Ҫ�༭�Ľ�Ŀ���Ա��������ͼƬ
// ��ע��һ����hmcv = mcCreateMcv(),Ȼ��mcSetCurMcv(hmcv)
// ����: hmcv ָ����Ŀ����� 
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSetCurMcv(HMCV hmcv);

//-----------------------------------
// ����: mcCloseMcv
// ����: �رս�Ŀ���ͷ��ڴ�
// ��ע�������½���Ŀ��򿪵Ľ�Ŀ��Ҫclose
// ����: 
//       HMCV hmcv
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcCloseMcv(HMCV hmcv);


//---------------------------------------------//
//ҳ�沥�ŷ�ʽ��
enum TMcPlayMode {
	//0-�ȴ���Ŀ���꣬������
	pm_Normal,
	//1-ָ��ʱ��������ָ��ʱ����л�����һҳ
	pm_TimeLen,
	//2-ȫ�ֽ�Ŀ 
	pm_Global,
	//3-��ʱ����: ����ָ��ʱ���Զ�����
	pm_Popup,
};


//-----------------------------------
// ����: mcAddPage
// ����: ���ӽ�Ŀҳ
// ��ע��
// ����: 
//       _szName	��Ŀҳ���֣����Ŀҳ1����ΪNULL�����Զ�����
//       playmode	ҳ�沥�ŷ�ʽ: 0-����ҳ 1-��ʱҳ 2-ȫ��ҳ 3-��ʱ����ҳ
//       playtimes	playmode=0ʱ�����Ŵ�����playmode=1ʱ������ʱ�䣨�룩
//       backcolor	ҳ�汳��ɫ(RGB)��Ĭ��0(��ɫ)
// ����: ��Ŀҳid (pageid)�����༭����ʹ�ã���ʧ�ܷ���0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcAddPage(const wchar_t* _szName,TMcPlayMode playmode,int playtimes,int backcolor=0);

enum  TMcDateType{
	dt_Null		=0,	//��
	dt_Everyday	=1,	//ÿ��
	dt_Week15	=2,	//ÿ��һ������
	dt_Week6	=3,	//����
	dt_Week7	=4,	//����
	dt_Week1	=5,	//��һ
	dt_Week2	=6,	//�ܶ�
	dt_Week3	=7,	//����
	dt_Week4	=8,	//����
	dt_Week5	=9,	//����
	dt_Date		=10,//ָ������
	dt_DateRange=11,//��ֹ����
};

//���壺ʱ���TMcTimeItem 
struct TMcTimeItem
{
	TMcDateType		datetype;		//��ʱ����
	wchar_t			begindate[20];	//����=10��ʱ��ָ������,��2015-07-03
	wchar_t			enddate[20];	 //����=11ʱ��ָ����ֹ������2015-07-10
	wchar_t			begintime[20];	//ָ����ʼ����ʱ��,��19:00:00
	wchar_t			endtime[20];	 //ָ����ֹ����ʱ�䣬��19:30:00
	int				tag;			 //��ǣ�����
};

//(ʱ��εľ������ӿ���MC100����Ķ�ʱ��Ŀҳ�в鿴)
//���ӣ�ÿ������19:00~19:30  dateype=7,begintime=��19:00:00�� endtime=��19:30:00��

//-----------------------------------
// ����: mcAddPageTimer
// ����: ����Ŀҳ���Ӷ�ʱ
// ��ע������Ҫ�ڴ�����Ŀҳʱ����ҳ������ָ��Ϊ3(��ʱҳ) һ����Ŀҳ�������Ӷ��ʱ���
// ����: 
//       int pageid				ָ����Ŀҳ
//       TMcTimeItem * time		ָ��һ��ʱ��Σ��������ʱ��Σ����Զ�������Ŀ��
// ����: ʱ������
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcAddPageTimer(int pageid,const TMcTimeItem* time);

//���壺�������ֶ�����ʽ
enum TMcTxtAniMode {
	ani_TxtNull=0,	//��ֹ
	ani_TxtMoveLeft=1,//��������
	ani_TxtXiangXiaHuaRu=2,//���ϻ���
};

//�������ֿ�����
struct TMcText 
{
	//���֣���Ϊ�գ�����ϵͳ�Զ�Ĭ�����֣�
	wchar_t		name[40];
	//���λ��
	int			left,top,width,height;
	//�߿��ȣ�Ĭ��0
	int			borderwidth;
	//�߿���ɫ��Ĭ�ϰ�
	int			bordercolor;
	//���壨��Ϊ�գ������ú��ӵ�Ĭ�Ϻ��壩
	wchar_t		fontname[20];
	//�ֺţ�Ĭ��20)
	int			fontsize;
	//��ɫ
	int			fontcolor;
	//����ɫ
	int			backcolor;
	//͸����(0-͸�� 255-��͸��)
	int			alpha;
	//����Ч�� Ĭ��0-��ֹ
	TMcTxtAniMode	animode;
	//�ƶ��ٶȵȼ�(0-5)��Ĭ��2
	int			speedLevel;
	//ÿ����Ϣ��ʾʱ��(��)  (����ˮƽ�ƶ�������Ч���ƶ�ʱ�����������ٶȾ���)
	int			playtime;
	//���뷽ʽ(X) 0-�� 1-�� 2-��
	int			alignx;
	//����ģʽ��Ĭ��1�����Ƕ�̬���֣�����0
	int			drawmode;
};


//-----------------------------------
// ����: mcGetDefaultText
// ����: ���һ��Ĭ���ı����ã����ֺ�Ĭ��20
// ��ע��Ϊ�����ı����ù��������������ñ��������һ��Ĭ�����ã�Ȼ���޸����м��
// ����: TMcText * text		����Ĭ������
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultText(TMcText* text);

//
//-----------------------------------
// ����: mcAddSingleText
// ����: ��ӵ������ֿ�
// ��ע��
// ����: 
//       pageid		ָ����Ŀҳ(���ڽ�Ŀҳ��id)
//       _szTxt		�������� (����ж�����Ϣ��ͬһ�����в��������ûس���\n�ָ�)
//       _len		���ָ���(��Ϊ-1�����ʾȫ������)
//       text		���ֿ����ã���TMcText˵��
// ����: ���ض���id (objid)����ʧ�ܷ���0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddSingleText(int pageid,const wchar_t* _szTxt,int _len,const TMcText* text);


//���壺ͼƬ��������
enum TMcAniMode {
	ani_Null=0,				//��ֹ	    
	ani_Rand=1,				//���	    
	ani_DanRuDanChu=2,		//���뵭��  
	ani_XiangZuoHuaRu=3,	//������  
	ani_XiangYouHuaRu=4,	//���һ���  
	ani_XiangShangHuaRu=5,	//���ϻ���  
	ani_XiangXiaHuaRu=6,	//���»���  
	ani_SiJiaoHuaRu=7,		//�Ľǻ���  
	ani_JianJianFangDa=8,	//�����Ŵ�  
	ani_Radar=9,			//�״�      
	ani_SuiHuaRongRu=10,	//�黨����  
	ani_LingXingJieKai=11,	//���νҿ�  
	ani_LingXingHeLong=12,	//���κ�£  
	ani_BaiYe=13,			//��Ҷ      
	ani_JaoCuo=14,			//����      
	ani_YouXuanJinRu=15,	//��������  
	ani_ZuoXuanJinRu=16,	//��������  
	ani_XiangShangYiDong=17,//�����ƶ�  
	ani_XiangZuoYiDong=18,	//�����ƶ�  
};

//ͼƬ������
struct TMcImage
{
	//���֣���Ϊ�գ�����ϵͳ�Զ�Ĭ�����֣�
	wchar_t		name[40];
	//���λ��
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
	//���ŷ�ʽ: 0-���� 1-����(Ĭ��) 2-ԭʼ 
	int			scaleType;
	//����Ч��
	TMcAniMode		animode;
	//�ƶ��ٶȵȼ�(0-5)
	int			speedLevel;
	//ÿ��ͼƬ��ʾʱ��(��)
	int			playtime;
	//����ɫ
	int			backcolor;
	//͸����(0-͸�� 255-��͸��)
	int			alpha;
};

//-----------------------------------
// ����: mcGetDefaultImg
// ����: ���һ��Ĭ��ͼƬ�����ã������������
// ��ע��
// ����: TMcImage * imgbox	����Ĭ������
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultImg(TMcImage* imgbox);

//-----------------------------------
// ����: mcAddImageBox
// ����: ���ͼƬ��,����objid
// ��ע������mcAddImageFile���ͼƬ��������Ӷ���ͼ��
// ����: 
//       int page		��Ŀҳid
//       imgbox			ͼƬ������(λ�ã�������ʽ��)
// ����: ͼƬ��id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddImageBox(int page,const TMcImage* imgbox);
//

//-----------------------------------
// ����: mcAddImageFile
// ����: ��ͼƬ�������ͼƬ(ÿ��ͼƬ�����������������ʾ)
// ��ע���ش�ͼƬ(4000����)��������ת������Ļ���ʺϵ�СͼƬ������Ӱ�첥��
// ����: 
//       int objid		ͼƬ��id
//       _szImgFile		ͼƬ�ļ�·��
// ����: ��š� ��ʧ�ܷ���0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddImageFile(int objid,const wchar_t* _szImgFile);


//��Ƶ������
struct TMcVideo
{
	//���֣���Ϊ�գ�����ϵͳ�Զ�Ĭ�����֣�
	wchar_t		name[40];
	//���λ��
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
};


//-----------------------------------
// ����: mcGetDefaultVideo
// ����: ���һ����Ƶ��Ĭ�����ã������������
// ��ע��
// ����: videobox	����Ĭ������
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultVideo(TMcVideo* videobox);

//-----------------------------------
// ����: mcAddVideoBox
// ����: �����Ƶ��
// ��ע��������mcAddVideoFile�����Ƶ��������Ӷ����
// ����: 
//       int pageid	��Ŀҳid
//       const TMcVideo * videobox		��Ƶ������
// ����: ��Ƶ�� id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddVideoBox(int pageid,const TMcVideo* videobox);

//-----------------------------------
// ����: mcAddVideoFile
// ����: ����Ƶ���������Ƶ(ÿ����Ƶ�����������������ʾ��
// ��ע���������mp4��ʽ�����Ž��ȶ���ע�⣡��Щ�߼���ʽ��mp4(��High@L3)������ͬʱ���������������ת�루��mc100����й��߲˵���
// ����: 
//       int objid		��Ƶ��id
//       _szVideoFile	��Ƶ�ļ� (֧��mp4,avi,mpg,mkv,flv,rmvb,vob�ȶ���)
//       int volume
// ����: ��š� ��ʧ�ܷ���0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddVideoFile(int objid,const wchar_t* _szVideoFile,int volume=100);



//��������
struct TMcNotice
{
	//���֣���Ϊ�գ�����ϵͳ�Զ�Ĭ�����֣�
	wchar_t		name[40];
	//���λ��
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
};

//-----------------------------------
// ����: mcGetDefaultNotice
// ����: ���һ������Ĭ�����ã������������
// ��ע��
// ����: noticebox	����Ĭ������
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultNotice(TMcNotice* noticebox);

//-----------------------------------
// ����: mcAddNoticeBox
// ����: ��ӹ������Բ��Ŷ����ı���RTF�ı�
// ��ע��������mcAddNoticeRTF()�Ⱥ���׷������
// ����: 
//       int pageid		��Ŀҳid
//       * notice		��������
// ����: ����id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddNoticeBox(int pageid,const TMcNotice* notice);

//-----------------------------------
// ����: mcAddNoticeRTF
// ����: Ϊ�������һ��rtf�ı���txt���ı�
// ��ע��
// ����: 
//       objid			����id
//       _szRTFFile		rtf,txt�ļ���
//       _animode		������ʽ��Ĭ��17=�����ƶ����Ƚ��ʺϳ��ı����������Ͳμ�TMcAniMode˵��
//       backcolor		����ɫ��Ĭ�Ϻ�
//       backalpha		����͸����(0~255)��0-ȫ͸��
//       fontcolor		��ɫ��Ĭ�ϰ�
//       speedLevel		�ƶ��ٶ�(0~4)��Ĭ��2
//       pagetime		����Ƿ�ҳ��ʾ�����ֵ��ʾÿҳ��ʾʱ�䣨�룩
// ����: ��š���ʧ�ܷ���0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddNoticeRTF(int objid,const wchar_t* _szRTFFile,TMcAniMode _animode=ani_XiangShangYiDong,int backcolor=0,int backalpha=255,int fontcolor=0xFFFFFF,int speedLevel=2,int pagetime=10);


//-----------------------------------
// ����: mcTxt2Rtf
// ����: Э���������ı�txt�ļ�����txt���������ֺ�ת����rtf��ʽ��Ȼ���������ٰ����rtf��mcAddNoticeRTF()����Ŀ��
// ʾ����bool isok = mcTxt2Rtf(L"D:\\�ĵ�ʾ��\\critical.txt",L"D:\\�ĵ�ʾ��\\critical.rtf",32,L"΢���ź�",true,48,L"����");
// ����: 
//       szTxtFile	ԭʼ���ı�txt���ļ�·��
//       szRtfFile  ������rtf�ı����ļ���
//       fontsize	�����ֺ�
//       fontname   ��������
//       isTitleCenter  �Ƿ�����Զ�����
//       titleFontSize	������ֺţ���Ϊ0������������һ�¡�
//       titleFontName  ��������壬��ΪNULL������������һ�¡�
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL	bool	__stdcall	mcTxt2Rtf(const wchar_t* szTxtFile, const wchar_t* szRtfFile,int fontsize,const wchar_t* fontname,bool isTitleCenter=false,int titleFontSize=0,const wchar_t* titleFontName=0);

//���ʱ����Ϣ��
//fmt���������ͣ�
enum TMcDateFmt
{
	mcdate_YMD,	//2013��11��28��
	mcdate_MD,	//11��28��
	mcdate_W,	//������
	mcdate_MDW,	//11��28�� ������
	mcdate_hm,	//07:46
	mcdate_hms,	//07:46:58
	mcdate_time,	//2013��11��28�� 07:46:58

};
//��Ϣ������
struct TMcInfo
{
	//���֣���Ϊ�գ�����ϵͳ�Զ�Ĭ�����֣�
	wchar_t		name[40];
	//���λ��
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
	//�ֺ�
	int			fontsize;
	//��ɫ
	int			fontcolor;
	//����ɫ
	int			backcolor;
	//͸����(0-͸�� 255-��͸��)
	int			alpha;
};

//-----------------------------------
// ����: mcGetDefaultInfo
// ����: ���һ����Ϣ��Ĭ�����ã������������
// ��ע��
// ����: info	����Ĭ������
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultInfo(TMcInfo* info);

//-----------------------------------
// ����: mcAddInfoBoxTime
// ����: ���һ��ʱ���
// ��ע��
// ����: 
//       int pageid		��Ŀҳ
//       TMcDateFmt fmt	ʱ���ʽ����
//       TMcInfo * info	
// ����: ʱ����Ϣ��id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddInfoBoxTime(int pageid,TMcDateFmt fmt,TMcInfo* info);



//-----------------------------------
// ����: mcStartSendMcv
// ����: ��ʼ�ϴ�ָ����Ŀ
// ��ע�����������ʼ�����ϴ��󷵻ء������ϴ�������Ҫһ��ʱ�䡣����mcGetUploadProgress()��ʱ��ѯ�ϴ����ȡ�
// ����: 
//       boxno		���Ӵ��� (��mcGetFirstBox,mcGetNextBox��ȡ�������ߺ��ӵĴ���)
//      _szMcvPath	��Ŀ�����ļ��У���E:\\McData\\�½�Ŀ1
//		 uploadMode 0-Ĭ�Ϸ�ʽ(ֻ�ϴ���С�б仯���ļ�) 1-ǿ���ϴ������ļ�
// ����: �Ƿ�ʼ
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStartSendMcv(HBOX boxno,const wchar_t* _szMcvPath,int uploadMode=0);

//-----------------------------------
// ����: mcGetUploadProgress
// ����: ��ú��ӵ�ǰ�ϴ����ܽ���
// ��ע��
// ����: 
//       HBOX boxno	���Ӵ���
// ����: ����ֵ(0-100)
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetUploadProgress(HBOX boxno);
//ֹͣ��ǰ�ϴ�
//-----------------------------------
// ����: mcStopUpload
// ����: ֹͣ�ϴ�
// ��ע��
// ����: 
//       HBOX boxno
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStopUpload(HBOX boxno);



//FTP��������
struct TMcFTP
{
	//ftp svr IP���ַ(��Ϊ�գ�����Ĭ������)
	wchar_t		svrip[40];
	//ftp�˿�
	int			port;
	//�û���
	wchar_t		user[20];
	//����
	wchar_t		pwd[20];
	//�ַ���(GBK��UTF-8)
	wchar_t		charset[10];

	TMcFTP()
	{
		svrip[0]=0;
		port=0;
		user[0]=0;
		pwd[0]=0;
		charset[0]=0;
	};
};


//
//-----------------------------------
// ����: mcStartFTP
// ����: ��ʼFTP����
// ��ע�����ú�ܿ췵�ء�Ȼ��Ҫ��mcGetProgressFTP()��ȡ����
// ����: 
//       HBOX boxno		ָ�����ź�
//       _szFtpPath		ָ��FTP�������ϵĴ������ļ��У�ע��Ҫ�������FTP RootĿ¼��·������ "/�½�Ŀ1"  "/doc/test1" 
//       _szDestPath	ָ�������ں������ĸ��ļ��С���Ϊ0����ʾ��Ŀר�õ��ļ���
//       int tag		���ñ�� Ĭ��tag=1ʱ��ʾ������ϵ��Զ���ʼ����
//       TMcFTP * set	FTP����
// ����: �Ƿ�ʼ
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStartFTP(HBOX boxno,const wchar_t* _szFtpPath,const wchar_t* _szDestPath=0,int tag=1,TMcFTP* set=0);

//-----------------------------------
// ����: mcGetProgressFTP
// ����: ��ò��ź�FTP���ؽ��ȣ���������3�����һ�Σ���������5~10�����ϵ���һ��
// ��ע��
// ����: 
//       HBOX boxno		ָ������
// ����: _DLLFUNC_MCCONTROL int	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetProgressFTP(HBOX boxno);
//
//-----------------------------------
// ����: mcGetFTPState
// ����: ��ò��ź�FTP����״̬
// ��ע��
// ����: 
//       HBOX boxno
// ����: ��ǰ״̬0-�� 1-������ 2-�������� 3-������ 4-�����쳣/�޷���¼  5-ֹͣȡ�� 6-������  7-Ŀ¼���� 
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetFTPState(HBOX boxno);


//-----------------------------------
// ����: mcStopFTP
// ����: ֹͣ��ǰFTP����
// ��ע�����ź�һ����ʼ���أ���ʹ����Ҳ���Զ��ϵ����������������������ȡ����������
// ����: 
//       HBOX boxno
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStopFTP(HBOX boxno);

//---------------------------------------------//
//��Ŀά��
//---------------------------------------------//

//-----------------------------------
// ����: mcSetDefaultMcv
// ����: ָ��Ĭ�ϲ��Ž�Ŀ
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       _szMcvName		ָ����Ŀ���������Ѿ��ϴ�����
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSetDefaultMcv(HBOX boxno,const wchar_t* _szMcvName);


//-----------------------------------
// ����: mcGetDefaultMcv
// ����: ��ò��źе�ǰ���ڲ��ŵĽ�Ŀ
// ��ע��
// ����: 
//       HBOX boxno	ָ�����ź�
//       _szMcvName	���ؽ�Ŀ��
//       _MaxNameLen ��Ŀ������󳤶�
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcGetDefaultMcv(HBOX boxno,wchar_t* _szMcvName,int _MaxNameLen=50);


//-----------------------------------
// ����: mcDeleteMcv
// ����: ɾ�������еĽ�Ŀ��������̿ռ�
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       _szMcvName		ָ����Ŀ
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcDeleteMcv(HBOX boxno,const wchar_t* _szMcvName);

//
//-----------------------------------
// ����: mcSendSingleText
// ����: ��̬�������֣�����ʵʱ�滻��ǰ��Ŀ�е���������
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       int objid		ָ�����ֿ�id������������id����Ϊ0����������_szObjNameָ�����ơ�
//       _szObjName		ָ�����ֿ����֡�����Ѿ�ָ��id, �����Ϊ0
//       _szTxt			ָ���������� (���ж�����Ϣ�ûس���\n�ָ�)
//		_mode			0-Ĭ��Ч��  1-�ֽ϶�ʱ�Զ���� 2-������ʾ�������ı���
// ����:
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendSingleText(HBOX boxno,int objid,const wchar_t* _szObjName,const wchar_t* _szTxt,int _mode=0);


//-----------------------------------
// ����: mcSendTextStyle
// ����: ��̬�趨���ַ��
// ��ע��֧��ͬʱ���������� ��Ȼ�������ÿ��ֻ�ı�����һ��������
// ����: 
//       HBOX boxno
//       int objid		ָ�����ֿ�id������������id����Ϊ0����������_szObjNameָ�����ơ�
//       _szObjName		ָ�����ֿ����֡�����Ѿ�ָ��id, �����Ϊ0
//       const wchar_t * _szStyle	����ַ���������ָ��������������磺 fontcolor=#FF00FF00;fontsize=32; ��ɫ��8λ�������λ��alpha����#AARRGGBB
//		Ŀǰ֧�����·���������£�fontcolor=#FF00FF00;fontsize=32;backcolor=#80FF0000;borderwidth=4;bordercolor=#FFFF8000;animode=2;
//		animode ��TMcTxtAniMode��ֵ�� ֧��0,1,2
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendTextStyle(HBOX boxno,int objid,const wchar_t* _szObjName,const wchar_t* _szStyle);

//-----------------------------------
// ����: mcSendObjStyle
// ����: ��̬���ػ���ʾ�ؼ�
// ��ע������������ڵ�����.��Ŀ�������󣬿���ң������Ŀɼ��ԡ���mcSetObjVisible�����������visible���浽��Ŀ��
// ����: 
//       HBOX boxno
//       int objid		ָ����id������������id����Ϊ0����������_szObjNameָ�����ơ�
//       _szObjName		ָ�������֡�����Ѿ�ָ��id, �����Ϊ0
//       int visible	0-���ɼ� 1-�ɼ�
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendObjVisible(HBOX boxno,int objid,const wchar_t* _szObjName,int visible);


//-----------------------------------
// ����: mcSetObjVisible
// ����: ��mcv��ĳ�������ָ�����ĳ�ʼ�ɼ��ԡ�Ĭ��Add�����ǿɼ��ġ������ָ����0�����Ŀ��ʼ����ʱ�����ǲ��ɼ���
// ��ע���տ�ʼʱ�����Ȱ�һ���ؼ������������Ժ��ٵ���mcSendObjVisible�������ֻ���ʧ
// ����: 
//       int pageid   �ÿ�����ҳ���id
//       int objid    �ÿ��id������Add xxxx ʱ�����������id
//       int visible 0-���ɼ� 1-�ɼ�
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetObjVisible(int pageid,int objid,int visible);

//-----------------------------------
// ����: mcSetLedName
// ����: Ϊ������������/�������Ա����������
// ��ע��
// ����: 
//       HBOX boxno
//       _szLedName		�������źе�����,�����Ǻ��֣���Ҫ����20�֡�
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedName(HBOX boxno,const wchar_t* _szLedName);

//-----------------------------------
// ����: mcSetMcid
// ����: Ϊ������һ���ڲ�id (mcid)
// ��ע��
// ����: 
//       HBOX boxno
//       _mcid ���ֻ�Ӣ�� ,��󳤶�11���ַ�
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetMcid(HBOX boxno,const char* _mcid);


//////////////////////////////////////////////////////////////////////////
//			��ĿԤ������
//
//ע�����
//mcDisplay...ϵ�к�����Ҫ�����߳��е���
//--��Ҫ����mcCloseMcv()�ر���,���򿴲���Ԥ��. mcCloseMcv��رյ�ǰ�༭�Ľ�Ŀ
//--��Ҫ�ڶ�ʱ���е���mcDisplayDraw()��ɻ���ˢ�¡���ʱ���ļ������֡�ʡ�����20ms (50FPS) ��������Ƚ��ã���30ms (30FPS)
//--����ֱ��Ԥ���ڴ���mcv,���������á� ��;һ�㲻��ҪmcSaveMcv();ֻҪ���û��˳��༭ʱmcSaveMcv()����
//
//---------------------------------------------//

/*
ʹ������:

(1)�����ʼ��ʱ
mcInit(8300,OnMcEvent,1);
...
mcDisplayInit(hwnd_screenWin,0x404040);

(2)Ԥ��
mcSetCurMcv(..)
mcDisplayShow();//��ʼԤ����ǰ��Ŀ
TimerDisplay->Enabled=true;//����һ����ʱ����ÿ20ms����һ��

(3)
onTimer()
{
	mcDisplayDraw();//���øú���ˢ��Ԥ������
}

*/

//-----------------------------------
// ����: mcDisplayInit
// ����: ��ʼ��Ԥ������
// ��ע�������������ǹ̶���С��(�����;�����˴�С��Ҫ��resize֮�����µ���mcDisplayInit)
//			Ԥ�����ܶ��Կ���һ��Ҫ���еĻ�����ʾ���ܺܲ�(���и���Atomƽ�����)�����ܻ��ʼ��ʧ��,�޷�Ԥ����
// ����: 
//       HWND outWindow		������ڵ�windows���(hwnd)�����������ⴰ��������������ǰ�ť��ֻҪ��hwnd��
//       int backcolor		������ڵı���ɫ����������ɫ��
//       int mode			���ò�����Ĭ��0
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcDisplayInit(void* outWindow,int backcolor=0x803010,int mode=0);

//-----------------------------------
// ����: mcDisplayShow
// ����: Ԥ����ǰ��Ŀ
// ��ע������mcSetCurMcv()�趨��ǰ���ڱ༭�Ľ�Ŀ�� ���ң�Ԥ��ǰ��Ҫ����mcCloseMcv!����ʲôҲ��������
//		 Ԥ����ʵʱ�ģ�Ԥ��ǰ���ر����Ŀ��
// ����: 
//       int pageid		�����Ŀ�ж�ҳ������ָ��һ����ʼҳ��
//       bool repeate   �Ƿ��ظ����Ÿ�ҳ������Ϊ�û�����Ҫһֱ�༭����һҳ����ϣ���Զ���ҳ��
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayShow(int pageid=0,bool repeate=false);


//-----------------------------------
// ����: mcDisplayDraw
// ����: ˢ��Ԥ������
// ��ע��ˢ�µ�Ƶ�ʾ�����֡�ʡ� ˢ��Խ�죬����Խ������CPUռ���ʻ��һ�㡣�ú���ͨ�����ڶ�ʱ���С�(��Ҫ���⿪�߳�ˢ����Ϊˢ��Ҫ�����߳��н���)
// ����: 
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayDraw();


//-----------------------------------
// ����: mcDisplayStop
// ����: ֹͣԤ����ǰ��Ŀ��
// ��ע��
// ����: 
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayStop();


//-----------------------------------
// ����: mcDisplayResize
// ����: ������ʾ������Ԥ�������С
// ��ע������Ҫ���ⲿ����������������outWindow(Ԥ������)��С��Ȼ���ٵ��øú���֪ͨ�ںˡ�
//		 outWindow->Width=xxx; outWindow->Height=xxx; mcDisplayResize();
// ����: 
// ����: 
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayResize();

//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// ����: mcConnectBox
// ����: ����ӽ�����ʱ���� ���ɵ��������������ӣ��������ӣ�
// ��ע���ú�������ʱ�����������ء� ���ӳɹ��󣬻�֪ͨMCEVENT_CONNECT��Ϣ (Ҫ������mcinit()ʱע��TOnMcEvent�ӿ�)��
//		��ʱ�����������512����̫����ʱ���ӻή���������ܡ�
//		SDKĬ�ϵ������Ƿ�������ʽ��������Ϊ�����������˿ڣ����������������ú��Զ�������
//
// ����: 
//       const char * IP		����IP
//       int port				���ӵĶ˿�(Ĭ��8100)
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL	void	__stdcall mcConnectBox(const char* IP,int port=8100);


//-----------------------------------
// ����: mcDisConnectBox
// ����: �Ͽ�����ӵ���ʱ����
// ��ע��
// ����: 
//       HBOX boxno
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL	void	__stdcall mcDisConnectBox(HBOX boxno);



//-----------------------------------
// ����: mcFindBoxByIP
// ����: ����IP���Һ��ӡ���û�ҵ�������0
// ��ע��
// ����: 
//       const char * IP
// ����: �������߱��
//-----------------------------------
_DLLFUNC_MCCONTROL	HBOX		__stdcall mcFindBoxByIP(const char* IP);


//��ȡ�����Ƿ�����
_DLLFUNC_MCCONTROL	bool		__stdcall mcBoxIsConnect(HBOX boxno);

//-----------------------------------
// ����: mcBoxSetTime
// ����: ���ú���ʱ�� 
// ��ע���ú�����ʹ��������,���������ӡ�
// ����: 
//       HBOX boxno
//       const char * stime		��ǰʱ�䣬��ʽ(19���ַ�)��2016-06-12 12:08:16
// ����: �Ƿ��������
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSetTime(HBOX boxno,const char* stime);


 
//-----------------------------------
// ����: mcBoxSetAutoConnect
// ����: ���ú����Զ�����
// ��ע���������Զ��������������Ϊֹͣ����������Ҫ�ֶ�����IP,��mcConnectBox�������ӡ�
// ����: 
//       HBOX boxno		���Ӵ���
//       const char * svrIP		������IP����ַ ��ΪNULL������ֹ����
//       int port	�������˿�
// ����: �Ƿ��������
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSetAutoConnect(HBOX boxno,const char* svrIP,int port);


//��յ�ǰ�̵������ã�׼����������
_DLLFUNC_MCCONTROL void		__stdcall mcBoxClearRelay(HBOX boxno);

//-----------------------------------
// ����: mcBoxAddRelayTime
// ����: ����һ��̵�����ʱ����(���Զ���). 
// ��ע������ʱ��β��ܻ����ͻ����ʼʱ��Ӧ˳�����
// ����: 
//       int relayNo		�̵�����ţ�֧����·(1��2)
//       int beginHour		��ʼʱ��_Сʱ����
//       int beginMin		��ʼʱ��_���Ӳ���
//       int onoff			����(0-�أ�1-��)
// ����: _DLLFUNC_MCCONTROL void		__stdcall
// ʾ���� ����7�㿪������22���
//		mcBoxAddRelayTime(1,7,0,1);
//		mcBoxAddRelayTime(1,22,0,0);
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxAddRelayTime(HBOX boxno,int relayNo,int beginHour,int beginMin,int onoff);


//-----------------------------------
// ����: mcBoxSendRelayTime
// ����: ���ͼ̵�����ʱ����
// ��ע���ڸ��ʱmcBoxAddRelayTime֮�󣬵��øú����ϴ�����
// ����: 
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSendRelayTime(HBOX boxno);

//ȡ���̵�����ʱ����
_DLLFUNC_MCCONTROL void		__stdcall mcBoxStopRelayTime(HBOX boxno);

//-----------------------------------
// ����: mcBoxReboot
// ����: �������ź�
// ��ע����������Ҫ��������
// ����: 
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxReboot(HBOX boxno);


//-----------------------------------
// ����: mcBoxSetRebootTime
// ����: ����ÿ�춨ʱ����
// ��ע��
// ����: 
//       int hour	����ʱ�䣬���㼸��
//       int min
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxSetRebootTime(HBOX boxno,int hour,int min);


//-----------------------------------
// ����: mcBoxGetMcvList
// ����: ��ú����ϵ�ǰ��Ŀ�嵥
// ��ע��
// ����: 
//       HBOX boxno
//       int timeout	����ʱ��ms, ���ڴӺ����϶��ؽ�Ŀ�嵥��Ҫһ����ʱ�䣬������Ҫһ����ʱ�������̻߳�������
// ����: ��ȡ�Ľ�Ŀ������ -1:��ȡʧ��(��ʱ) 0-û�н�Ŀ >=1��Ŀ��
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcBoxGetMcvList(HBOX boxno,int timeout=1500);

struct TMcProgramInfo {
	//��Ŀ��(��Ŀ�ļ�������)
	wchar_t name[200];
	//�ϴ�ʱ�䣬��: 2016-07-12 12:01:32
	wchar_t	date[40];
	//��С(�ֽ�)
	int		size;
	//��Ŀ��ע��Ϣ
	wchar_t rem[100];
};

//-----------------------------------
// ����: mcBoxGetMcvInfo
// ����: ��ȡ��Ŀ��Ϣ
// ��ע��Ŀǰֻ�ܴӺ��Ӷ��ؽ�Ŀ������Ϣ��Ȼ���������ʾ��Ŀ�嵥���л���ǰ��Ŀ��
//		 �޷��Ӻ������ؽ�Ŀ���ݡ� ���Ժ����ϵĽ�ĿҪ�ڱ����ϱ��ݣ�����û�취�޸ġ�
// ����: 
//       HBOX boxno		���Ӵ���
//       int index		��Ŀ��š� (index>=0 ����index < ��Ŀ����) (��������mcBoxGetMcvList()��ȡ��Ŀ����)
//       TMcProgramInfo * tf
// ����: �Ƿ��ã� ���index�Ƿ�����false
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxGetMcvInfo(HBOX boxno,int index,TMcProgramInfo* tf);


//////////////////////////////////////////////////////////////////////////
//
//	����һϵ�� mcMcv.... ��������������/�޸���ǰ��Ŀ���ݡ�
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// ����: mcMcvGetPageCount
// ����: ���ص�ǰ���ڱ༭�Ľ�Ŀ�Ľ�Ŀҳ����
// ��ע������mcCreateMcv()��mcLoadMcv()������mcSetCurMcv()
// ����: 
// ����: _DLLFUNC_MCCONTROL int	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetPageCount();

//-----------------------------------
// ����: mcMcvGetPageId
// ����: ����ָ����ŵĽ�Ŀҳ��id,�Ա�ʹ��mcAddPageTimer,mcDelPage�Ⱥ���
// ��ע��
// ����: 
//       int index
// ����: pageid (>=1), ���Ҳ�������0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetPageId(int index);

//ɾ��ָ��ҳ
_DLLFUNC_MCCONTROL bool	__stdcall mcMcvDelPage(int pageid);

//ָ����Ŀҳ������������Ľ�Ŀ������� 
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetObjCount(int pageid);


//-----------------------------------
// ����: mcMcvGetObjId
// ����: ����ĳҳ��ָ����Ŷ����id
// ��ע��
// ����: 
//       int pageid
//       int index	��ţ�0 ~ page.objcount-1
//
// ����: objid (>=1) ���Ҳ���������0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetObjId(int pageid,int index);

enum TMcObjType {
	mcobj_Null,		//δ֪
	mcobj_Page,		//��Ŀҳ
	mcobj_TextBox,	//���ֿ�
	mcobj_ImageBox,	//ͼ���
	mcobj_VideoBox,	//��Ƶ��
	mcobj_NoticeBox,//����(ͼ�Ŀ�)
	mcobj_InfoBox,	//��Ϣ��
	mcobj_SoundBox,	//���ֿ�
	mcobj_ClockBox,	//ģ��ʱ��
};

struct TMcObjInfo 
{
	int			id;		//����id
	TMcObjType	type;	//����
	wchar_t		name[40];//����
	int			left,top,width,height;//��С
};


//-----------------------------------
// ����: mcMcvGetObjInfo
// ����: ���ָ���������Ϣ
// ��ע��
// ����: 
//       int objid		�����id����mcMcvGetObjId()��ã�������mcAdd....ϵ�к������ص�id
//       TMcObjInfo * tf
// ����: _DLLFUNC_MCCONTROL bool	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcMcvGetObjInfo(int objid,TMcObjInfo* tf);

//ɾ��ָ������
_DLLFUNC_MCCONTROL void	__stdcall mcMcvDelObj(int objid);

//-----------------------------------
// ����: mcGetUploadFile
// ����: ��ú��ӵ�ǰ��Ҫ�ϴ����ļ����
// ��ע�������ڱ����ϴ�����FTP���ط�ʽ��Ч
// ����: 
//       HBOX boxno	���Ӵ���
// ����: �Ƿ��ϴ�״̬:0-��  3-�����ϴ� 4-�ϴ����� 5-�쳣/ȡ��
//-----------------------------------
struct TMcUploadFileInfo
{
	//�����ϴ����ļ���
	wchar_t szFile[300];
	//�����ϴ����ļ����
	int		index;
	//�˴�Ҫ�ϴ����ļ�����
	int		count;
};
_DLLFUNC_MCCONTROL int 	__stdcall mcGetUploadFileInfo(HBOX boxno,TMcUploadFileInfo* info);



//-----------------------------------
// ����: mcSetLedIP
// ����: ���ô������þ�̬IP
// ��ע������IP��������ǰLED�ж����ӣ��������ӡ�
// ����: 
//       HBOX boxno
//       const char * _szIP		 ָ��һ���̶�IP 
//       const char * _gateWay	 ���ء���ΪNULL������xxx.xxx.xxx.1
//       const char * _mask		 ���롣��ΪNULL������255.255.255.0
//       const char * _dns1		 Ϊ���ܹ�˳����������ʱ��Ҫָ��һ��dns����
//       const char * _dns2
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedIP(HBOX boxno,const char* _szIP,const char* _gateWay=0,const char* _mask=0,const char* _dns1=0,const char* _dns2=0);


//-----------------------------------
// ����: mcSetLedDhcp
// ����: ���ô���dhcp
// ��ע�����������������ǰLED�ж����ӣ��������ӡ�
// ����: 
//       HBOX boxno		
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedDhcp(HBOX boxno);

//-----------------------------------
// ����: mcSetLedDhcp
// ����: ���õ�ǰ����
// ��ע��
// ����: 
//       HBOX boxno		
//		 int bright		����0~100 (Ĭ��75) 0:��������������ʱ��Ҫ��ʱ������
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetCurrentBright(HBOX boxno,int bright=75);

_DLLFUNC_MCCONTROL void   __stdcall mcNowDatetime(double* dtime);


// borderImg="border7b.png" borderWidth="8" borderMoveSpeed="12" borderRepeatCount="4" borderBlendmode="0"

//-----------------------------------
// ����: mcSetPageBorder
// ����: ��ҳ���һ������Ʊ߿�
// ��ע��(�貥�ź�����1.99��)
// ����: 
//       int pageid			ҳ��id
//       int borderStyle	�������ʽ: 1-�����ʴ� 2-��ɫˮ���� 3-Բ��	
//       int moveSpeed		�ƶ��ٶȡ�Ĭ��12��Խ��Խ��
//       int repeatCount	����Ƶ��ܶȣ��ظ�����Խ��Խ��)
//		 int aniType		��������: 0-��ֹ 1-˳ʱ�� 2-��ʱ�� 3-�� 4-��˸
//		 int duration		���ڣ�����): ��������Ķ�������Ҫ���ڲ���
//		 int borderWidth	�߿���ʾ��ȣ����ص���)
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetPageBorder(int pageid,int borderStyle,int moveSpeed=12,int repeatCount=4,int aniType=1,int duration=1500,int borderWidth=4);

// ����: ��ҳ���һ������Ʊ߿��Զ�����ʽ
// ��ע��(�貥�ź�����1.99��)
// ����: 
//       int pageid			ҳ��id
//       wchar_t* _szBorderImgFile	����Ʊ���ͼƬ(����png��ʽ������alphaͨ��)	�����С 256x32,��512x32 (һ��ˮƽ�ʴ�״��ͼƬ)
//       int moveSpeed		�ƶ��ٶȡ�Ĭ��12��Խ��Խ��
//       int repeatCount	����Ƶ��ܶȣ��ظ�����Խ��Խ��)
//		 int aniType		��������: 0-��ֹ 1-˳ʱ�� 2-��ʱ�� 3-�� 4-��˸
//		 int duration		���ڣ�����): ��������Ķ�������Ҫ���ڲ���
//		 int borderWidth	�߿���ʾ��ȣ����ص���)
// ����: 
_DLLFUNC_MCCONTROL bool		__stdcall mcSetPageBorderEx(int pageid,const wchar_t* _szBorderImgFile,int moveSpeed=12,int repeatCount=4,int aniType=1,int duration=1500,int borderWidth=4);


//-----------------------------------
// ����: mcCloseScreen
// ����: ���������������
// ��ע����Ŀͣ���� ����ж�ʱ�岥��Ҳ��һֹͬͣ�岥��ע��Ҫ���ʵ��������mcOpenScreen��������Ϊ��Ļ������!
// ����: 
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcCloseScreen(HBOX boxno);


//-----------------------------------
// ����: mcOpenScreen
// ����: ����
// ��ע�����²�����ǰ��Ĭ�Ͻ�Ŀ
// ����: 
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcOpenScreen(HBOX boxno);



//-----------------------------------
// ����: mcBoxSetRelay   
// ����: ���ͼ̵���״̬
// ��ע�����øú����ϴ�����  add by [zz]
// ����: 
//       int relayNo		�̵�����ţ�֧����·(1��2)
//       int onoff			����(0-�أ�1-��)
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxSetRelay(HBOX boxno,int relayNo,int onoff);


//-----------------------------------
// ����: mcGet4GState
// ����: ��ѯ3G/4G�Ĺ���״̬
// ��ע��
// ����: 
// ����: -1��ȡʧ��(���Ժ��ٲ�) -2���Ӱ汾��֧��	0-3G/4Gû�а�װ  1-������ 2-������ 3-���ڹ��� 4-�쳣
//-----------------------------------
_DLLFUNC_MCCONTROL int  __stdcall mcGet4GState(HBOX boxno);


//-----------------------------------
// ����: mcGet4GIMEI
// ����: ��ȡ4Gģ��IMEI
// ��ע��
// ����: 
//       HBOX boxno	 ���Ӻ�
//       char * IMEI	�����ַ���
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool __stdcall mcGet4GIMEI(HBOX boxno,char* IMEI);

//[V1.3]
//
//-----------------------------------
// ����: mcSendTexts
// ����: ��̬���Ͷ�����֣�����ʵʱ�滻��ǰ��Ŀ�е��������ݣ�����ָ��������ֿ�
// ��ע��
// ����: 
//       HBOX boxno			ָ�����ź�
//		mode			0-Ĭ��Ч��  1-�ֽ϶�ʱ�Զ����
//       _szNameAndTexts	���ֿ����Ƽ������ݣ������ж���������|������磺���ֿ�1:ABC|���ֿ�2:HELLO|���ֿ�3:����
// ����: �Ƿ�ɹ�
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendTexts(HBOX boxno,int mode,const wchar_t* _szNameAndTexts);



struct TMessageBox {
	//��ʾʱ��
	int timeout;
	//�Ի���Ĵ�С
	int width;
	int height;
	//����1�������С����ɫ
	int fontsize;
	int fontcolor;
	//����2�������С����ɫ
	int fontsize2;
	int fontcolor2;
	//����ɫ
	int backcolor;
	//����1
	wchar_t title[100];
	//����2
	wchar_t title2[100];
	//����ͼ��Ĭ��Ϊ�գ���Ҫָ��һ��ͼƬ������������ϴ���UserMedia
	wchar_t backimg[100];
};


//-----------------------------------
// ����: mcShowMessage
// ����: �ڴ����ϵ�����ʱ��Ϣ��Ȼ���Զ���ʧ
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       int mode		��ʽ��0-������ʾ  
//       const wchar_t * _szText	Ҫ��ʾ�����֡��������У���\n�ָ�
//       TMessageBox*	msgbox	������Ĳ���
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcShowMessage(HBOX boxno,const TMessageBox* msgbox);


//////////////////////////////////////////////////////////////////////////
//
//		����mcGridϵ�к������ṩ�˶Ա�����ݡ���ͼ��ʾ�Ŀ���
//
//		mcGridData,mcGridClear,mcGridAddRow,mcGridSetCell �Ⱥ������ṩ�˶�̬���ͱ�����ݹ��ܡ��ɶ�̬���ɾ���У��޸�ָ����Ԫ��
//		mcAddGrid mcGridShow��mcGridAutoScroll ��������ʾ����� ��������������Ϊ��Ԫ�� ָ����ҳ��ʽ�ȡ�
//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// ����: mcAddGrid
// ����: �ڽ�Ŀҳ�϶���һ������ö���ı���������У���Ϊһ�������ͼ��������ʾ�������
// ��ע�����һ��ͽ�Ŀҳ�󶨣�������mcv.xml��
// ����: 
//       int pageid		ҳ��id
//       int gridid		ָ����񣬰���ͼ�����
//       int column		������ һ��Ӧ��grid������ͬ
//       const wchar_t * txtBoxNameList  �ı����������У���|�ָ��������������С� ÿ�����ֿ��Ӧһ�����cell
// ����: txt ���� ����Ԫ�����)
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddGrid(int pageid,int gridid,int column, const wchar_t* txtBoxNameList);


//-----------------------------------
// ����: mcGridData
// ����: ���ͱ�����ݵ�������
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       int gridid		grid��ID
//       int column		���� 
//       const wchar_t * txtArray	����и�����������ݡ�������|�ָ������������Զ����С�������ΪNULL���򴴽��ձ�
//							�����ܳ����ó���60000�֡�������������mcGridAddRow������ӡ�
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridData(HBOX boxno,int gridid, int column,const wchar_t* txtArray);


//-----------------------------------
// ����: mcGridClear
// ����: ���ָ�����
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       int gridid		ָ�����
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridClear(HBOX boxno,int gridid);

//-----------------------------------
// ����: mcGridAddRow
// ����: �������ϵı�����һ������
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       int gridid		ָ�����
//       const wchar_t* rowtexts  ��������ݣ�������|�ָ����Ӧ�������������ͬ��
//       int row		������к�λ�ã����ڸ���֮ǰ����(�кŴ�0��ʼ)��Ĭ��-1����ʾ��ĩβ��ӡ� 
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridAddRow(HBOX boxno,int gridid,const wchar_t* rowtexts,int row=-1);


//-----------------------------------
// ����: mcSendFile
// ����: �ϴ�һ���û��Լ����ļ���������
// ��ע��ע�⣡ ������������Ҫһ����ʱ�䣬����mcGetUploadProgress����ѯ����״̬
// ����: 
//       HBOX boxno		ָ�����ź�
//       const wchar_t * _szFile			�ļ��ڱ����ϵ�����·��;
//       const wchar_t * _szDestDir			�����ϵ�Ŀ���ļ��У����ִ�Сд��Ĭ��user
// ����: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendFile(HBOX boxno,const wchar_t* _szFile,const wchar_t* _szDestDir=0);



//�����������������ϴ�����ļ��������У�
//-----------------------------------
// ����: mcClearSendFile
// ����: �����ǰ�ϴ�����
// ��ע��
// ����: 
//       HBOX boxno
// ����: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcClearSendFile(HBOX boxno);


//-----------------------------------
// ����: mcAddSendFile
// ����: ���һ�����ϴ��ı����ļ�.
// ��ע��
// ����: 
//       HBOX boxno
//       const wchar_t * _szFile
// ����: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcAddSendFile(HBOX boxno,const wchar_t* _szFile);

//-----------------------------------
// ����: mcStartSendFile
// ����: ��ʼ�ϴ��û��ļ�
// ��ע�������ڶ�̬ˢ��ͼƬ����������������Ѷ��ͼƬ�ز��ϴ���userĿ¼�¡�Ȼ����mcSetImageBox()����ͼƬ��ĵ�ǰͼƬ���ݡ�
//		ע�⣡ ������������Ҫһ����ʱ�䣬����mcGetUploadProgress����ѯ����״̬
//		���⣬ ͼƬ�ļ�������������СͼƬ��̫���ͼƬ�Ϸ��ڴ�
//		����mcStopUploadֹͣ�ϴ�

// ����: 
//       HBOX boxno
//       const wchar_t * _szDestDir			�����ϵ�Ŀ���ļ��У����ִ�Сд��Ĭ��user
// ����: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcStartSendFile(HBOX boxno,const wchar_t* _szDestDir=0);


//-----------------------------------
// ����: mcDelUserFile
// ����: ɾ����������û��Լ����ļ�
// ��ע��
// ����: 
//       HBOX boxno		ָ�����ź�
//       const wchar_t * _szFile			�ļ�����֧��*ͨ���(*.* ��*.jpg �� abc*.jpg ��abc*)	 ��֧��?ͨ���
//       const wchar_t * _szDestDir			�����ϵ�Ŀ���ļ��У����ִ�Сд��Ĭ��user
// ����: ��ͨ������󣬷���false
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcDelUserFile(HBOX boxno,const wchar_t* _szFile,const wchar_t* _szDestDir=0);


//-----------------------------------
// ����: mcSetImageBox
// ����: ��̬������Ŀ�ϵ�ͼƬ��
// ��ע��Ҳ����ͬʱ��������ͼ�����ǽ�������ʾ��
// ����: 
//       HBOX boxno
//		const wchar_t* _szImageBoxName		ͼƬ������
//       const wchar_t * _szUserImageFile	�����ϵ�ͼƬ�ļ������������ϴ�ͼƬ�زĵ�_szUserDir�¡������Ƕ���ͼƬ���м���|�ָ�
//		 const wchar_t* _szUserDir �����ϵ�ͼƬ�ļ������ļ��С���ΪNULL����Ĭ��Ϊuser�ļ���
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetImageBox(HBOX boxno,const wchar_t* _szImageBoxName, const wchar_t* _szUserImageFile=0,const wchar_t* _szUserDir=0);



#define IMGDATA_RGB		1	//24bit RGB�ڴ���
#define IMGDATA_ARGB	2	//32bit ARGB�ڴ���
#define IMGDATA_JPG		3	//jpg ���ڴ��ļ�
#define IMGDATA_PNG		4	//png ���ڴ��ļ�
#define IMGDATA_BMP		5	//bmp ���ڴ��ļ�
//-----------------------------------
// ����: mcSetImageBoxData
// ����: ʹ��ͼ��RGB���ݣ����ڴ����ݣ���̬������Ŀ�ϵ�ͼƬ��
// ��ע��ͼ��Ҫ̫�󣬺����ڴ����ޡ�������imageboxһ����С��
// ����: 
//       HBOX boxno
//       const wchar_t * _szImageBoxName	ͼƬ������
//       int _ImageIndex	ͼƬ������0��ʾ��һ�š� һ��ͼƬ����ܰ����ж���ͼƬ������ָ������һ�Ž��и�������������� >= img count �����Զ����ͼƬ��ĩβ���������滻��
//       int datatype	ͼ��buf���ݸ�ʽ������IMGDATA_XXX ����ѡһ��
//       void * buf		�ڴ�buf
//       int buflen		����(�ֽ�����
//       int imgw	ͼ��Ĵ�С�� �����RGB���ݣ�����ָ��ͼ���ߡ������jpg,png,bmp�����������0�����ӿ����Զ�������С�� �����Ϊ0���Ͱ���ָ���Ĵ�С�������š�
//       int imgh
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetImageBoxData(HBOX boxno,const wchar_t* _szImageBoxName,int _ImageIndex,int datatype,void* buf,int buflen,int imgw,int imgh);


//-----------------------------------
// ����: mcSetVideoBox
// ����: ��̬������Ŀ�ϵ���Ƶ��
// ��ע��Ҳ����ͬʱ��������ͼ�����ǽ�������ʾ��
// ����: 
//       HBOX boxno
//		const wchar_t* _szVideoBoxName		��Ƶ������
//       const wchar_t * _szUserVideoFile	�����ϵ���Ƶ�ļ������������ϴ���Ƶ�زĵ�_szUserDir�¡������Ƕ�����Ƶ���м���|�ָ�
//		 const wchar_t* _szUserDir �����ϵ���Ƶ�ļ������ļ��С���ΪNULL����Ĭ��Ϊuser�ļ���
// ����: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetVideoBox(HBOX boxno,const wchar_t* _szVideoBoxName, const wchar_t* _szUserVideoFile=0,const wchar_t* _szUserDir=0);



//-----------------------------------
// ����: mcCheckMcv
// ����: ���������Ƿ���ĳ����Ŀ
// ��ע�����������ȴ����ӵķ��أ�������ʱ
// ����: 
//       HBOX boxno
//       const wchar_t * _szMcv	 ��Ŀ��
//		timeOutMs		��ʱ (ms)
// ����: �Ƿ���� 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcCheckMcv(HBOX boxno,const wchar_t* _szMcv,int timeOutMs=1500);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif // McControl_h__
