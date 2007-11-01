/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// CrashHandler.h
#if !defined(INCLUDED_CRASHHANDLER)
#define INCLUDED_CRASHHANDLER

namespace skynet
{

/**
 * BugSlayer CrashHandler(MiniCore Dump)를 설정한다
 * - 덤프파일은 dir\\filenamePrefix_pid.DMP 형식으로 저장된다
 * @parm dir 덤프파일을 기록할 디렉토리(끝에 \가 붙어야 한다)
 * @parm filenamePrefix 덤프파일명 앞에 붙을 접두어
 */
void initCrashHandler(const ACE_TCHAR* dir, const ACE_TCHAR* filenamePrefix);

} // namespace skynet

#endif // !defined(INCLUDED_CRASHHANDLER)
