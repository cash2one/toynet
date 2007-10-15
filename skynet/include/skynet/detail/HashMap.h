/**
* Copyright (c)
*
* @author
*/
// HashMap.h

#if !defined(INCLUDED_HASHMAP)
#define INCLUDED_HASHMAP
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h" // Needed for the lock



// Little helper class
template<class EXT_ID, class INT_ID>
class Hash_Map : 
        public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, 
        ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_Null_Mutex>
{};


#endif
