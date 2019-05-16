#ifndef __DBPAPI_ERR_H_
#define __DBPAPI_ERR_H_

//通用错误代码 
#define  DBERR_OK  			0x00		//成功
#define  DBERR_FAIL			0x01		//失败
//数据库级别错误代码
#define  DBERR_CONNECT		0x02		//没有连接，连接失败
#define  DBERR_NOINTERFACE	0x03		//无此接口
#define  DBERR_HANDLE		0x04		//错误的驱动句柄
#define  DBERR_RECS  		0x05		//错误的记录集
#define	 DBERR_MEMEORY		0x06		//内存错误
#define  DBERR_NOTSUPPORT	0x07		//不支持
#define  DBERR_NOMOREREC	0x08		//没有记录
#define  DBERR_NOTAG		0x09		//无此标签
#define  DBERR_TYPE			0x0A		//类型无法转换或错误
#define  DBERR_TIME			0x0B		//时标错误
#define  DBERR_NOVAL		0x0C		//没有数据
#define  DBERR_NOALL_OK		0x0D		//标签操作不全成功
#define  DBERR_BUF_FULL		0x0E		//缓存已满，请稍后再补
#define  DBERR_NODRV		0X1E		//无此驱动

//代理服务器级别错误代码
#define DBPE_OK				0x00	//成功
#define DBPE_ERR			0x01	//错误
#define DBPE_NOTAG		  	0xA1	//标签不存在
#define DBPE_TAGID_EXIST	0xA2	//ID存在
#define DBPE_TAGNAME_EXIST	0xA3	//名称存在
#define DBPE_TAG_EXIST		0xA4	//ID和名称存在
#define DBPE_TYPE			0xA5	//类型错误
#define DBPE_MODIFYTYPE     0xA6	//不能修改类型
#define DBPE_MEMORY			0xA7	//内存错误
#define DBPE_NODB			0xA8	//无数据库
#define DBPE_CONBUSY		0xA9	//数据库连接忙
#define DBPE_MANING			0xAA    //数据库管理中
#define DBPE_DBCONID		0xAB    //数据库连接ID错误
#define DBPE_TIMEOUT		0xAC	//超时
#define DBPE_NODATA			0xAD	//无数据
#define DBPE_WORKBUSY		0xAE	//数据库工作忙
#define DBPE_INVALID_SESSION	0xAF	//无效Session，也可能是对应Session的标签已经被删除，需要重新搜索

//标签值错误代码
#define  ERRTV_OK		DBERR_OK	//成功
#define  ERRTV_NOTAG	DBERR_NOTAG	//无此标签
#define  ERRTV_TYPE		DBERR_TYPE	//类型错误或类型无法转换
#define  ERRTV_TIME		DBERR_TIME	//时标错误
#define  ERRTV_ERR		DBERR_FAIL  //标签值错误或无效
#define  ERRTV_BUF_FULL	DBERR_BUF_FULL //缓存已满，请稍后再补

// 安全级别错误代码 
#define DS_OK				0		//
#define DS_ERR				1		//失败
#define DS_ACTOR_FULL		0x20	//角色满
#define DS_OPERATOR_FULL	0x21	//用户满
#define DS_TRUSTIP_FULL		0x22	//信任连接满
#define DS_NOACTOR			0x23	//无此角色
#define DS_NOOPERATOR		0x24	//无此账号
#define DS_NOTRUSTIP		0x25	//无此IP
#define DS_ACTOR_EXIST		0x26	//角色存在
#define DS_OPERATOR_EXIST   0x27	//账号存在
#define DS_TRUSTIP_EXIST    0x28	//IP存在
#define DS_NOPOWER			0x29	//无此权限
#define DS_PASSERR			0x2A	//密码错误
#define DS_USERLOCK			0x2B	//账号禁用
#define DS_ACTORUSE			0x2C	//角色被使用


//代理服务器API错误代码
#define APIE_OK				0x00
//#define APIE_ERR			0x01
#define APIE_HANDLE			0xC0		//错误的句柄
#define APIE_CONNECT		0xC1		//没有连接
#define APIE_MEMORY			0xC2		//API内存错误
#define APIE_NOMOREBUF      0xC3		//接收数据区不够
#define APIE_ERR			0xC4		//新定义值，与DBPE_ERR区分

#endif /* __DBPAPI_ERR_H_ */