#include "Util/CountedIntrusiveList.h"

extern "C" void fn_0022A6D4(reconstructed::CountedIntrusiveList* list,
                            reconstructed::CallbackListEntryPrefix* entry) {
    reconstructed::IntrusiveListNode* position = &list->sentinel;
    reconstructed::IntrusiveListNode* node = &entry->link;
    fn_002323E8(list, position, node);
}

extern "C" void fn_002B5590(reconstructed::CountedIntrusiveList* list,
                            reconstructed::ResourceListEntryPrefix* entry) {
    reconstructed::IntrusiveListNode* position = &list->sentinel;
    reconstructed::IntrusiveListNode* node = &entry->link;
    fn_002323E8(list, position, node);
}
