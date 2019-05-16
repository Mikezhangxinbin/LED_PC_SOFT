#ifndef __DBPAPI_ERR_H_
#define __DBPAPI_ERR_H_

//ͨ�ô������ 
#define  DBERR_OK  			0x00		//�ɹ�
#define  DBERR_FAIL			0x01		//ʧ��
//���ݿ⼶��������
#define  DBERR_CONNECT		0x02		//û�����ӣ�����ʧ��
#define  DBERR_NOINTERFACE	0x03		//�޴˽ӿ�
#define  DBERR_HANDLE		0x04		//������������
#define  DBERR_RECS  		0x05		//����ļ�¼��
#define	 DBERR_MEMEORY		0x06		//�ڴ����
#define  DBERR_NOTSUPPORT	0x07		//��֧��
#define  DBERR_NOMOREREC	0x08		//û�м�¼
#define  DBERR_NOTAG		0x09		//�޴˱�ǩ
#define  DBERR_TYPE			0x0A		//�����޷�ת�������
#define  DBERR_TIME			0x0B		//ʱ�����
#define  DBERR_NOVAL		0x0C		//û������
#define  DBERR_NOALL_OK		0x0D		//��ǩ������ȫ�ɹ�
#define  DBERR_BUF_FULL		0x0E		//�������������Ժ��ٲ�
#define  DBERR_NODRV		0X1E		//�޴�����

//�������������������
#define DBPE_OK				0x00	//�ɹ�
#define DBPE_ERR			0x01	//����
#define DBPE_NOTAG		  	0xA1	//��ǩ������
#define DBPE_TAGID_EXIST	0xA2	//ID����
#define DBPE_TAGNAME_EXIST	0xA3	//���ƴ���
#define DBPE_TAG_EXIST		0xA4	//ID�����ƴ���
#define DBPE_TYPE			0xA5	//���ʹ���
#define DBPE_MODIFYTYPE     0xA6	//�����޸�����
#define DBPE_MEMORY			0xA7	//�ڴ����
#define DBPE_NODB			0xA8	//�����ݿ�
#define DBPE_CONBUSY		0xA9	//���ݿ�����æ
#define DBPE_MANING			0xAA    //���ݿ������
#define DBPE_DBCONID		0xAB    //���ݿ�����ID����
#define DBPE_TIMEOUT		0xAC	//��ʱ
#define DBPE_NODATA			0xAD	//������
#define DBPE_WORKBUSY		0xAE	//���ݿ⹤��æ
#define DBPE_INVALID_SESSION	0xAF	//��ЧSession��Ҳ�����Ƕ�ӦSession�ı�ǩ�Ѿ���ɾ������Ҫ��������

//��ǩֵ�������
#define  ERRTV_OK		DBERR_OK	//�ɹ�
#define  ERRTV_NOTAG	DBERR_NOTAG	//�޴˱�ǩ
#define  ERRTV_TYPE		DBERR_TYPE	//���ʹ���������޷�ת��
#define  ERRTV_TIME		DBERR_TIME	//ʱ�����
#define  ERRTV_ERR		DBERR_FAIL  //��ǩֵ�������Ч
#define  ERRTV_BUF_FULL	DBERR_BUF_FULL //�������������Ժ��ٲ�

// ��ȫ���������� 
#define DS_OK				0		//
#define DS_ERR				1		//ʧ��
#define DS_ACTOR_FULL		0x20	//��ɫ��
#define DS_OPERATOR_FULL	0x21	//�û���
#define DS_TRUSTIP_FULL		0x22	//����������
#define DS_NOACTOR			0x23	//�޴˽�ɫ
#define DS_NOOPERATOR		0x24	//�޴��˺�
#define DS_NOTRUSTIP		0x25	//�޴�IP
#define DS_ACTOR_EXIST		0x26	//��ɫ����
#define DS_OPERATOR_EXIST   0x27	//�˺Ŵ���
#define DS_TRUSTIP_EXIST    0x28	//IP����
#define DS_NOPOWER			0x29	//�޴�Ȩ��
#define DS_PASSERR			0x2A	//�������
#define DS_USERLOCK			0x2B	//�˺Ž���
#define DS_ACTORUSE			0x2C	//��ɫ��ʹ��


//���������API�������
#define APIE_OK				0x00
//#define APIE_ERR			0x01
#define APIE_HANDLE			0xC0		//����ľ��
#define APIE_CONNECT		0xC1		//û������
#define APIE_MEMORY			0xC2		//API�ڴ����
#define APIE_NOMOREBUF      0xC3		//��������������
#define APIE_ERR			0xC4		//�¶���ֵ����DBPE_ERR����

#endif /* __DBPAPI_ERR_H_ */