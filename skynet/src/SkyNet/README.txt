//여기서 에러가 난다. 그럼 2byte가 아니라는건데. 정확한 사유는 알지 못함. 
AceUtil.cpp
BOOST_STATIC_ASSERT(sizeof(ACE_WSTRING_TYPE) == 2);
