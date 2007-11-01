/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// CrashHandler.h
#if !defined(INCLUDED_CRASHHANDLER)
#define INCLUDED_CRASHHANDLER

namespace skynet
{

/**
 * BugSlayer CrashHandler(MiniCore Dump)�� �����Ѵ�
 * - ���������� dir\\filenamePrefix_pid.DMP �������� ����ȴ�
 * @parm dir ���������� ����� ���丮(���� \�� �پ�� �Ѵ�)
 * @parm filenamePrefix �������ϸ� �տ� ���� ���ξ�
 */
void initCrashHandler(const ACE_TCHAR* dir, const ACE_TCHAR* filenamePrefix);

} // namespace skynet

#endif // !defined(INCLUDED_CRASHHANDLER)
