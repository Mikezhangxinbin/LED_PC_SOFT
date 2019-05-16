//===================================================================
/**\file  
 *  Filename:   McControl.h
 *  Desc:       MC100播放盒 数据访问/控制接口
 *		
 *				相关C示例代码，可阅读TestSdk/main.cpp
 *      
 *  His:      __stdcall mcsoft  create @  2015-7-10   14:44
 */
//===================================================================
#ifndef McControl_h__
#define McControl_h__

#include "McSdkBase.h"

#define MAXLEN_BOXNAME	40
#define MAXLEN_MCVNAME	100

//盒子临时编号（类似于文件句柄，盒子在线时这个值不变。只要服务器不关闭，盒子重新上线下线仍是这个编号）
typedef		int		HBOX;
typedef		long long MCID;


#ifdef MCCONTROL_EXPORTS
#define _DLLFUNC_MCCONTROL  __declspec(dllexport) 
#else
#define _DLLFUNC_MCCONTROL  __declspec(dllimport) 
#endif

#define		MCEVENT_CONNECT			100	//有盒子连接
#define		MCEVENT_DISCONNECT		101	//有盒子断开连接
#define		MCEVENT_ERROR			102	//有连接出错。(boxno=0，表示最后发起的临时连接出错)


//常用颜色 0xAABBGGRR
#define MC_COLOR_RED	0xFF0000FF	//红
#define MC_COLOR_GREEN	0xFF00FF00
#define MC_COLOR_BLUE	0xFFFF0000
#define MC_COLOR_WHITE	0xFFFFFFFF	//白
#define MC_COLOR_BLACK	0xFF000000	//黑
#define MC_COLOR_YELLOW	0xFF00FFFF	//黄
#define MC_COLOR_RED2	0x800000FF	//半透明红
#define MC_COLOR_NULL	0x00000000	//透明

//回调函数：盒子连接事件通知
//-----------------------------------
// 回调函数: OnMcEvent
// 功能: 由McControl.dll回调上层程序，通知盒子的连接与断开等事件
// 备注：这个回调不是在主线程中！！！ 所以在这个函数中刷新主界面通常是不安全的，有时界面会卡死。所以建议在这个函数中，你如果要刷新主界面，要把这个事件再次post message到主线程中。
// 参数: 
//      event，			事件类型，参见MCEVENT_CONNECT等
//		boxno			播放盒代号
//		para1,para2		备用参数
//-----------------------------------
typedef void (__stdcall *TOnMcEvent)(int event,HBOX boxno,int para1,int para2);



#define  MCMSG_COMMON		21
#define  MCMSG_FAIL			22
//回调函数：盒子函数返回执行过程中一些消息通知
//-----------------------------------
// 回调函数: OnMcMessage
// 功能: 用于McControl告诉你一些必要的信息，你可以把_mess打印在日志窗口中，以便调试。有些函数调用失败返回false,会同时在此输出异常消息.如： "该函数需播放盒升级到1.998以上版本" "文字样式参数有误"
// 备注：用mcSetMessageCallBack()函数来设定你的回调。 onMcMessage回调大部分发生在调用者的主线程中。
// 参数: 
//      msgid 消息类型:  函数执行失败时，大部分返回MCMSG_FAIL
// 返回: 
//-----------------------------------
typedef void (__stdcall *TOnMcMessage)(int msgid,const wchar_t* mess);

//------------- DLL -----------------//
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 

//---------------------------------------------//
//基本调用
//---------------------------------------------//
//
//--------------------------------------
// 函数: mcInit
// 功能: 程序启动时初始化McControl
// 参数: SvrPort	当前SVR软件的监听端口号
// 参数: _onevent	回调函数，当有盒子连接或断开事件时，通知上层程序
// 参数: _debugmode	调试模式，默认0
// 返回: bool
//-------------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcInit(int SvrPort,TOnMcEvent _onevent,int _debugmode=0);

//-----------------------------------
// 函数: mcClose
// 功能: 程序退出时关闭McControl控制
// 返回: 无
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcClose();


//-----------------------------------
// 函数: mcSetMessageCallBack
// 功能: 设置你的文字通知消息处理函数
// 备注：用于McControl告诉你一些必要的信息，你可以把收到的message打印在日志窗口中，以便调试，查看异常错误原因。
// 参数: 
//       TOnMcMessage _onmess	回调函数，参见上文TOnMcMessage说明。
// 返回: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcSetMessageCallBack(TOnMcMessage _onmess);

//-----------------------------------
// 函数: mcSdkVersion
// 功能: 获取当前接口版本号
// 备注：
// 参数: 
// 返回: 版本号
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcSdkVersion();



//---------------------------------------------//
//连线状况
//---------------------------------------------//


//-----------------------------------
// 函数: mcGetBoxCount
// 功能: 得到当前已连线播放盒个数
// 返回: 连线的盒子数
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcGetBoxCount();

//-----------------------------------
// 函数: mcGetFirstBox
// 功能: 返回第一个播放盒
// 备注：用于遍历所有盒子
// 返回: 盒子临时编号，代表特定的盒子，对盒子进行后继操作时要用到。若没有，返回0
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcGetFirstBox();
//-----------------------------------
// 函数: mcGetNextBox
// 功能: 获得下一个盒子代号
// 备注：用于遍历所有盒子
// 参数: curbox	上一个盒子代号
// 返回: _DLLFUNC_MCCONTROL HBOX		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcGetNextBox(HBOX curbox);
//
//-----------------------------------
// 函数: mcFindBox
// 功能: 根据盒子名字，查找盒子临时代号
// 备注：
// 参数: _szBoxName	盒子名称（即大屏名称LedName,由初始设置给定）
// 返回: 若找不到，返回0
//-----------------------------------
_DLLFUNC_MCCONTROL HBOX		__stdcall mcFindBox(const wchar_t* _szBoxName);

//---------------------------------------------//
//设置当前要操作的盒子, 备用
_DLLFUNC_MCCONTROL bool	__stdcall mcSetCurrentBox(HBOX boxno);



//盒子基本信息
struct TMcBoxInfo 
{
	//用户自定义名称(又叫LedName或BoxName);
	wchar_t	Name[MAXLEN_BOXNAME];
	//当前ip地址
	wchar_t	ip[30];
	//LED大小
	int		width,height;
	//LED磁盘剩余空间(M)
	int		diskspace;
	//当前节目名
	wchar_t mcv[MAXLEN_MCVNAME];
	//盒子版本号
	int		ver;
};


//-----------------------------------
// 函数: mcGetBoxInfo
// 功能: 查看盒子的基本信息(名字,屏幕大小,在线状态，当前节目等)
// 备注：
// 参数: boxno	盒子编号
//		 info	返回参数，返回盒子信息
// 返回: 是否找到
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcGetBoxInfo(HBOX boxno,TMcBoxInfo* info);

//---------------------------------------------//
//	动态创建节目
//---------------------------------------------//
typedef void*	HMCV;		//节目句柄

