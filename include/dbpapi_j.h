#if !defined _DPAPI2_J_H_I
#define _DPAPI2_J_H_I

//��������
#define DT_VOID		0	//������
#define DT_DIGITAL	1	//������	,Ҳ��4�ֽ�����		
#define DT_INT32	2	//4�ֽ�����
#define DT_FLOAT32	3	//4�ֽڸ�����
#define DT_INT64	4	//8�ֽ�����
#define DT_FLOAT64	5	//8�ֽڸ�����
#define DT_STRING	6	//�ַ���
#define DT_BLOB		7	//���������ݿ�

// ���������������
#define DBP_QUALITY_GOOD					0x00	// GOOD
#define DBP_QUALITY_BAD						0x01	// ��ֵ����û���ض�ԭ��
#define DBP_QUALITY_BAD_CONFIG_ERROR		0x02	// ���������ô������磬��ǩ�����еķ�����������ɾ����
#define DBP_QUALITY_BAD_NOT_CONNECTED		0x03	// ����Ҫ�����ӵ�����Ŀ�꣬���޷��ɹ����ӡ�������˵���ڸ�ʱ����ϵ�ֵ�����ţ���Ϊ��һ������Դδ�ṩ
#define DBP_QUALITY_BAD_DEVICE_FAIL			0x04	// ��⵽�豸����
#define DBP_QUALITY_BAD_SENSOR_FAIL			0x05	// ��⵽����������
#define DBP_QUALITY_BAD_LAST				0x06	// �豸���������ͨѶʧ�ܣ����ǣ������Чֵ�ǿ��õġ���ע�⣬ʱ���ǵ������һ����Чֵ��ʱ�꣬���������������ʱ��
#define DBP_QUALITY_BAD_COMM_FAIL			0x07	// �豸���������ͨѶʧ�ܣ�����û�������õ���Чֵ
#define DBP_QUALITY_BAD_OUT_OF_SERVICE		0x08	// �ñ�ǩ���鱻����Ϊ�ǻ����ʾ�ñ�ǩ����ĳ��ԭ�򲻱�����������

#define DBP_QUALITY_UNCERTAIN				0xE0	// ֵ��Ч�Բ�ȷ������û���ض�ԭ��
#define DBP_QUALITY_UNCERTAIN_LAST			0xE1	// �Ѿ�ֹͣд������ֵ����ֵӦ����Ϊ����ȥֵ������ע�⣬��״̬��ʾΪ�ڹ涨ʱ���ڱ�ǩδ�����¡���Bad/Last Usable Value��ʾΪ��⵽ͨѶ������
#define DBP_QUALITY_UNCERTAIN_SENSOR		0xE2	// ֵ�������������̷�Χ���ߴ������ڲ����Ϊ������׼ȷ
#define DBP_QUALITY_UNCERTAIN_EU_EXCEEDED	0xE3	// ����ֵ�����˸�ֵ����ķ�Χ����ע�⣬��״̬������ζ�Ÿ�ֵ�ǳ��޵�
#define DBP_QUALITY_UNCERTAIN_SUB_NORMAL	0xE4	// ��ֵ���Զ��Դ��������״̬ΪGOOD������Դ�������ڹ涨ֵ

#define DBP_QUALITY_CALC_OFF				0xF0	// ����ֹͣ

#define DBP_QUALITY_LOLO_LIMIT				0xF9	// �͵ͱ�
#define DBP_QUALITY_LO_LIMIT				0xFA	// �ͱ�
#define DBP_QUALITY_HI_LIMIT				0xFB	// �߱�
#define DBP_QUALITY_HIHI_LIMIT				0xFC	// �߸߱�
#define DBP_QUALITY_NOT_INIT				0xFD	// �ű���������δ����ʼ��
#define DBP_QUALITY_SHUTDOWN				0xFE	// �������ر�
#define DBP_QUALITY_UNKOWN					0xFF	// δ֪����

// ����ʷstr,��ʷblob,SOE��ʱ�������
#define  RST_CUR			1		//��ǰʱ�괦
#define  RST_AFTER			2		//��ǰ֮��һ��
#define  RST_DEFAULT		(RST_CUR | RST_AFTER) //��ǰ��֮��һ��

// GetHisVal3ʹ�ã���ʾ���ָ��ʱ����ֵʱ�����ȡֵ
#define RHV_BEFORE			1		// ȡǰһֵ
#define RHV_AFTER			2		// ȡ��һֵ
#define RHV_INTERP			3		// ȡ��ֵ

