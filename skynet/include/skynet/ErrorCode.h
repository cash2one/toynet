/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ErrorCode.h
//  - 에러 코드 정의
//
#if !defined(INCLUDED_ERRORCODE)
#define INCLUDED_ERRORCODE


namespace skynet
{

/// 에러 코드
typedef int ErrorCode;

#define DEFINE_ERRORCODE static const ErrorCode

/// 알 수 없는 에러
DEFINE_ERRORCODE EC_UNKNOWN = -1;

/// 성공
DEFINE_ERRORCODE EC_OK = 0;


/// 성공?
inline bool isSucceeded(ErrorCode ec) {
    return ec == EC_OK;
}

/// 실패?
inline bool isFailed(ErrorCode ec) {
    return ec != EC_OK;
}

} // namespace skynet

#endif // !defined(INCLUDED_ERRORCODE)