//-----------------------------------
// 函数: mcCreateMcv
// 功能: 新建一个节目
// 备注：如果本机就是FTP服务器，可以直接把FTP文件夹作为节目的根目录，就不用专门上传FTP了
// 参数: 
//       _szMcvBasePath		专门用于保存节目的本地文件夹，所有节目都保存到此处，如E:\\McData
//       _szMcvName			节目名，40字符以内，可汉字， 保存时用这个节目名新建一个文件夹，保存所有节目数据。
//       screenwidth		节目播放窗口大小，一般要与屏幕大小相同
//       screenheight		
//       _IsDeleteOld		如果已有这个节目，是否强制删除以前节目，默认删除
//       _IsCopySrcFile		是否把节目用到的图片、视频等文件复制到节目文件夹下集中起来，便于手动上传到其它FTP服务器
// 返回: HMCV	节目句柄，代表新建的节目	失败返回 0
//-----------------------------------
_DLLFUNC_MCCONTROL HMCV	__stdcall mcCreateMcv(const wchar_t* _szMcvBasePath,const wchar_t* _szMcvName,int screenwidth,int screenheight,bool _IsDeleteOld=true,bool _IsCopySrcFile=true);

//-----------------------------------
// 函数: mcLoadMcv
// 功能: 打开一个节目，以便添加新内容
// 备注：
// 参数: 
//       _szMcvPath	节目所在文件夹，如E:\\McData\\新节目1
// 返回: HMCV	节目句柄 失败返回 0
//-----------------------------------
_DLLFUNC_MCCONTROL HMCV	__stdcall mcLoadMcv(const wchar_t* _szMcvPath);

//-----------------------------------
// 函数: mcSaveMcv
// 功能: 保存节目
// 备注：发送或上传之前，要保存
// 参数:  hmcv	节目句柄
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSaveMcv(HMCV hmcv);

//-----------------------------------
// 函数: mcSetCurMcv
// 功能: 设置当前需要编辑的节目，以便添加文字图片
// 备注：一般先hmcv = mcCreateMcv(),然后mcSetCurMcv(hmcv)
// 参数: hmcv 指定节目句柄。 
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSetCurMcv(HMCV hmcv);

//-----------------------------------
// 函数: mcCloseMcv
// 功能: 关闭节目，释放内存
// 备注：所有新建节目或打开的节目需要close
// 参数: 
//       HMCV hmcv
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcCloseMcv(HMCV hmcv);


//---------------------------------------------//
//页面播放方式：
enum TMcPlayMode {
	//0-等待节目播完，播几遍
	pm_Normal,
	//1-指定时长，播完指定时间后切换到下一页
	pm_TimeLen,
	//2-全局节目 
	pm_Global,
	//3-定时播放: 到了指定时间自动弹出
	pm_Popup,
};


//-----------------------------------
// 函数: mcAddPage
// 功能: 增加节目页
// 备注：
// 参数: 
//       _szName	节目页名字，如节目页1。若为NULL，则自动起名
//       playmode	页面播放方式: 0-正常页 1-限时页 2-全局页 3-定时弹出页
//       playtimes	playmode=0时代表播放次数，playmode=1时代表播放时间（秒）
//       backcolor	页面背景色(RGB)，默认0(黑色)
// 返回: 节目页id (pageid)，供编辑函数使用，若失败返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcAddPage(const wchar_t* _szName,TMcPlayMode playmode,int playtimes,int backcolor=0);

enum  TMcDateType{
	dt_Null		=0,	//无
	dt_Everyday	=1,	//每天
	dt_Week15	=2,	//每周一到周五
	dt_Week6	=3,	//周六
	dt_Week7	=4,	//周日
	dt_Week1	=5,	//周一
	dt_Week2	=6,	//周二
	dt_Week3	=7,	//周三
	dt_Week4	=8,	//周四
	dt_Week5	=9,	//周五
	dt_Date		=10,//指定日期
	dt_DateRange=11,//起止日期
};

//定义：时间段TMcTimeItem 
struct TMcTimeItem
{
	TMcDateType		datetype;		//定时类型
	wchar_t			begindate[20];	//类型=10或时，指定日期,如2015-07-03
	wchar_t			enddate[20];	 //类型=11时，指定终止日期如2015-07-10
	wchar_t			begintime[20];	//指定起始播放时间,如19:00:00
	wchar_t			endtime[20];	 //指定终止播放时间，如19:30:00
	int				tag;			 //标记，备用
};

//(时间段的具体例子可以MC100软件的定时节目页中查看)
//例子：每周三的19:00~19:30  dateype=7,begintime=”19:00:00” endtime=”19:30:00”

//-----------------------------------
// 函数: mcAddPageTimer
// 功能: 给节目页增加定时
// 备注：您需要在创建节目页时，把页面类型指定为3(定时页) 一个节目页可以增加多个时间段
// 参数: 
//       int pageid				指定节目页
//       TMcTimeItem * time		指定一个时间段，到了这个时间段，会自动弹出节目。
// 返回: 时间段序号
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcAddPageTimer(int pageid,const TMcTimeItem* time);

//定义：单行文字动画方式
enum TMcTxtAniMode {
	ani_TxtNull=0,	//静止
	ani_TxtMoveLeft=1,//向左走字
	ani_TxtXiangXiaHuaRu=2,//向上滑入
};

//单行文字框设置
struct TMcText 
{
	//名字（若为空，则由系统自动默认名字）
	wchar_t		name[40];
	//输出位置
	int			left,top,width,height;
	//边框宽度，默认0
	int			borderwidth;
	//边框颜色，默认白
	int			bordercolor;
	//字体（若为空，将采用盒子的默认黑体）
	wchar_t		fontname[20];
	//字号（默认20)
	int			fontsize;
	//字色
	int			fontcolor;
	//背景色
	int			backcolor;
	//透明度(0-透明 255-不透明)
	int			alpha;
	//动画效果 默认0-静止
	TMcTxtAniMode	animode;
	//移动速度等级(0-5)，默认2
	int			speedLevel;
	//每条信息显示时间(秒)  (对于水平移动文字无效，移动时间由字数和速度决定)
	int			playtime;
	//对齐方式(X) 0-左 1-中 2-右
	int			alignx;
	//绘制模式：默认1，若是动态文字，请用0
	int			drawmode;
};


//-----------------------------------
// 函数: mcGetDefaultText
// 功能: 填充一个默认文本设置，如字号默认20
// 备注：为减少文本设置工作量，可以先用本函数获得一个默认设置，然后修改其中几项。
// 参数: TMcText * text		返回默认设置
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultText(TMcText* text);

//
//-----------------------------------
// 函数: mcAddSingleText
// 功能: 添加单行文字框
// 备注：
// 参数: 
//       pageid		指定节目页(所在节目页的id)
//       _szTxt		文字内容 (如果有多条信息在同一个框中播出，请用回车符\n分隔)
//       _len		文字个数(若为-1，则表示全部长度)
//       text		文字框设置，见TMcText说明
// 返回: 返回对象id (objid)，若失败返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddSingleText(int pageid,const wchar_t* _szTxt,int _len,const TMcText* text);


