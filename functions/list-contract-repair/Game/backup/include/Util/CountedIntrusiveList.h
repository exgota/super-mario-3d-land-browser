#ifndef GAME_UTIL_COUNTED_INTRUSIVE_LIST_H
#define GAME_UTIL_COUNTED_INTRUSIVE_LIST_H

#include <stddef.h>
#include <nn/types.h>

// Reconstructed from the EU binary. These are descriptive storage names,
// not recovered NintendoWare, SDK, or sead class identities.
namespace reconstructed {

struct IntrusiveListNode {
    IntrusiveListNode* next;
    IntrusiveListNode* previous;
};

struct CountedIntrusiveList {
    u32 count;
    IntrusiveListNode sentinel;
};

// Distinct observed owners share the node storage, not a claimed base class.
// These prefixes do not describe the complete original objects.
struct CallbackListEntryPrefix {
    const void* dispatchTable;
    IntrusiveListNode link;
};

struct ResourceListEntryPrefix {
    const void* dispatchTable;
    IntrusiveListNode link;
};

typedef char IntrusiveListNodeSize[(sizeof(IntrusiveListNode) == 8) ? 1 : -1];
typedef char IntrusiveListPreviousOffset[(offsetof(IntrusiveListNode, previous) == 4) ? 1 : -1];
typedef char CountedIntrusiveListSize[(sizeof(CountedIntrusiveList) == 12) ? 1 : -1];
typedef char CountedIntrusiveListSentinelOffset[(offsetof(CountedIntrusiveList, sentinel) == 4) ? 1 : -1];
typedef char CallbackListLinkOffset[(offsetof(CallbackListEntryPrefix, link) == 4) ? 1 : -1];
typedef char ResourceListLinkOffset[(offsetof(ResourceListEntryPrefix, link) == 4) ? 1 : -1];

} // namespace reconstructed

// Insert node before position and increment list->count. Only three inputs
// are consumed. The original return type is unknown; this void projection
// is for the audited callers that discard the machine result.
extern "C" void fn_002323E8(reconstructed::CountedIntrusiveList* list,
                            reconstructed::IntrusiveListNode* position,
                            reconstructed::IntrusiveListNode* node);

extern "C" void fn_0022A6D4(reconstructed::CountedIntrusiveList* list,
                            reconstructed::CallbackListEntryPrefix* entry);
extern "C" void fn_002B5590(reconstructed::CountedIntrusiveList* list,
                            reconstructed::ResourceListEntryPrefix* entry);

#endif
