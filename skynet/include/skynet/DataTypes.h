/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// DataTypes.h
//  - WarNet 전반에 걸쳐 사용될 데이터 타입 선언
//
#if !defined(INCLUDED_DATATYPES)
#define INCLUDED_DATATYPES

#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#pragma warning ( pop )

namespace skynet
{

typedef std::vector<std::string> StringList;

} // namespace skynet

#endif // !defined(INCLUDED_DATATYPES)