//定义：图片动画类型
enum TMcAniMode {
	ani_Null=0,				//静止	    
	ani_Rand=1,				//随机	    
	ani_DanRuDanChu=2,		//淡入淡出  
	ani_XiangZuoHuaRu=3,	//向左滑入  
	ani_XiangYouHuaRu=4,	//向右滑入  
	ani_XiangShangHuaRu=5,	//向上滑入  
	ani_XiangXiaHuaRu=6,	//向下滑入  
	ani_SiJiaoHuaRu=7,		//四角滑入  
	ani_JianJianFangDa=8,	//渐渐放大  
	ani_Radar=9,			//雷达      
	ani_SuiHuaRongRu=10,	//碎花融入  
	ani_LingXingJieKai=11,	//菱形揭开  
	ani_LingXingHeLong=12,	//菱形合拢  
	ani_BaiYe=13,			//百叶      
	ani_JaoCuo=14,			//交错      
	ani_YouXuanJinRu=15,	//右旋进入  
	ani_ZuoXuanJinRu=16,	//左旋进入  
	ani_XiangShangYiDong=17,//向上移动  
	ani_XiangZuoYiDong=18,	//向左移动  
};

//图片框设置
struct TMcImage
{
	//名字（若为空，则由系统自动默认名字）
	wchar_t		name[40];
	//输出位置
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
	//缩放方式: 0-居中 1-拉伸(默认) 2-原始 
	int			scaleType;
	//动画效果
	TMcAniMode		animode;
	//移动速度等级(0-5)
	int			speedLevel;
	//每个图片显示时间(秒)
	int			playtime;
	//背景色
	int			backcolor;
	//透明度(0-透明 255-不透明)
	int			alpha;
};

//-----------------------------------
// 函数: mcGetDefaultImg
// 功能: 填充一个默认图片框设置，方便参数设置
// 备注：
// 参数: TMcImage * imgbox	返回默认设置
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultImg(TMcImage* imgbox);

//-----------------------------------
// 函数: mcAddImageBox
// 功能: 添加图片框,返回objid
// 备注：需再mcAddImageFile添加图片。可以添加多张图。
// 参数: 
//       int page		节目页id
//       imgbox			图片框设置(位置，动画方式等)
// 返回: 图片框id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddImageBox(int page,const TMcImage* imgbox);
//

//-----------------------------------
// 函数: mcAddImageFile
// 功能: 向图片框中添加图片(每个图片将轮流在这个框中显示)
// 备注：特大图片(4000以上)建议事先转成与屏幕相适合的小图片，否则影响播放
// 参数: 
//       int objid		图片框id
//       _szImgFile		图片文件路径
// 返回: 序号。 若失败返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddImageFile(int objid,const wchar_t* _szImgFile);


//视频框设置
struct TMcVideo
{
	//名字（若为空，则由系统自动默认名字）
	wchar_t		name[40];
	//输出位置
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
};


//-----------------------------------
// 函数: mcGetDefaultVideo
// 功能: 填充一个视频框默认设置，方便参数设置
// 备注：
// 参数: videobox	返回默认设置
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultVideo(TMcVideo* videobox);

//-----------------------------------
// 函数: mcAddVideoBox
// 功能: 添加视频框
// 备注：需再用mcAddVideoFile添加视频。可以添加多个。
// 参数: 
//       int pageid	节目页id
//       const TMcVideo * videobox		视频框设置
// 返回: 视频框 id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddVideoBox(int pageid,const TMcVideo* videobox);

//-----------------------------------
// 函数: mcAddVideoFile
// 功能: 向视频框中添加视频(每个视频将轮流在这个框中显示）
// 备注：建议多用mp4格式，播放较稳定。注意！有些高级格式的mp4(如High@L3)，不能同时播两个，最好事先转码（在mc100软件中工具菜单）
// 参数: 
//       int objid		视频框id
//       _szVideoFile	视频文件 (支持mp4,avi,mpg,mkv,flv,rmvb,vob等多种)
//       int volume
// 返回: 序号。 若失败返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddVideoFile(int objid,const wchar_t* _szVideoFile,int volume=100);



//广告框设置
struct TMcNotice
{
	//名字（若为空，则由系统自动默认名字）
	wchar_t		name[40];
	//输出位置
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
};

//-----------------------------------
// 函数: mcGetDefaultNotice
// 功能: 填充一个广告框默认设置，方便参数设置
// 备注：
// 参数: noticebox	返回默认设置
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultNotice(TMcNotice* noticebox);

//-----------------------------------
// 函数: mcAddNoticeBox
// 功能: 添加广告框，用以播放多行文本，RTF文本
// 备注：需再用mcAddNoticeRTF()等函数追加内容
// 参数: 
//       int pageid		节目页id
//       * notice		广告框设置
// 返回: 广告框id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddNoticeBox(int pageid,const TMcNotice* notice);

//-----------------------------------
// 函数: mcAddNoticeRTF
// 功能: 为广告框添加一个rtf文本或txt纯文本
// 备注：
// 参数: 
//       objid			广告框id
//       _szRTFFile		rtf,txt文件名
//       _animode		动画方式（默认17=向上移动，比较适合长文本）动画类型参见TMcAniMode说明
//       backcolor		背景色，默认黑
//       backalpha		背景透明度(0~255)，0-全透明
//       fontcolor		字色，默认白
//       speedLevel		移动速度(0~4)，默认2
//       pagetime		如果是翻页显示，这个值表示每页显示时间（秒）
// 返回: 序号。若失败返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddNoticeRTF(int objid,const wchar_t* _szRTFFile,TMcAniMode _animode=ani_XiangShangYiDong,int backcolor=0,int backalpha=255,int fontcolor=0xFFFFFF,int speedLevel=2,int pagetime=10);


//-----------------------------------
// 函数: mcTxt2Rtf
// 功能: 协助您处理纯文本txt文件：把txt加上字体字号转换成rtf格式，然后您可以再把这个rtf用mcAddNoticeRTF()添到节目中
// 示例：bool isok = mcTxt2Rtf(L"D:\\文档示例\\critical.txt",L"D:\\文档示例\\critical.rtf",32,L"微软雅黑",true,48,L"楷体");
// 参数: 
//       szTxtFile	原始纯文本txt，文件路径
//       szRtfFile  新生的rtf的保存文件名
//       fontsize	正文字号
//       fontname   正文字体
//       isTitleCenter  是否标题自动居中
//       titleFontSize	标题的字号，若为0，保持与正文一致。
//       titleFontName  标题的字体，若为NULL，保持与正文一致。
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL	bool	__stdcall	mcTxt2Rtf(const wchar_t* szTxtFile, const wchar_t* szRtfFile,int fontsize,const wchar_t* fontname,bool isTitleCenter=false,int titleFontSize=0,const wchar_t* titleFontName=0);

