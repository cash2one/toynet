

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                       
// File : CUtil.h                                                       
//                                                                       
// 클레스 제작일  : 1999.02.24                                           
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

		//파일 찾기 시작.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//파일이 없으면 -1;
		if( hFind == INVALID_HANDLE_VALUE )		return -1;

		//파일이 더 있는 한 처리를 계속한다.
		while( hFind != INVALID_HANDLE_VALUE && bRet )
		{
			//파일 이면.
			if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				//파일 수.
				nFindFileNum++;
			}

			//다음 파일을 찾는다.
			bRet = FindNextFile( hFind, &fd );
		}

		//찾아낸 것을 닫는다.
		FindClose( hFind );

		return nFindFileNum;
	}

	int  CUtil::Get_DirectoryNum( char *pFilePathName )
	{
		int nFindDirectoryNum = 0;
		WIN32_FIND_DATA fd;	
		HANDLE hFind;	
		BOOL bRet = TRUE;

		//파일 찾기 시작.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//파일이 없으면 -1;
		if(hFind == INVALID_HANDLE_VALUE) return -1;

		//파일이 더 있는 한 처리를 계속한다.
		while( hFind != INVALID_HANDLE_VALUE && bRet )
		{
			//디렉토리 이면.
			if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				//디렉토리수 수.
				nFindDirectoryNum++;
			}

			//다음 파일을 찾는다.
			bRet = FindNextFile( hFind, &fd );
		}

		//찾아낸 것을 닫는다.
		FindClose( hFind );

		//파일만 있는경우.
		if( nFindDirectoryNum == 0 ) return -1;
		// .  .. 이 디렉토리는 뺀다.
		return ( nFindDirectoryNum -= 2 );
	}

	//	파일이 있는지 검사
	bool CUtil::IsPolderOrFileName( char *pFilePathName )
	{
		WIN32_FIND_DATA fd;	
		HANDLE hFind;	

		//	파일 찾기 시작.
		hFind = FindFirstFile( pFilePathName, &fd );
		
		//	파일이 없으면 -1;
		if( hFind == INVALID_HANDLE_VALUE )		return false;

		FindClose( hFind );

		return true;
	}


	//	드라이브명, 폴더명, 파일명, 확장자 분리... 파일이 없을땐 마지만이 파일명에 들어간다
	void CUtil::SplitPath( char *pFilePathName, char *pDrive, char *pDir, char *pFileName, char *pFileExt )
	{
		//	드라이브명, 폴더, 파일이름, 확장자 분리하는것
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