//Ȩ��
#define DS_POWERMAN		0x00000001 //�˺Ź���
#define DS_SYSMAN		0x00000002 //ϵͳ����,���ݿ������ǩ����
#define DS_WRITE		0x00000004 //дȨ��
#define DS_READ			0x00000008 //��Ȩ��

//��ǩ����
#define TAG_FLAG_DDC	0 //�ֳ����ݱ�ǩ(ʵʱ���ǩ) ������"ddctag"
#define TAG_FLAG_DEF	1 //�����ǩ(�ֹ���ǩ)       ������"deftag"
#define TAG_FLAG_PRE	2 //Ԥ�����ǩ(Ԥ�����ǩ)   ������"pretag"
#define TAG_FLAG_CUR	3 //���߱�ǩ                 ������"curtag"	
#define TAG_FLAG_CALC	4 //�����ǩ				 ������"calctag"

//��ʷ��������
#define HISTORY_RAW     0
#define HISTORY_INSTEP  1
#define HISTPRY_PLOT    2

// ��Ϊ�˹淶java�ӿ�����
#define DBPGetVersion DBP_GetVersion

extern "C"{

// dbpapi�汾�ţ��汾��V2.5.4.0��ʾΪ0x02050400
DWORD  __stdcall DBP_GetVersion();

///////////////////////////////////////////////////////////
//�������
DWORD __stdcall DBPCreate( //��������
		char*	sips[],    //������IP���� 16�ֽ�
		char*	susers[],	//��¼�˺����� 16�ֽ�
		char*	spasss[],  //��¼��������  16�ֽ�
		WORD	wport[],	//�˿�����
		int		nsize		//�������
		);	//���ض�����	

DWORD __stdcall DBPCreate2( //��������֧��Ⱥ��
		char*	sip,    // ������IP 16�ֽ�
		char*	suser,	// ��¼�˺� 16�ֽ�
		char*	spass,  // ��¼����  16�ֽ�
		WORD	wport,		// �˿�
		BOOL	bUseGroup	//�Ƿ�ʹ��Ⱥ��
		);	//���ض�����	

DWORD __stdcall DBPDestroy( //���ٶ���
		DWORD	h  //������������Destroy�󣬸ò����Ͳ�����ʹ����
		);//���ش�����

///////////////////////////////////////////////////////////
//���ӹ���
DWORD __stdcall DBPConnect( //���ӷ�����
		DWORD	h
		);//���ش�����

DWORD __stdcall DBPDisConnect( //�Ͽ�����������
		DWORD	h
		);//���ش�����

DWORD __stdcall DBPIsConnect( //�жϷ���������
		DWORD	h,
		int*	pnCon //in/out, ����0��ʾû���ӣ���0��ʾ������
		);//���ش�����

DWORD __stdcall DBPGetSvrConInfo( //ȡ������Ϣ
								 DWORD	h,
								 char*	sip,    //in/out������IP 16�ֽ�
								 int* pnNum			// in/out �������ͻ�������
								 );//���ش������
///////////////////////////////////////////////////////////
//��ǩ����
DWORD __stdcall DBPQueryTagFromDbp(//��ѯ��ǩ,��ѯ���������ڶ����У��´���EnumTagAttr���ȡ����
		DWORD	h,
		const	char*	snameflt, //��ǩ�����˴���NULL��մ���ʾȫ��
		const	char*	sdesflt,  //�������˴���NULL��մ���ʾȫ��
		int				ntype,	  //��������,-1��ʾȫ��
		long*			pltagnum	//��ѯ���ı�ǩ��
		);//���ش�����
		
DWORD __stdcall DBPQueryTagFromDbp2(//��ѯ��ǩ,��ѯ���������ڶ����У��´���EnumTagAttr���ȡ����
		DWORD	h,
		const	char*	snameflt, //��ǩ�����˴���NULL��մ���ʾȫ��
		const	char*	sdesflt,  //�������˴���NULL��մ���ʾȫ��
		int			    ntype,	  //��������,-1��ʾȫ��
		int			    ntagtype, //��ǩ����,-1��ʾȫ��
		long*			pltagnum  //��ѯ���ı�ǩ��
		);//���ش�����
											 
DWORD __stdcall DBPEnumTagAttr( //ö�ٲ�ѯ���ǩ����
		DWORD				h,
		char*	sname,		//��ǩ�� 80
		char*	sdbname,	//���ݿ�ʵ���� 16
		char*	sdbtagname,	//���ݿ��ǩ�� 80
		char*	sdes,		//���� 80
		char*	sunit,		//��λ 16
		DWORD*  dwid,		//��ǩID
		WORD*	wtype,		//��������
		short*  errcode,	//�������
		long*	ltagflag,	//��ǩ����
		int		nIndex      //λ�� 0����ѯ�ı�ǩ��-1
		); //���� DBP_OK��ʾ�ɹ�����������û����

DWORD __stdcall DBPGetTagAttrFromDbp( //�ӷ�����ȡ��ǩ����
		DWORD	h,
		char*	sname[],		//in/out��ǩ�� 80
		char*	sdbname[],		//in/out���ݿ�ʵ���� 16
		char*	sdbtagname[],	//in/out���ݿ��ǩ�� 80
		char*	sdes[],			//in/out���� 80
		char*	sunit[],		//in/out��λ 16
		DWORD   dwid[],				//in/out��ǩID
		WORD	wtype[],			//in/out��������
		short   errcode[],			//in/out�������
		long	ltagflag[],			//in/out��ǩ����,
		int		nsize
		);//���ش�����


DWORD __stdcall DBPGetTagAttr( //�ӷ�����ȡ������ǩ����
		DWORD	h,
		char*	sname,		//in/out��ǩ�� 80
		char*	sdbname,		//in/out���ݿ�ʵ���� 16
		char*	sdbtagname,	//in/out���ݿ��ǩ�� 80
		char*	sdes,			//in/out���� 80
		char*	sunit,		//in/out��λ 16
		DWORD*  dwid,				//in/out��ǩID
		WORD*	wtype,			//in/out��������
		short*  errcode,			//in/out�������
		long*	ltagflag			//in/out��ǩ����,
		);//���ش�����	
///////////////////////////////////////////////////////////
//��ֵ

DWORD __stdcall DBPGetSnapshotStr( //������,string
		DWORD		h,
		const char* sname, //��ǩ��
		long*		pltime,
		short*		psnqa,
		char		sval[],		//������,������ַ�������β��0
		int			nsize,			//��������С
		int*		pndatasize		//���յ������ֽ���������������0
		); //���ش�����

DWORD __stdcall DBPGetSnapshotBlob( //������,blob
		DWORD		h,
		const char* sname, //��ǩ��
		long*		pltime,
		short*		psnqa,
		char		sval[],		//������,������ַ�������β��0
		int			nsize,			//��������С
		int*		pndatasize		//�����������ֽ�����
		);//���ش�����

DWORD __stdcall DBPGetHisVal( //����ʷ
		DWORD		h,
		const char* sname, //��ǩ��
		long		lts,		//in,��ʼʱ��
		long		lte,		//in,����ʱ��
		long		linsertsec, //in,��ֵʱ��
		long		lflag,	//in,��־
		double		dblvals[],//in/out,double��
		long		lvals[], //in/out,long ��
		long		ltimes[], //in/out,ʱ����
		short		snqas[],  //in/out,������
		int			nsize,   //in,�ռ��С
		int*		pntype,  //in/out,��������
		int*		pndatasize//in/out,���ݸ���
		);
DWORD __stdcall DBPGetHisValFilter(
		DWORD		h, 
		const char* sname, //��ǩ��
		long		lts,		//in,��ʼʱ��
		long		lte,		//in,����ʱ��
		long		linsertsec, //in,��ֵʱ��
		long		lflag,	//in,��־
		const char *expression,//�����ַ���
		double		dblvals[],//in/out,double��
		long		lvals[], //in/out,long ��
		long		ltimes[], //in/out,ʱ����
		short		snqas[],  //in/out,������
		int			nsize,   //in,�ռ��С
		int*		pntype,  //in/out,��������
		int*		pndatasize//in/out,���ݸ���
		);

DWORD __stdcall DBPGetHisStr( //��string��ʷ
		DWORD		h,
		const char* sname, //in,��ǩ��
		long		ltime,		//in,��ʼʱ��
		DWORD		dwflag,		//in,��־,RST_CUR,RST_AFTER,RST_DEFAULT֮һ
		long*		pltime,		//in/out, ʱ��
		short*		psnqa,		//in/out, ����
		char		sval[],		//������,������ַ�������β��0
		int			nsize,		//in,��������С
		int*		pndatasize	//in/out,���յ������ֽ���������������0
		); //���ش�����

DWORD __stdcall DBPGetHisBlob( //��BLOB��ʷ
		DWORD		h,
		const char* sname, //in,��ǩ��
		long		ltime,		//in,��ʼʱ��
		DWORD		dwflag,		//int,��־,RST_CUR,RST_AFTER,RST_DEFAULT֮һ
		long*		pltime,		//in/out, ʱ��
		short*		psnqa,		//in/out, ����
		char		sval[],		//in/out, ������,������ַ�������β��0
		int			nsize,		//in,��������С
		int*		pndatasize	//in/out,�����������ֽ�����
		);//���ش�����

///////////////////////////////////////////////////////////
// дֵ
DWORD __stdcall DBPWriteSnapshotStr( //дstring ����
		DWORD		h,
		const char* sname, //��ǩ��
		long		ltime,		 //ʱ��
		short		snqa,		 //����
		const char*	sval		 //�ַ���ֵ����β��0
		); //���ش�����

DWORD __stdcall DBPWriteHisStr( //дstring��ʷ
		DWORD		h,
		const char* sname, //��ǩ��
		long		ltime,		 //ʱ��
		short		snqa,		 //����
		const char*	sval		 //�ַ���ֵ����β��0
		); //���ش�����

DWORD __stdcall DBPWriteSnapshotBlob( //дBLOB����
		DWORD		h,
		const char* sname, //��ǩ��
		long		ltime,		 //ʱ��
		short		snqa,		 //����
		char*		pval,		 //ֵ
		int			ndatasize	 //ֵ����		
		); //���ش�����

DWORD __stdcall DBPWriteHisBlob( //дBLOB��ʷ
		DWORD		h,
		const char* sname, //��ǩ��
		long		ltime,		 //ʱ��
		short		snqa,		 //����
		char*		pval,		 //ֵ
		int			ndatasize	 //ֵ����
		); //���ش�����

DWORD __stdcall DBPDelHis( //ɾ��������ʷ
		DWORD		h,
		const char* sname, //in ��ǩ��
		long		lts,   //in ��ʼʱ��
		long		lte    //in ����ʱ��
		);

DWORD __stdcall DBPGetSumVal(
		DWORD	h,
		const char* sname,		//in,��ǩ��
		long		lts,		//in,��ʼʱ��
		long		lte,		//in,����ʱ��
		long		ltype,		//in,ͳ������
		long		lds,		//in,�ֶ�ʱ��
		long		ltimes[],	//in/out,ʱ��
		short		snqas[],	//in/out,����
		double		dblvals[],	//in/out,dblvalֵ�����
		int			lvals[],	//in/out,longֵ�����
		int			nsize,		//in,�����ֵ�ռ��С
		int*		pntype,		//in/out,��ǩֵ����DT_FLOAT32��
		int*		pnrecsize	//in/out,ʵ��������ݸ���
		);//��ͳ��ֵ������nsize����¼

DWORD __stdcall DBPDelHis2( //ɾ�������ʷ
		DWORD	h,
		char*	sTagNames[],	//in,��ǩ���ַ���ָ������  
		long	ltimes[],		//in, ʱ��
		short	errs[],			//in/out, ������
		int		nsize			//in,����
		);

/* ����֧�ָýӿڣ���DBPGetMultiPointHisVal()����
DWORD __stdcall DBPGetHisVal2( //�������ʷ
		DWORD		h,
		long	ltime,				//in,ʱ��
		char*	sTagNames[],		//��ǩ���ַ���ָ������  //apistring
		int		ntypes[],			//in/out,��������,DT_INT32,DT_FLOAT32��
		double	dblvals[],			//in/out,ֵ����ռ�
		long	lvals[],			//in/out,ֵ����ռ�
		short	snqas[],			//in/out,����
		short	snerrs[],			//in/out,����
		int		nsize				//in,��ǩ����
		);
*/
DWORD __stdcall DBPGetSnapshot( //������
		DWORD	h,
		char*	sTagNames[],//in,��ǩ���ַ���ָ������  
		long	ltimes[],	//in/out, ʱ��
		short	snqas[],	//in/out, ����
		double  dblvals[],	//in/out, ���doubleֵ,DT_FLOAT32,DT_FLOAT64�����
		long	lvals[],	//in/out, ���Longֵ,DT_DIGITAL,DT_INT32,DT_INT64�����
		int		ntypes[],	//in/out, ��������,DT_INT32,DT_FLOAT32�ȡ�
		short	errs[],		 //in/out, ������
		int		nsize		 //in, ����
		);//���ش�����

DWORD __stdcall DBPWriteHis( //д��ʷ
		DWORD	h,
		char*	sTagNames[],  //in,��ǩ���ַ���ָ������  
		long	ltimes[],	  //in, ʱ��
		short	snqas[],	  //in, ����
		double  dblvals[],	  //in, ���doubleֵ,DT_FLOAT32,DT_FLOAT64�����
		long	lvals[],	  //in, ���Longֵ,DT_DIGITAL,DT_INT32,DT_INT64�����
		int		ntypes[],	  //in, ��������,DT_INT32,DT_FLOAT32�ȡ�
		short	errs[],		  //in/out, ������
		int		nsize		  //in, ����
		);

DWORD __stdcall DBPWriteSnapshot( //д����
		DWORD	h,
		char*	sTagNames[],  //in,��ǩ���ַ���ָ������  
		long	ltimes[],	  //in, ʱ��
		short	snqas[],	  //in, ����
		double  dblvals[],	  //in, ���doubleֵ,DT_FLOAT32,DT_FLOAT64�����
		long	lvals[],	  //in, ���Longֵ,DT_DIGITAL,DT_INT32,DT_INT64�����
		int		ntypes[],	  //in, ��������,DT_INT32,DT_FLOAT32�ȡ�
		short	errs[],		  //in/out, ������
		int		nsize		  //in, ����
		);

DWORD __stdcall DBPGetServerInfo(
		DWORD	h,
		DWORD *dwVersion,			// ����������汾��
		long *lNowTime,				// ��ǰʱ��
		long *lStartupTime,			// ����ʱ��
		DWORD *dwConnectCount,		// ��ǰ������
		DWORD *dwTagCount,			// ��ǰ��ǩ����
		float *fCPUUsage,			// CPUռ��
		DWORD *dwHandleCount,		// �����
		DWORD *dwThreadsCount,		// �߳���
		DWORD *dwMemoryBytes,		// �ڴ�ʹ��
		DWORD *dwMemoryBytesPeak,	// �߷��ڴ�ʹ��
		DWORD *dwVirtualBytes,		// �����ڴ��С
		DWORD *dwVirtualBytesPeak	// �߷������ڴ��С
		);

DWORD __stdcall DBPGetMultiPointHisVal(
		DWORD		h,
		long	lMode,				//ȡֵģʽ��RHV_BEFORE��RHV_AFTER��RHV_INTERP
		char*	sTagNames[],		//��ǩ���ַ���ָ������  //apistring
		long	ltimes[],			//in/out,��ǩ����Ӧʱ�����飬���ΪRHV_BEFORE��RHV_AFTER���ͣ��᷵�ر��ֵ����ʵʱ��
		int		ntypes[],			//in/out,��������,DT_INT32,DT_FLOAT32��
		double	dblvals[],			//in/out,ֵ����ռ�
		long	lvals[],			//in/out,ֵ����ռ�
		short	snqas[],			//in/out,����
		short	snerrs[],			//in/out,����
		int		nsize				//in,��ǩ����
		);

DWORD __stdcall DBPGetDiSumVal(
		DWORD		h,
		const char* sname,		//in,��ǩ��
		long		lts,		//in,��ʼʱ��
		long		lte,		//in,����ʱ��
		long		lds,		//in,�ֶ�ʱ��
		long		lval,		//in,ͳ�ƿ�����ֵ
		long		lpSumCount[],	//in/out,���ص�ͳ�ƴ�������
		long		lpSumTimes[],	//in/out,���ص�ͳ��ʱ������
		int			nsize,		//in,�����ֵ�ռ��С
		int*		pnrecsize	//in/out,ʵ��������ݸ���
		);

DWORD __stdcall DBPGetErrorMessage( //��ȡ�������������Ϣ�����sval==NULL��dwsize�ռ䲻�㣬��������APIE_MEMORY��dwsize������С����ռ��С
			   DWORD	dwErrorCode,// in,API���÷��صĴ�����
			   char*	sval,		// out, ��ŷ��صĴ�����Ϣ���������NULL����pnrecsize��������ռ��С
			   DWORD	*dwsize	// in/out, ��ŷ��صĴ�����Ϣ�������Ĵ�С��128���ȵ�sval�ռ��ǰ�ȫ��
			);

}
#endif //_DPAPI2_J_H_I