/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ErrorCode.h
//  - ���� �ڵ� ����
//
#if !defined(INCLUDED_ERRORCODE)
#define INCLUDED_ERRORCODE


namespace skynet
{

/// ���� �ڵ�
typedef int ErrorCode;

#define DEFINE_ERRORCODE static const ErrorCode

/// �� �� ���� ����
DEFINE_ERRORCODE EC_UNKNOWN = -1;

/// ����
DEFINE_ERRORCODE EC_OK = 0;


/// ����?
inline bool isSucceeded(ErrorCode ec) {
    return ec == EC_OK;
}

/// ����?
inline bool isFailed(ErrorCode ec) {
    return ec != EC_OK;
}

} // namespace skynet

#endif // !defined(INCLUDED_ERRORCODE)