//添加时间信息框
//fmt有如下类型：
enum TMcDateFmt
{
	mcdate_YMD,	//2013年11月28日
	mcdate_MD,	//11月28日
	mcdate_W,	//星期日
	mcdate_MDW,	//11月28日 星期日
	mcdate_hm,	//07:46
	mcdate_hms,	//07:46:58
	mcdate_time,	//2013年11月28日 07:46:58

};
//信息框设置
struct TMcInfo
{
	//名字（若为空，则由系统自动默认名字）
	wchar_t		name[40];
	//输出位置
	int			left,top,width,height;
	int			borderwidth;
	int			bordercolor;
	//字号
	int			fontsize;
	//字色
	int			fontcolor;
	//背景色
	int			backcolor;
	//透明度(0-透明 255-不透明)
	int			alpha;
};

//-----------------------------------
// 函数: mcGetDefaultInfo
// 功能: 填充一个信息框默认设置，方便参数设置
// 备注：
// 参数: info	返回默认设置
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcGetDefaultInfo(TMcInfo* info);

//-----------------------------------
// 函数: mcAddInfoBoxTime
// 功能: 添加一个时间框
// 备注：
// 参数: 
//       int pageid		节目页
//       TMcDateFmt fmt	时间格式类型
//       TMcInfo * info	
// 返回: 时间信息框id
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddInfoBoxTime(int pageid,TMcDateFmt fmt,TMcInfo* info);



//-----------------------------------
// 函数: mcStartSendMcv
// 功能: 开始上传指定节目
// 备注：这个函数开始在线上传后返回。整个上传过程需要一定时间。请用mcGetUploadProgress()定时查询上传进度。
// 参数: 
//       boxno		盒子代号 (由mcGetFirstBox,mcGetNextBox获取所有在线盒子的代号)
//      _szMcvPath	节目所在文件夹，如E:\\McData\\新节目1
//		 uploadMode 0-默认方式(只上传大小有变化的文件) 1-强制上传所有文件
// 返回: 是否开始
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStartSendMcv(HBOX boxno,const wchar_t* _szMcvPath,int uploadMode=0);

//-----------------------------------
// 函数: mcGetUploadProgress
// 功能: 获得盒子当前上传的总进度
// 备注：
// 参数: 
//       HBOX boxno	盒子代号
// 返回: 进度值(0-100)
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetUploadProgress(HBOX boxno);
//停止当前上传
//-----------------------------------
// 函数: mcStopUpload
// 功能: 停止上传
// 备注：
// 参数: 
//       HBOX boxno
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStopUpload(HBOX boxno);



