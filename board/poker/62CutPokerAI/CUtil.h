

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                       
// File : CUtil.h                                                       
//                                                                       
// Ŭ���� ������  : 1999.02.24                                           
//                                                                       
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _CUTIL_H_
#define _CUTIL_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CUtil
{

private:

public:
	CUtil()  {};
	~CUtil() {};


	int  CUtil::Get_FileNum( char *pFilePathName )
	{
		int nFindFileNum =  0;
		WIN32_FIND_DATA fd;	
		HANDLE hFind;	
		BOOL bRet = TRUE;

		//���� ã�� ����.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//������ ������ -1;
		if( hFind == INVALID_HANDLE_VALUE )		return -1;

		//������ �� �ִ� �� ó���� ����Ѵ�.
		while( hFind != INVALID_HANDLE_VALUE && bRet )
		{
			//���� �̸�.
			if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				//���� ��.
				nFindFileNum++;
			}

			//���� ������ ã�´�.
			bRet = FindNextFile( hFind, &fd );
		}

		//ã�Ƴ� ���� �ݴ´�.
		FindClose( hFind );

		return nFindFileNum;
	}

	int  CUtil::Get_DirectoryNum( char *pFilePathName )
	{
		int nFindDirectoryNum = 0;
		WIN32_FIND_DATA fd;	
		HANDLE hFind;	
		BOOL bRet = TRUE;

		//���� ã�� ����.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//������ ������ -1;
		if(hFind == INVALID_HANDLE_VALUE) return -1;

		//������ �� �ִ� �� ó���� ����Ѵ�.
		while( hFind != INVALID_HANDLE_VALUE && bRet )
		{
			//���丮 �̸�.
			if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				//���丮�� ��.
				nFindDirectoryNum++;
			}

			//���� ������ ã�´�.
			bRet = FindNextFile( hFind, &fd );
		}

		//ã�Ƴ� ���� �ݴ´�.
		FindClose( hFind );

		//���ϸ� �ִ°��.
		if( nFindDirectoryNum == 0 ) return -1;
		// .  .. �� ���丮�� ����.
		return ( nFindDirectoryNum -= 2 );
	}

	//	������ �ִ��� �˻�
	bool CUtil::IsPolderOrFileName( char *pFilePathName )
	{
		WIN32_FIND_DATA fd;	
		HANDLE hFind;	

		//	���� ã�� ����.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//	������ ������ -1;
		if( hFind == INVALID_HANDLE_VALUE )		return false;

		FindClose( hFind );

		return true;
	}


	//	����̺��, ������, ���ϸ�, Ȯ���� �и�... ������ ������ �������� ���ϸ� ����
	void CUtil::SplitPath( char *pFilePathName, char *pDrive, char *pDir, char *pFileName, char *pFileExt )
	{
		//	����̺��, ����, �����̸�, Ȯ���� �и��ϴ°�
		TCHAR szDrive[_MAX_DRIVE] = { 0, };
		TCHAR szDir[_MAX_DIR] = { 0, };
		TCHAR szFileName[200] = { 0, };
		TCHAR szFileExt[20] = { 0, };
		_tsplitpath( pFilePathName, szDrive, szDir,szFileName, szFileExt );

		strcpy( pDrive, szDrive );
		strcpy( pDir, szDrive );
		strcpy( pFileName, szDrive );
		strcpy( pFileExt, szDrive );
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //_CUTIL_H_


