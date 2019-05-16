#pragma once 

//#include <afxTempl.h> 

class CIni 
{ 
private: 
	CString m_strFileName; 
public: 
	CIni(CString strFileName):m_strFileName(strFileName) 
	{ 
	} 
public: 
	//һ���Բ����� 
	BOOL SetFileName(LPCTSTR lpFileName); //�����ļ��� 
	CString GetFileName(void); //����ļ��� 
	BOOL SetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue,bool bCreate=true); //���ü�ֵ��bCreate��ָ����������δ����ʱ���Ƿ񴴽��� 
	CString GetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault=_T("")); //�õ���ֵ. 
	BOOL DelSection(LPCTSTR strSection); //ɾ������ 
	BOOL DelKey(LPCTSTR lpSection, LPCTSTR lpKey); //ɾ������ 
	
	
public: 
	//�߼������� 
	int GetSections(CStringArray& arrSection); //ö�ٳ�ȫ���Ķ��� 
	int GetKeyValues(CStringArray& arrKey,CStringArray& arrValue,LPCTSTR lpSection); //ö�ٳ�һ���ڵ�ȫ��������ֵ 
	
	BOOL DelAllSections(); 
	
};