//FTP下载设置
struct TMcFTP
{
	//ftp svr IP或地址(若为空，则用默认设置)
	wchar_t		svrip[40];
	//ftp端口
	int			port;
	//用户名
	wchar_t		user[20];
	//密码
	wchar_t		pwd[20];
	//字符集(GBK或UTF-8)
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
// 函数: mcStartFTP
// 功能: 开始FTP下载
// 备注：调用后很快返回。然后要用mcGetProgressFTP()获取进度
// 参数: 
//       HBOX boxno		指定播放盒
//       _szFtpPath		指定FTP服务器上的待下载文件夹，注意要用相对于FTP Root目录的路径，如 "/新节目1"  "/doc/test1" 
//       _szDestPath	指定保存在盒子上哪个文件夹。若为0，表示节目专用的文件夹
//       int tag		备用标记 默认tag=1时表示下载完毕的自动开始播放
//       TMcFTP * set	FTP设置
// 返回: 是否开始
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStartFTP(HBOX boxno,const wchar_t* _szFtpPath,const wchar_t* _szDestPath=0,int tag=1,TMcFTP* set=0);

//-----------------------------------
// 函数: mcGetProgressFTP
// 功能: 获得播放盒FTP下载进度，局域网可3秒调用一次，外网建议5~10秒以上调用一次
// 备注：
// 参数: 
//       HBOX boxno		指定盒子
// 返回: _DLLFUNC_MCCONTROL int	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetProgressFTP(HBOX boxno);
//
//-----------------------------------
// 函数: mcGetFTPState
// 功能: 获得播放盒FTP下载状态
// 备注：
// 参数: 
//       HBOX boxno
// 返回: 当前状态0-无 1-正下载 2-正常结束 3-连结中 4-连接异常/无法登录  5-停止取消 6-磁盘满  7-目录错误 
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcGetFTPState(HBOX boxno);


//-----------------------------------
// 函数: mcStopFTP
// 功能: 停止当前FTP下载
// 备注：播放盒一旦开始下载，即使重启也会自动断点续传。除非你用这个命令取消下载任务。
// 参数: 
//       HBOX boxno
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcStopFTP(HBOX boxno);

//---------------------------------------------//
//节目维护
//---------------------------------------------//

//-----------------------------------
// 函数: mcSetDefaultMcv
// 功能: 指定默认播放节目
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       _szMcvName		指定节目名（事先已经上传过）
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcSetDefaultMcv(HBOX boxno,const wchar_t* _szMcvName);


//-----------------------------------
// 函数: mcGetDefaultMcv
// 功能: 获得播放盒当前正在播放的节目
// 备注：
// 参数: 
//       HBOX boxno	指定播放盒
//       _szMcvName	返回节目名
//       _MaxNameLen 节目名的最大长度
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcGetDefaultMcv(HBOX boxno,wchar_t* _szMcvName,int _MaxNameLen=50);


//-----------------------------------
// 函数: mcDeleteMcv
// 功能: 删除盒子中的节目，清理磁盘空间
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       _szMcvName		指定节目
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcDeleteMcv(HBOX boxno,const wchar_t* _szMcvName);

//
//-----------------------------------
// 函数: mcSendSingleText
// 功能: 动态推送文字：在线实时替换当前节目中的文字内容
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       int objid		指定文字框id，如果不清楚其id，可为0，换用下面_szObjName指定名称。
//       _szObjName		指定文字框名字。如果已经指定id, 此项可为0
//       _szTxt			指定文字内容 (若有多条信息用回车符\n分割)
//		_mode			0-默认效果  1-字较多时自动左滚 2-折行显示（多行文本）
// 返回:
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendSingleText(HBOX boxno,int objid,const wchar_t* _szObjName,const wchar_t* _szTxt,int _mode=0);


//-----------------------------------
// 函数: mcSendTextStyle
// 功能: 动态设定文字风格
// 备注：支持同时设多个参数。 当然，你可以每次只改变其中一个参数。
// 参数: 
//       HBOX boxno
//       int objid		指定文字框id，如果不清楚其id，可为0，换用下面_szObjName指定名称。
//       _szObjName		指定文字框名字。如果已经指定id, 此项可为0
//       const wchar_t * _szStyle	风格字符串，可以指定单个或多个风格，如： fontcolor=#FF00FF00;fontsize=32; 颜色是8位，最高两位是alpha，如#AARRGGBB
//		目前支持以下风格，例举如下：fontcolor=#FF00FF00;fontsize=32;backcolor=#80FF0000;borderwidth=4;bordercolor=#FFFF8000;animode=2;
//		animode 是TMcTxtAniMode的值。 支持0,1,2
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendTextStyle(HBOX boxno,int objid,const wchar_t* _szObjName,const wchar_t* _szStyle);

//-----------------------------------
// 函数: mcSendObjStyle
// 功能: 动态隐藏或显示控件
// 备注：这个是运行期的设置.节目跑起来后，可以遥控组件的可见性。而mcSetObjVisible是设计期设置visible，存到节目里
// 参数: 
//       HBOX boxno
//       int objid		指定框id，如果不清楚其id，可为0，换用下面_szObjName指定名称。
//       _szObjName		指定框名字。如果已经指定id, 此项可为0
//       int visible	0-不可见 1-可见
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendObjVisible(HBOX boxno,int objid,const wchar_t* _szObjName,int visible);


//-----------------------------------
// 函数: mcSetObjVisible
// 功能: 对mcv中某个组件，指定它的初始可见性。默认Add后它是可见的。如果你指定了0，则节目开始播放时，它是不可见的
// 备注：刚开始时可以先把一个控件隐藏起来，以后再调用mcSendObjVisible让它显现或消失
// 参数: 
//       int pageid   该框所在页面的id
//       int objid    该框的id，你在Add xxxx 时，返回了这个id
//       int visible 0-不可见 1-可见
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetObjVisible(int pageid,int objid,int visible);

//-----------------------------------
// 函数: mcSetLedName
// 功能: 为大屏单独起名/改名，以便管理与区分
// 备注：
// 参数: 
//       HBOX boxno
//       _szLedName		大屏播放盒的名字,可以是汉字，不要超过20字。
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedName(HBOX boxno,const wchar_t* _szLedName);

//-----------------------------------
// 函数: mcSetMcid
// 功能: 为大屏赋一个内部id (mcid)
// 备注：
// 参数: 
//       HBOX boxno
//       _mcid 数字或英文 ,最大长度11个字符
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetMcid(HBOX boxno,const char* _mcid);


//////////////////////////////////////////////////////////////////////////
//			节目预览功能
//
//注意事项：
//mcDisplay...系列函数，要在主线程中调用
//--不要调用mcCloseMcv()关闭它,否则看不到预览. mcCloseMcv会关闭当前编辑的节目
//--需要在定时器中调用mcDisplayDraw()完成画面刷新。定时器的间隔决定帧率。建议20ms (50FPS) 如果机器比较烂，就30ms (30FPS)
//--可以直接预览内存中mcv,所见即所得。 中途一般不需要mcSaveMcv();只要在用户退出编辑时mcSaveMcv()即可
//
//---------------------------------------------//

/*
使用例举:

(1)程序初始化时
mcInit(8300,OnMcEvent,1);
...
mcDisplayInit(hwnd_screenWin,0x404040);

(2)预览
mcSetCurMcv(..)
mcDisplayShow();//开始预览当前节目
TimerDisplay->Enabled=true;//开启一个定时器，每20ms调用一次

(3)
onTimer()
{
	mcDisplayDraw();//调用该函数刷新预览画面
}

*/

//-----------------------------------
// 函数: mcDisplayInit
// 功能: 初始化预览功能
// 备注：输出窗口最好是固定大小。(如果中途调整了大小，要在resize之后，重新调用mcDisplayInit)
//			预览功能对显卡有一定要求。有的机器显示性能很差(如有个别Atom平板电脑)，可能会初始化失败,无法预览。
// 参数: 
//       HWND outWindow		输出窗口的windows句柄(hwnd)，可以在任意窗口中输出（甚至是按钮，只要有hwnd）
//       int backcolor		输出窗口的背景色。（清屏颜色）
//       int mode			备用参数。默认0
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcDisplayInit(void* outWindow,int backcolor=0x803010,int mode=0);

//-----------------------------------
// 函数: mcDisplayShow
// 功能: 预览当前节目
// 备注：可用mcSetCurMcv()设定当前正在编辑的节目。 并且，预览前不要调用mcCloseMcv!否则什么也看不到。
//		 预览是实时的，预览前不必保存节目。
// 参数: 
//       int pageid		如果节目有多页，可以指定一个起始页。
//       bool repeate   是否重复播放该页。（因为用户可能要一直编辑其中一页，不希望自动翻页）
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayShow(int pageid=0,bool repeate=false);


//-----------------------------------
// 函数: mcDisplayDraw
// 功能: 刷新预览窗口
// 备注：刷新的频率决定了帧率。 刷的越快，画面越流畅，CPU占用率会高一点。该函数通常放在定时器中。(不要另外开线程刷，因为刷新要在主线程中进行)
// 参数: 
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayDraw();


//-----------------------------------
// 函数: mcDisplayStop
// 功能: 停止预览当前节目。
// 备注：
// 参数: 
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayStop();


//-----------------------------------
// 函数: mcDisplayResize
// 功能: 缩放显示：重置预览画面大小
// 备注：你需要在外部代码中先重新设置outWindow(预览窗口)大小，然后再调用该函数通知内核。
//		 outWindow->Width=xxx; outWindow->Height=xxx; mcDisplayResize();
// 参数: 
// 返回: 
_DLLFUNC_MCCONTROL void	__stdcall mcDisplayResize();

//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// 函数: mcConnectBox
// 功能: 与盒子建立临时连接 （由电脑主动发起连接，连到盒子）
// 备注：该函数不耗时，会立即返回。 连接成功后，会通知MCEVENT_CONNECT消息 (要事先在mcinit()时注册TOnMcEvent接口)。
//		临时连接最多允许512个。太多临时连接会降低连接性能。
//		SDK默认的连接是服务器方式，电脑做为服务器监听端口，各个盒子事先设置好自动回连。
//
// 参数: 
//       const char * IP		盒子IP
//       int port				盒子的端口(默认8100)
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL	void	__stdcall mcConnectBox(const char* IP,int port=8100);


//-----------------------------------
// 函数: mcDisConnectBox
// 功能: 断开与盒子的临时连接
// 备注：
// 参数: 
//       HBOX boxno
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL	void	__stdcall mcDisConnectBox(HBOX boxno);



//-----------------------------------
// 函数: mcFindBoxByIP
// 功能: 根据IP查找盒子。若没找到，返回0
// 备注：
// 参数: 
//       const char * IP
// 返回: 盒子在线编号
//-----------------------------------
_DLLFUNC_MCCONTROL	HBOX		__stdcall mcFindBoxByIP(const char* IP);


//获取盒子是否在线
_DLLFUNC_MCCONTROL	bool		__stdcall mcBoxIsConnect(HBOX boxno);

//-----------------------------------
// 函数: mcBoxSetTime
// 功能: 设置盒子时钟 
// 备注：该函数会使盒子重启,需重新连接。
// 参数: 
//       HBOX boxno
//       const char * stime		当前时间，格式(19个字符)：2016-06-12 12:08:16
// 返回: 是否设置完毕
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSetTime(HBOX boxno,const char* stime);


 
//-----------------------------------
// 函数: mcBoxSetAutoConnect
// 功能: 设置盒子自动回连
// 备注：会引发自动重连。如果设置为停止回连，就需要手动输入IP,用mcConnectBox建立连接。
// 参数: 
//       HBOX boxno		盒子代号
//       const char * svrIP		服务器IP或网址 若为NULL，则终止回连
//       int port	服务器端口
// 返回: 是否设置完毕
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSetAutoConnect(HBOX boxno,const char* svrIP,int port);


//清空当前继电器设置，准备重新设置
_DLLFUNC_MCCONTROL void		__stdcall mcBoxClearRelay(HBOX boxno);

//-----------------------------------
// 函数: mcBoxAddRelayTime
// 功能: 增加一项继电器定时设置(可以多项). 
// 备注：各项时间段不能互相冲突，起始时间应顺序递增
// 参数: 
//       int relayNo		继电器编号，支持两路(1或2)
//       int beginHour		起始时间_小时部分
//       int beginMin		起始时间_分钟部分
//       int onoff			开关(0-关，1-开)
// 返回: _DLLFUNC_MCCONTROL void		__stdcall
// 示例： 早上7点开，晚上22点关
//		mcBoxAddRelayTime(1,7,0,1);
//		mcBoxAddRelayTime(1,22,0,0);
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxAddRelayTime(HBOX boxno,int relayNo,int beginHour,int beginMin,int onoff);


//-----------------------------------
// 函数: mcBoxSendRelayTime
// 功能: 发送继电器定时设置
// 备注：在各项定时mcBoxAddRelayTime之后，调用该函数上传设置
// 参数: 
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxSendRelayTime(HBOX boxno);

//取消继电器定时设置
_DLLFUNC_MCCONTROL void		__stdcall mcBoxStopRelayTime(HBOX boxno);

//-----------------------------------
// 函数: mcBoxReboot
// 功能: 重启播放盒
// 备注：重启后，需要重新连接
// 参数: 
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxReboot(HBOX boxno);


//-----------------------------------
// 函数: mcBoxSetRebootTime
// 功能: 设置每天定时重启
// 备注：
// 参数: 
//       int hour	重启时间，几点几分
//       int min
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxSetRebootTime(HBOX boxno,int hour,int min);


//-----------------------------------
// 函数: mcBoxGetMcvList
// 功能: 获得盒子上当前节目清单
// 备注：
// 参数: 
//       HBOX boxno
//       int timeout	阻塞时间ms, 由于从盒子上读回节目清单需要一定的时间，所以需要一个超时。调用线程会阻塞。
// 返回: 获取的节目个数。 -1:获取失败(超时) 0-没有节目 >=1节目数
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcBoxGetMcvList(HBOX boxno,int timeout=1500);

struct TMcProgramInfo {
	//节目名(节目文件夹名称)
	wchar_t name[200];
	//上传时间，如: 2016-07-12 12:01:32
	wchar_t	date[40];
	//大小(字节)
	int		size;
	//节目备注信息
	wchar_t rem[100];
};

//-----------------------------------
// 函数: mcBoxGetMcvInfo
// 功能: 获取节目信息
// 备注：目前只能从盒子读回节目名等信息，然后你可以显示节目清单，切换当前节目。
//		 无法从盒子下载节目内容。 所以盒子上的节目要在本机上备份，否则没办法修改。
// 参数: 
//       HBOX boxno		盒子代号
//       int index		节目序号。 (index>=0 并且index < 节目个数) (先用上面mcBoxGetMcvList()获取节目个数)
//       TMcProgramInfo * tf
// 返回: 是否获得， 如果index非法返回false
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcBoxGetMcvInfo(HBOX boxno,int index,TMcProgramInfo* tf);


//////////////////////////////////////////////////////////////////////////
//
//	下面一系列 mcMcv.... 函数，用来访问/修改以前节目内容。
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// 函数: mcMcvGetPageCount
// 功能: 返回当前正在编辑的节目的节目页个数
// 备注：事先mcCreateMcv()或mcLoadMcv()，并且mcSetCurMcv()
// 参数: 
// 返回: _DLLFUNC_MCCONTROL int	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetPageCount();

//-----------------------------------
// 函数: mcMcvGetPageId
// 功能: 返回指定序号的节目页的id,以便使用mcAddPageTimer,mcDelPage等函数
// 备注：
// 参数: 
//       int index
// 返回: pageid (>=1), 若找不到返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetPageId(int index);

//删除指定页
_DLLFUNC_MCCONTROL bool	__stdcall mcMcvDelPage(int pageid);

//指定节目页，返回它里面的节目对象个数 
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetObjCount(int pageid);


//-----------------------------------
// 函数: mcMcvGetObjId
// 功能: 返回某页中指定序号对象的id
// 备注：
// 参数: 
//       int pageid
//       int index	序号，0 ~ page.objcount-1
//
// 返回: objid (>=1) 若找不到，返回0
//-----------------------------------
_DLLFUNC_MCCONTROL int	__stdcall mcMcvGetObjId(int pageid,int index);

enum TMcObjType {
	mcobj_Null,		//未知
	mcobj_Page,		//节目页
	mcobj_TextBox,	//文字框
	mcobj_ImageBox,	//图像框
	mcobj_VideoBox,	//视频框
	mcobj_NoticeBox,//广告框(图文框)
	mcobj_InfoBox,	//信息框
	mcobj_SoundBox,	//音乐框
	mcobj_ClockBox,	//模拟时钟
};

struct TMcObjInfo 
{
	int			id;		//对象id
	TMcObjType	type;	//类型
	wchar_t		name[40];//名称
	int			left,top,width,height;//大小
};


//-----------------------------------
// 函数: mcMcvGetObjInfo
// 功能: 获得指定对象的信息
// 备注：
// 参数: 
//       int objid		对象的id，由mcMcvGetObjId()获得，或者由mcAdd....系列函数返回的id
//       TMcObjInfo * tf
// 返回: _DLLFUNC_MCCONTROL bool	__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool	__stdcall mcMcvGetObjInfo(int objid,TMcObjInfo* tf);

//删除指定对象
_DLLFUNC_MCCONTROL void	__stdcall mcMcvDelObj(int objid);

//-----------------------------------
// 函数: mcGetUploadFile
// 功能: 获得盒子当前需要上传的文件情况
// 备注：适用于本机上传，对FTP下载方式无效
// 参数: 
//       HBOX boxno	盒子代号
// 返回: 是否上传状态:0-无  3-正在上传 4-上传结束 5-异常/取消
//-----------------------------------
struct TMcUploadFileInfo
{
	//正在上传的文件名
	wchar_t szFile[300];
	//正在上传的文件序号
	int		index;
	//此次要上传的文件总数
	int		count;
};
_DLLFUNC_MCCONTROL int 	__stdcall mcGetUploadFileInfo(HBOX boxno,TMcUploadFileInfo* info);



//-----------------------------------
// 函数: mcSetLedIP
// 功能: 设置大屏启用静态IP
// 备注：设置IP会引发当前LED中断连接，重新连接。
// 参数: 
//       HBOX boxno
//       const char * _szIP		 指定一个固定IP 
//       const char * _gateWay	 网关。若为NULL，采用xxx.xxx.xxx.1
//       const char * _mask		 掩码。若为NULL，采用255.255.255.0
//       const char * _dns1		 为了能够顺利上网，有时需要指定一个dns域名
//       const char * _dns2
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedIP(HBOX boxno,const char* _szIP,const char* _gateWay=0,const char* _mask=0,const char* _dns1=0,const char* _dns2=0);


//-----------------------------------
// 函数: mcSetLedDhcp
// 功能: 设置大屏dhcp
// 备注：重设网络会引发当前LED中断连接，重新连接。
// 参数: 
//       HBOX boxno		
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetLedDhcp(HBOX boxno);

//-----------------------------------
// 函数: mcSetLedDhcp
// 功能: 设置当前亮度
// 备注：
// 参数: 
//       HBOX boxno		
//		 int bright		亮度0~100 (默认75) 0:会引发黑屏（有时需要临时黑屏）
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetCurrentBright(HBOX boxno,int bright=75);

_DLLFUNC_MCCONTROL void   __stdcall mcNowDatetime(double* dtime);


// borderImg="border7b.png" borderWidth="8" borderMoveSpeed="12" borderRepeatCount="4" borderBlendmode="0"

//-----------------------------------
// 函数: mcSetPageBorder
// 功能: 给页面加一个跑马灯边框
// 备注：(需播放盒升到1.99版)
// 参数: 
//       int pageid			页面id
//       int borderStyle	跑马灯样式: 1-长条彩带 2-彩色水滴型 3-圆点	
//       int moveSpeed		移动速度。默认12，越大越快
//       int repeatCount	跑马灯的密度（重复次数越多越密)
//		 int aniType		动画类型: 0-静止 1-顺时针 2-逆时针 3-振荡 4-闪烁
//		 int duration		周期（毫秒): 对于振荡类的动画，需要周期参数
//		 int borderWidth	边框显示宽度（像素点数)
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetPageBorder(int pageid,int borderStyle,int moveSpeed=12,int repeatCount=4,int aniType=1,int duration=1500,int borderWidth=4);

// 功能: 给页面加一个跑马灯边框，自定义样式
// 备注：(需播放盒升到1.99版)
// 参数: 
//       int pageid			页面id
//       wchar_t* _szBorderImgFile	跑马灯边条图片(建议png格式，具有alpha通道)	建议大小 256x32,或512x32 (一个水平彩带状的图片)
//       int moveSpeed		移动速度。默认12，越大越快
//       int repeatCount	跑马灯的密度（重复次数越多越密)
//		 int aniType		动画类型: 0-静止 1-顺时针 2-逆时针 3-振荡 4-闪烁
//		 int duration		周期（毫秒): 对于振荡类的动画，需要周期参数
//		 int borderWidth	边框显示宽度（像素点数)
// 返回: 
_DLLFUNC_MCCONTROL bool		__stdcall mcSetPageBorderEx(int pageid,const wchar_t* _szBorderImgFile,int moveSpeed=12,int repeatCount=4,int aniType=1,int duration=1500,int borderWidth=4);


//-----------------------------------
// 函数: mcCloseScreen
// 功能: 关屏（进入黑屏）
// 备注：节目停播。 如果有定时插播，也会一同停止插播。注意要在适当机会调用mcOpenScreen，否则以为屏幕坏掉了!
// 参数: 
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcCloseScreen(HBOX boxno);


//-----------------------------------
// 函数: mcOpenScreen
// 功能: 开屏
// 备注：重新播放先前的默认节目
// 参数: 
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcOpenScreen(HBOX boxno);



//-----------------------------------
// 函数: mcBoxSetRelay   
// 功能: 发送继电器状态
// 备注：调用该函数上传设置  add by [zz]
// 参数: 
//       int relayNo		继电器编号，支持两路(1或2)
//       int onoff			开关(0-关，1-开)
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL void		__stdcall mcBoxSetRelay(HBOX boxno,int relayNo,int onoff);


//-----------------------------------
// 函数: mcGet4GState
// 功能: 查询3G/4G的工作状态
// 备注：
// 参数: 
// 返回: -1获取失败(可稍后再查) -2盒子版本不支持	0-3G/4G没有安装  1-已上线 2-已下线 3-正在工作 4-异常
//-----------------------------------
_DLLFUNC_MCCONTROL int  __stdcall mcGet4GState(HBOX boxno);


//-----------------------------------
// 函数: mcGet4GIMEI
// 功能: 获取4G模块IMEI
// 备注：
// 参数: 
//       HBOX boxno	 盒子号
//       char * IMEI	返回字符串
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL bool __stdcall mcGet4GIMEI(HBOX boxno,char* IMEI);

//[V1.3]
//
//-----------------------------------
// 函数: mcSendTexts
// 功能: 动态推送多个文字：在线实时替换当前节目中的文字内容，可以指定多个文字框
// 备注：
// 参数: 
//       HBOX boxno			指定播放盒
//		mode			0-默认效果  1-字较多时自动左滚
//       _szNameAndTexts	文字框名称及其内容，可以有多项，各项间用|间隔。如：文字框1:ABC|文字框2:HELLO|文字框3:张三
// 返回: 是否成功
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendTexts(HBOX boxno,int mode,const wchar_t* _szNameAndTexts);



struct TMessageBox {
	//显示时间
	int timeout;
	//对话框的大小
	int width;
	int height;
	//标题1的字体大小，字色
	int fontsize;
	int fontcolor;
	//标题2的字体大小，字色
	int fontsize2;
	int fontcolor2;
	//背景色
	int backcolor;
	//标题1
	wchar_t title[100];
	//标题2
	wchar_t title2[100];
	//背景图（默认为空，若要指定一个图片，则必须事先上传到UserMedia
	wchar_t backimg[100];
};


//-----------------------------------
// 函数: mcShowMessage
// 功能: 在大屏上弹出临时消息框，然后自动消失
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       int mode		方式：0-居中显示  
//       const wchar_t * _szText	要显示的文字。若有折行，用\n分隔
//       TMessageBox*	msgbox	弹出框的参数
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcShowMessage(HBOX boxno,const TMessageBox* msgbox);


//////////////////////////////////////////////////////////////////////////
//
//		下面mcGrid系列函数，提供了对表格数据、视图显示的控制
//
//		mcGridData,mcGridClear,mcGridAddRow,mcGridSetCell 等函数，提供了动态发送表格数据功能。可动态添加删除行，修改指定单元格。
//		mcAddGrid mcGridShow，mcGridAutoScroll 负责画面显示输出。 定义文字阵列作为单元格， 指定翻页方式等。
//////////////////////////////////////////////////////////////////////////

//-----------------------------------
// 函数: mcAddGrid
// 功能: 在节目页上定义一个表格。用多个文本框组成阵列，作为一个表格视图，用来显示表格数据
// 备注：这个一般和节目页绑定，保存在mcv.xml中
// 参数: 
//       int pageid		页面id
//       int gridid		指定表格，把视图与表格绑定
//       int column		列数。 一般应与grid列数相同
//       const wchar_t * txtBoxNameList  文本框名称阵列，用|分割。按上面的列数折行。 每个文字框对应一个表格cell
// 返回: txt 个数 （单元格个数)
//-----------------------------------
_DLLFUNC_MCCONTROL int		__stdcall mcAddGrid(int pageid,int gridid,int column, const wchar_t* txtBoxNameList);


//-----------------------------------
// 函数: mcGridData
// 功能: 发送表格数据到盒子上
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       int gridid		grid的ID
//       int column		列数 
//       const wchar_t * txtArray	表格中各项的文字内容。各项用|分割，按上面的列数自动折行。若该项为NULL，则创建空表。
//							文字总长不得超过60000字。若超过，请用mcGridAddRow逐行添加。
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridData(HBOX boxno,int gridid, int column,const wchar_t* txtArray);


//-----------------------------------
// 函数: mcGridClear
// 功能: 清空指定表格
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       int gridid		指定表格
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridClear(HBOX boxno,int gridid);

//-----------------------------------
// 函数: mcGridAddRow
// 功能: 给盒子上的表格添加一行数据
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       int gridid		指定表格
//       const wchar_t* rowtexts  表格行内容，各项用|分割。项数应该与表格的列数相同。
//       int row		插入的行号位置，将在该行之前插入(行号从0开始)。默认-1，表示在末尾添加。 
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcGridAddRow(HBOX boxno,int gridid,const wchar_t* rowtexts,int row=-1);


//-----------------------------------
// 函数: mcSendFile
// 功能: 上传一个用户自己的文件到盒子里
// 备注：注意！ 本函数可能需要一定的时间，请用mcGetUploadProgress来查询传送状态
// 参数: 
//       HBOX boxno		指定播放盒
//       const wchar_t * _szFile			文件在本机上的所在路径;
//       const wchar_t * _szDestDir			盒子上的目标文件夹，区分大小写。默认user
// 返回: 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSendFile(HBOX boxno,const wchar_t* _szFile,const wchar_t* _szDestDir=0);



//下面三个函数用来上传多个文件到盒子中！
//-----------------------------------
// 函数: mcClearSendFile
// 功能: 清除当前上传任务
// 备注：
// 参数: 
//       HBOX boxno
// 返回: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcClearSendFile(HBOX boxno);


//-----------------------------------
// 函数: mcAddSendFile
// 功能: 添加一个待上传的本地文件.
// 备注：
// 参数: 
//       HBOX boxno
//       const wchar_t * _szFile
// 返回: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcAddSendFile(HBOX boxno,const wchar_t* _szFile);

//-----------------------------------
// 函数: mcStartSendFile
// 功能: 开始上传用户文件
// 备注：可用于动态刷新图片。先用这个函数，把多个图片素材上传到user目录下。然后用mcSetImageBox()设置图片框的当前图片内容。
//		注意！ 本函数可能需要一定的时间，请用mcGetUploadProgress来查询传送状态
//		另外， 图片文件尽量事先做成小图片，太大的图片较费内存
//		可用mcStopUpload停止上传

// 参数: 
//       HBOX boxno
//       const wchar_t * _szDestDir			盒子上的目标文件夹，区分大小写。默认user
// 返回: _DLLFUNC_MCCONTROL void		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcStartSendFile(HBOX boxno,const wchar_t* _szDestDir=0);


//-----------------------------------
// 函数: mcDelUserFile
// 功能: 删除盒子里的用户自己的文件
// 备注：
// 参数: 
//       HBOX boxno		指定播放盒
//       const wchar_t * _szFile			文件名，支持*通配符(*.* 或*.jpg 或 abc*.jpg 或abc*)	 不支持?通配符
//       const wchar_t * _szDestDir			盒子上的目标文件夹，区分大小写。默认user
// 返回: 若通配符有误，返回false
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcDelUserFile(HBOX boxno,const wchar_t* _szFile,const wchar_t* _szDestDir=0);


//-----------------------------------
// 函数: mcSetImageBox
// 功能: 动态更换节目上的图片。
// 备注：也可以同时更换多张图。它们将轮流显示。
// 参数: 
//       HBOX boxno
//		const wchar_t* _szImageBoxName		图片框名称
//       const wchar_t * _szUserImageFile	盒子上的图片文件名。请事先上传图片素材到_szUserDir下。可以是多张图片，中间用|分割
//		 const wchar_t* _szUserDir 盒子上的图片文件所在文件夹。若为NULL，则默认为user文件夹
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetImageBox(HBOX boxno,const wchar_t* _szImageBoxName, const wchar_t* _szUserImageFile=0,const wchar_t* _szUserDir=0);



#define IMGDATA_RGB		1	//24bit RGB内存流
#define IMGDATA_ARGB	2	//32bit ARGB内存流
#define IMGDATA_JPG		3	//jpg 的内存文件
#define IMGDATA_PNG		4	//png 的内存文件
#define IMGDATA_BMP		5	//bmp 的内存文件
//-----------------------------------
// 函数: mcSetImageBoxData
// 功能: 使用图像RGB数据，或内存数据，动态更换节目上的图片。
// 备注：图像不要太大，盒子内存有限。建议与imagebox一样大小。
// 参数: 
//       HBOX boxno
//       const wchar_t * _szImageBoxName	图片框名称
//       int _ImageIndex	图片索引，0表示第一张。 一个图片框可能包含有多张图片。可以指定其中一张进行更换。如果索引号 >= img count ，将自动添加图片到末尾，而不是替换。
//       int datatype	图像buf数据格式。上面IMGDATA_XXX 类型选一个
//       void * buf		内存buf
//       int buflen		长度(字节数）
//       int imgw	图像的大小。 如果是RGB数据，必须指定图像宽高。如果是jpg,png,bmp，该项可以是0，盒子可以自动解析大小。 如果不为0，就按所指定的大小进行缩放。
//       int imgh
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetImageBoxData(HBOX boxno,const wchar_t* _szImageBoxName,int _ImageIndex,int datatype,void* buf,int buflen,int imgw,int imgh);


//-----------------------------------
// 函数: mcSetVideoBox
// 功能: 动态更换节目上的视频。
// 备注：也可以同时更换多张图。它们将轮流显示。
// 参数: 
//       HBOX boxno
//		const wchar_t* _szVideoBoxName		视频框名称
//       const wchar_t * _szUserVideoFile	盒子上的视频文件名。请事先上传视频素材到_szUserDir下。可以是多张视频，中间用|分割
//		 const wchar_t* _szUserDir 盒子上的视频文件所在文件夹。若为NULL，则默认为user文件夹
// 返回: _DLLFUNC_MCCONTROL bool		__stdcall
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcSetVideoBox(HBOX boxno,const wchar_t* _szVideoBoxName, const wchar_t* _szUserVideoFile=0,const wchar_t* _szUserDir=0);



//-----------------------------------
// 函数: mcCheckMcv
// 功能: 检查盒子上是否有某个节目
// 备注：这个函数会等待盒子的返回，会有延时
// 参数: 
//       HBOX boxno
//       const wchar_t * _szMcv	 节目名
//		timeOutMs		延时 (ms)
// 返回: 是否存在 
//-----------------------------------
_DLLFUNC_MCCONTROL bool		__stdcall mcCheckMcv(HBOX boxno,const wchar_t* _szMcv,int timeOutMs=1500);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif // McControl_h__
