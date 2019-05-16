#if !defined _DPAPI2_J_H_I
#define _DPAPI2_J_H_I

//数据类型
#define DT_VOID		0	//空类型
#define DT_DIGITAL	1	//开关量	,也是4字节整数		
#define DT_INT32	2	//4字节整数
#define DT_FLOAT32	3	//4字节浮点数
#define DT_INT64	4	//8字节整数
#define DT_FLOAT64	5	//8字节浮点数
#define DT_STRING	6	//字符串
#define DT_BLOB		7	//二进制数据块

// 代理服务器质量码
#define DBP_QUALITY_GOOD					0x00	// GOOD
#define DBP_QUALITY_BAD						0x01	// 坏值，但没有特定原因
#define DBP_QUALITY_BAD_CONFIG_ERROR		0x02	// 服务器配置错误。例如，标签从运行的服务器配置中删除了
#define DBP_QUALITY_BAD_NOT_CONNECTED		0x03	// 输入要求连接到其他目标，但无法成功连接。该质量说明在该时间点上的值不可信，因为有一个数据源未提供
#define DBP_QUALITY_BAD_DEVICE_FAIL			0x04	// 检测到设备故障
#define DBP_QUALITY_BAD_SENSOR_FAIL			0x05	// 检测到传感器故障
#define DBP_QUALITY_BAD_LAST				0x06	// 设备与服务器的通讯失败，但是，最后有效值是可用的。请注意，时标标记的是最后一个有效值的时标，而不是质量变更的时标
#define DBP_QUALITY_BAD_COMM_FAIL			0x07	// 设备与服务器的通讯失败，并且没有最后可用的有效值
#define DBP_QUALITY_BAD_OUT_OF_SERVICE		0x08	// 该标签或组被设置为非活动，表示该标签由于某种原因不被服务器更新

#define DBP_QUALITY_UNCERTAIN				0xE0	// 值有效性不确定，但没有特定原因
#define DBP_QUALITY_UNCERTAIN_LAST			0xE1	// 已经停止写入最新值，该值应被视为“过去值”。请注意，该状态表示为在规定时间内标签未被更新。而Bad/Last Usable Value表示为检测到通讯错误发生
#define DBP_QUALITY_UNCERTAIN_SENSOR		0xE2	// 值超出传感器量程范围或者传感器内部诊断为测量不准确
#define DBP_QUALITY_UNCERTAIN_EU_EXCEEDED	0xE3	// 返回值超出了该值定义的范围。请注意，该状态并不意味着该值是超限的
#define DBP_QUALITY_UNCERTAIN_SUB_NORMAL	0xE4	// 该值来自多个源，但其中状态为GOOD的数据源个数少于规定值

#define DBP_QUALITY_CALC_OFF				0xF0	// 计算停止

#define DBP_QUALITY_LOLO_LIMIT				0xF9	// 低低报
#define DBP_QUALITY_LO_LIMIT				0xFA	// 低报
#define DBP_QUALITY_HI_LIMIT				0xFB	// 高报
#define DBP_QUALITY_HIHI_LIMIT				0xFC	// 高高报
#define DBP_QUALITY_NOT_INIT				0xFD	// 才被创建，还未被初始化
#define DBP_QUALITY_SHUTDOWN				0xFE	// 服务器关闭
#define DBP_QUALITY_UNKOWN					0xFF	// 未知错误

// 读历史str,历史blob,SOE是时标的意义
#define  RST_CUR			1		//当前时标处
#define  RST_AFTER			2		//当前之后一条
#define  RST_DEFAULT		(RST_CUR | RST_AFTER) //当前或之后一条

// GetHisVal3使用，表示如果指定时标无值时，如何取值
#define RHV_BEFORE			1		// 取前一值
#define RHV_AFTER			2		// 取后一值
#define RHV_INTERP			3		// 取插值

//权限
#define DS_POWERMAN		0x00000001 //账号管理
#define DS_SYSMAN		0x00000002 //系统管理,数据库管理，标签管理
#define DS_WRITE		0x00000004 //写权限
#define DS_READ			0x00000008 //读权限

