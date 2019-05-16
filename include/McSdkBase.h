//===================================================================
/**\file  
 *  Filename:   McSdkBase.h
 *  Desc:       MC100 SDK  基本定义
 *		
 *		
 *      
 *  His:      mcsoft create @  2015-7-9   14:23
 */
//===================================================================
#ifndef McSdkBase_h__
#define McSdkBase_h__

//所有MCRESULT==0 表示成功
typedef int MCRESULT;
#define  MC_OK	0
#define  MC_FAIL -1

//内部通讯数据包头
struct TMcDataHead 
{
	int		len;
	int		cmd;
};

#endif // McSdkBase_h__