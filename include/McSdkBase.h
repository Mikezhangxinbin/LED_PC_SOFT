//===================================================================
/**\file  
 *  Filename:   McSdkBase.h
 *  Desc:       MC100 SDK  ��������
 *		
 *		
 *      
 *  His:      mcsoft create @  2015-7-9   14:23
 */
//===================================================================
#ifndef McSdkBase_h__
#define McSdkBase_h__

//����MCRESULT==0 ��ʾ�ɹ�
typedef int MCRESULT;
#define  MC_OK	0
#define  MC_FAIL -1

//�ڲ�ͨѶ���ݰ�ͷ
struct TMcDataHead 
{
	int		len;
	int		cmd;
};

#endif // McSdkBase_h__