//标签类型
#define TAG_FLAG_DDC	0 //现场数据标签(实时库标签) 导入名"ddctag"
#define TAG_FLAG_DEF	1 //定义标签(手工标签)       导入名"deftag"
#define TAG_FLAG_PRE	2 //预处理标签(预处理标签)   导入名"pretag"
#define TAG_FLAG_CUR	3 //曲线标签                 导入名"curtag"	
#define TAG_FLAG_CALC	4 //计算标签				 导入名"calctag"

//历史数据类型
#define HISTORY_RAW     0
#define HISTORY_INSTEP  1
#define HISTPRY_PLOT    2

// 仅为了规范java接口命名
#define DBPGetVersion DBP_GetVersion

extern "C"{

// dbpapi版本号，版本号V2.5.4.0表示为0x02050400
DWORD  __stdcall DBP_GetVersion();

///////////////////////////////////////////////////////////
//对象管理
DWORD __stdcall DBPCreate( //创建对象
		char*	sips[],    //服务器IP数组 16字节
		char*	susers[],	//登录账号数组 16字节
		char*	spasss[],  //登录密码数组  16字节
		WORD	wport[],	//端口数组
		int		nsize		//数组个数
		);	//返回对象句柄	

DWORD __stdcall DBPCreate2( //创建对象，支持群组
		char*	sip,    // 服务器IP 16字节
		char*	suser,	// 登录账号 16字节
		char*	spass,  // 登录密码  16字节
		WORD	wport,		// 端口
		BOOL	bUseGroup	//是否使用群组
		);	//返回对象句柄	

DWORD __stdcall DBPDestroy( //销毁对象
		DWORD	h  //对象句柄，调用Destroy后，该参数就不能再使用了
		);//返回错误码

///////////////////////////////////////////////////////////
//连接管理
DWORD __stdcall DBPConnect( //连接服务器
		DWORD	h
		);//返回错误码

DWORD __stdcall DBPDisConnect( //断开服务器连接
		DWORD	h
		);//返回错误码

DWORD __stdcall DBPIsConnect( //判断服务器连接
		DWORD	h,
		int*	pnCon //in/out, 回填0表示没连接，非0表示已连接
		);//返回错误码

DWORD __stdcall DBPGetSvrConInfo( //取连接信息
								 DWORD	h,
								 char*	sip,    //in/out服务器IP 16字节
								 int* pnNum			// in/out 服务器客户连接数
								 );//返回错误代码
///////////////////////////////////////////////////////////
//标签属性
DWORD __stdcall DBPQueryTagFromDbp(//查询标签,查询后结果保存在对象中，下次用EnumTagAttr逐个取属性
		DWORD	h,
		const	char*	snameflt, //标签名过滤传，NULL或空串表示全部
		const	char*	sdesflt,  //描述过滤传，NULL或空串表示全部
		int				ntype,	  //数据类型,-1表示全部
		long*			pltagnum	//查询到的标签数
		);//返回错误码
		
DWORD __stdcall DBPQueryTagFromDbp2(//查询标签,查询后结果保存在对象中，下次用EnumTagAttr逐个取属性
		DWORD	h,
		const	char*	snameflt, //标签名过滤传，NULL或空串表示全部
		const	char*	sdesflt,  //描述过滤传，NULL或空串表示全部
		int			    ntype,	  //数据类型,-1表示全部
		int			    ntagtype, //标签类型,-1表示全部
		long*			pltagnum  //查询到的标签数
		);//返回错误码
											 
DWORD __stdcall DBPEnumTagAttr( //枚举查询后标签属性
		DWORD				h,
		char*	sname,		//标签名 80
		char*	sdbname,	//数据库实例名 16
		char*	sdbtagname,	//数据库标签名 80
		char*	sdes,		//描述 80
		char*	sunit,		//单位 16
		DWORD*  dwid,		//标签ID
		WORD*	wtype,		//数据类型
		short*  errcode,	//错误代码
		long*	ltagflag,	//标签类型
		int		nIndex      //位置 0到查询的标签数-1
		); //返回 DBP_OK表示成功，否则错误或没有了

DWORD __stdcall DBPGetTagAttrFromDbp( //从服务器取标签属性
		DWORD	h,
		char*	sname[],		//in/out标签名 80
		char*	sdbname[],		//in/out数据库实例名 16
		char*	sdbtagname[],	//in/out数据库标签名 80
		char*	sdes[],			//in/out描述 80
		char*	sunit[],		//in/out单位 16
		DWORD   dwid[],				//in/out标签ID
		WORD	wtype[],			//in/out数据类型
		short   errcode[],			//in/out错误代码
		long	ltagflag[],			//in/out标签类型,
		int		nsize
		);//返回错误码


DWORD __stdcall DBPGetTagAttr( //从服务器取单个标签属性
		DWORD	h,
		char*	sname,		//in/out标签名 80
		char*	sdbname,		//in/out数据库实例名 16
		char*	sdbtagname,	//in/out数据库标签名 80
		char*	sdes,			//in/out描述 80
		char*	sunit,		//in/out单位 16
		DWORD*  dwid,				//in/out标签ID
		WORD*	wtype,			//in/out数据类型
		short*  errcode,			//in/out错误代码
		long*	ltagflag			//in/out标签类型,
		);//返回错误码	
///////////////////////////////////////////////////////////
//读值

DWORD __stdcall DBPGetSnapshotStr( //读快照,string
		DWORD		h,
		const char* sname, //标签名
		long*		pltime,
		short*		psnqa,
		char		sval[],		//接收区,如果是字符串，结尾补0
		int			nsize,			//接收区大小
		int*		pndatasize		//接收的数据字节数，不含结束符0
		); //返回错误码

DWORD __stdcall DBPGetSnapshotBlob( //读快照,blob
		DWORD		h,
		const char* sname, //标签名
		long*		pltime,
		short*		psnqa,
		char		sval[],		//接收区,如果是字符串，结尾补0
		int			nsize,			//接收区大小
		int*		pndatasize		//读到的数据字节数。
		);//返回错误码

DWORD __stdcall DBPGetHisVal( //读历史
		DWORD		h,
		const char* sname, //标签名
		long		lts,		//in,开始时间
		long		lte,		//in,结束时间
		long		linsertsec, //in,插值时间
		long		lflag,	//in,标志
		double		dblvals[],//in/out,double区
		long		lvals[], //in/out,long 区
		long		ltimes[], //in/out,时标区
		short		snqas[],  //in/out,质量区
		int			nsize,   //in,空间大小
		int*		pntype,  //in/out,数据类型
		int*		pndatasize//in/out,数据个数
		);
DWORD __stdcall DBPGetHisValFilter(
		DWORD		h, 
		const char* sname, //标签名
		long		lts,		//in,开始时间
		long		lte,		//in,结束时间
		long		linsertsec, //in,插值时间
		long		lflag,	//in,标志
		const char *expression,//过滤字符串
		double		dblvals[],//in/out,double区
		long		lvals[], //in/out,long 区
		long		ltimes[], //in/out,时标区
		short		snqas[],  //in/out,质量区
		int			nsize,   //in,空间大小
		int*		pntype,  //in/out,数据类型
		int*		pndatasize//in/out,数据个数
		);

DWORD __stdcall DBPGetHisStr( //读string历史
		DWORD		h,
		const char* sname, //in,标签名
		long		ltime,		//in,开始时间
		DWORD		dwflag,		//in,标志,RST_CUR,RST_AFTER,RST_DEFAULT之一
		long*		pltime,		//in/out, 时标
		short*		psnqa,		//in/out, 质量
		char		sval[],		//接收区,如果是字符串，结尾补0
		int			nsize,		//in,接收区大小
		int*		pndatasize	//in/out,接收的数据字节数，不含结束符0
		); //返回错误码

DWORD __stdcall DBPGetHisBlob( //读BLOB历史
		DWORD		h,
		const char* sname, //in,标签名
		long		ltime,		//in,开始时间
		DWORD		dwflag,		//int,标志,RST_CUR,RST_AFTER,RST_DEFAULT之一
		long*		pltime,		//in/out, 时标
		short*		psnqa,		//in/out, 质量
		char		sval[],		//in/out, 接收区,如果是字符串，结尾补0
		int			nsize,		//in,接收区大小
		int*		pndatasize	//in/out,读到的数据字节数。
		);//返回错误码

///////////////////////////////////////////////////////////
// 写值
DWORD __stdcall DBPWriteSnapshotStr( //写string 快照
		DWORD		h,
		const char* sname, //标签名
		long		ltime,		 //时标
		short		snqa,		 //质量
		const char*	sval		 //字符串值，结尾补0
		); //返回错误码

DWORD __stdcall DBPWriteHisStr( //写string历史
		DWORD		h,
		const char* sname, //标签名
		long		ltime,		 //时标
		short		snqa,		 //质量
		const char*	sval		 //字符串值，结尾补0
		); //返回错误码

DWORD __stdcall DBPWriteSnapshotBlob( //写BLOB快照
		DWORD		h,
		const char* sname, //标签名
		long		ltime,		 //时标
		short		snqa,		 //质量
		char*		pval,		 //值
		int			ndatasize	 //值长度		
		); //返回错误码

DWORD __stdcall DBPWriteHisBlob( //写BLOB历史
		DWORD		h,
		const char* sname, //标签名
		long		ltime,		 //时标
		short		snqa,		 //质量
		char*		pval,		 //值
		int			ndatasize	 //值长度
		); //返回错误码

DWORD __stdcall DBPDelHis( //删除单点历史
		DWORD		h,
		const char* sname, //in 标签名
		long		lts,   //in 开始时间
		long		lte    //in 结束时间
		);

DWORD __stdcall DBPGetSumVal(
		DWORD	h,
		const char* sname,		//in,标签名
		long		lts,		//in,开始时间
		long		lte,		//in,结束时间
		long		ltype,		//in,统计类型
		long		lds,		//in,分段时间
		long		ltimes[],	//in/out,时标
		short		snqas[],	//in/out,质量
		double		dblvals[],	//in/out,dblval值输出区
		int			lvals[],	//in/out,long值输出区
		int			nsize,		//in,输出区值空间大小
		int*		pntype,		//in/out,标签值类型DT_FLOAT32等
		int*		pnrecsize	//in/out,实际输出数据个数
		);//读统计值，最多读nsize条记录

DWORD __stdcall DBPDelHis2( //删除多点历史
		DWORD	h,
		char*	sTagNames[],	//in,标签名字符串指针数组  
		long	ltimes[],		//in, 时标
		short	errs[],			//in/out, 错误码
		int		nsize			//in,个数
		);

/* 不再支持该接口，由DBPGetMultiPointHisVal()代替
DWORD __stdcall DBPGetHisVal2( //读多点历史
		DWORD		h,
		long	ltime,				//in,时间
		char*	sTagNames[],		//标签名字符串指针数组  //apistring
		int		ntypes[],			//in/out,数据类型,DT_INT32,DT_FLOAT32等
		double	dblvals[],			//in/out,值输出空间
		long	lvals[],			//in/out,值输出空间
		short	snqas[],			//in/out,质量
		short	snerrs[],			//in/out,错误
		int		nsize				//in,标签个数
		);
*/
DWORD __stdcall DBPGetSnapshot( //读快照
		DWORD	h,
		char*	sTagNames[],//in,标签名字符串指针数组  
		long	ltimes[],	//in/out, 时标
		short	snqas[],	//in/out, 质量
		double  dblvals[],	//in/out, 存放double值,DT_FLOAT32,DT_FLOAT64存放区
		long	lvals[],	//in/out, 存放Long值,DT_DIGITAL,DT_INT32,DT_INT64存放区
		int		ntypes[],	//in/out, 数据类型,DT_INT32,DT_FLOAT32等。
		short	errs[],		 //in/out, 错误码
		int		nsize		 //in, 个数
		);//返回错误码

DWORD __stdcall DBPWriteHis( //写历史
		DWORD	h,
		char*	sTagNames[],  //in,标签名字符串指针数组  
		long	ltimes[],	  //in, 时标
		short	snqas[],	  //in, 质量
		double  dblvals[],	  //in, 存放double值,DT_FLOAT32,DT_FLOAT64存放区
		long	lvals[],	  //in, 存放Long值,DT_DIGITAL,DT_INT32,DT_INT64存放区
		int		ntypes[],	  //in, 数据类型,DT_INT32,DT_FLOAT32等。
		short	errs[],		  //in/out, 错误码
		int		nsize		  //in, 个数
		);

DWORD __stdcall DBPWriteSnapshot( //写快照
		DWORD	h,
		char*	sTagNames[],  //in,标签名字符串指针数组  
		long	ltimes[],	  //in, 时标
		short	snqas[],	  //in, 质量
		double  dblvals[],	  //in, 存放double值,DT_FLOAT32,DT_FLOAT64存放区
		long	lvals[],	  //in, 存放Long值,DT_DIGITAL,DT_INT32,DT_INT64存放区
		int		ntypes[],	  //in, 数据类型,DT_INT32,DT_FLOAT32等。
		short	errs[],		  //in/out, 错误码
		int		nsize		  //in, 个数
		);

DWORD __stdcall DBPGetServerInfo(
		DWORD	h,
		DWORD *dwVersion,			// 代理服务器版本号
		long *lNowTime,				// 当前时间
		long *lStartupTime,			// 启动时间
		DWORD *dwConnectCount,		// 当前连接数
		DWORD *dwTagCount,			// 当前标签总数
		float *fCPUUsage,			// CPU占用
		DWORD *dwHandleCount,		// 句柄数
		DWORD *dwThreadsCount,		// 线程数
		DWORD *dwMemoryBytes,		// 内存使用
		DWORD *dwMemoryBytesPeak,	// 高峰内存使用
		DWORD *dwVirtualBytes,		// 虚拟内存大小
		DWORD *dwVirtualBytesPeak	// 高峰虚拟内存大小
		);

DWORD __stdcall DBPGetMultiPointHisVal(
		DWORD		h,
		long	lMode,				//取值模式，RHV_BEFORE、RHV_AFTER、RHV_INTERP
		char*	sTagNames[],		//标签名字符串指针数组  //apistring
		long	ltimes[],			//in/out,标签名对应时标数组，如果为RHV_BEFORE、RHV_AFTER类型，会返回标符值的真实时标
		int		ntypes[],			//in/out,数据类型,DT_INT32,DT_FLOAT32等
		double	dblvals[],			//in/out,值输出空间
		long	lvals[],			//in/out,值输出空间
		short	snqas[],			//in/out,质量
		short	snerrs[],			//in/out,错误
		int		nsize				//in,标签个数
		);

DWORD __stdcall DBPGetDiSumVal(
		DWORD		h,
		const char* sname,		//in,标签名
		long		lts,		//in,开始时间
		long		lte,		//in,结束时间
		long		lds,		//in,分段时间
		long		lval,		//in,统计开关量值
		long		lpSumCount[],	//in/out,返回的统计次数数组
		long		lpSumTimes[],	//in/out,返回的统计时间数组
		int			nsize,		//in,输出区值空间大小
		int*		pnrecsize	//in/out,实际输出数据个数
		);

DWORD __stdcall DBPGetErrorMessage( //获取错误码的描述信息，如果sval==NULL或dwsize空间不足，则函数返回APIE_MEMORY，dwsize返回最小所需空间大小
			   DWORD	dwErrorCode,// in,API调用返回的错误码
			   char*	sval,		// out, 存放返回的错误信息，如果传递NULL，则pnrecsize返回所需空间大小
			   DWORD	*dwsize	// in/out, 存放返回的错误信息缓冲区的大小，128长度的sval空间是安全的
			);

}
#endif //_DPAPI2_J_H_I