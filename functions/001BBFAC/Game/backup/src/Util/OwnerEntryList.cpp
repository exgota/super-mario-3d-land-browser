namespace observed_owner_entries {
struct OwnerPrefix {
    unsigned char unknown00[0x10];
    unsigned identity;
    unsigned blocking;
};
struct FlagProviderPrefix { unsigned value; };
struct EntryPrefix {
    unsigned key0, key1;
    OwnerPrefix* activeOwner;
    OwnerPrefix* owner;
    FlagProviderPrefix* provider;
};
struct ListPrefix {
    unsigned char unknown00[0xc];
    int count;
    EntryPrefix** entries;
    unsigned flags;
};
static_assert(offsetof(OwnerPrefix, identity) == 0x10, "owner identity");
static_assert(offsetof(OwnerPrefix, blocking) == 0x14, "owner blocking word");
static_assert(offsetof(EntryPrefix, activeOwner) == 8, "active owner");
static_assert(offsetof(EntryPrefix, provider) == 0x10, "flag provider");
static_assert(offsetof(ListPrefix, count) == 0xc, "active count");
static_assert(offsetof(ListPrefix, entries) == 0x10, "entry pointer array");
}
extern "C" bool fn_00332660(const observed_owner_entries::OwnerPrefix*,
                            const observed_owner_entries::OwnerPrefix*);
extern "C" bool fn_0033466C(const observed_owner_entries::FlagProviderPrefix*);

extern "C" void fn_001BBFAC(observed_owner_entries::ListPrefix* list,
                           observed_owner_entries::OwnerPrefix* owner) {
    using namespace observed_owner_entries;
    EntryPrefix* selected = 0;
    for (int i = 0; i < list->count; ++i) {
        if (list->entries[i]->owner == owner) {
            selected = list->entries[i];
            break;
        }
    }
    if (!selected) return;
    unsigned oldKey0 = selected->key0;
    unsigned oldKey1 = selected->key1;
    for (int i = 0; i < list->count; ++i) {
        EntryPrefix* entry = list->entries[i];
        if (entry->key0 == oldKey0 && entry->key1 == oldKey1) {
            for (int j = i; j < list->count - 1; ++j)
                list->entries[j] = list->entries[j + 1];
            if (entry->activeOwner && i < list->count - 1)
                list->entries[i]->activeOwner = list->entries[i]->owner;
            --list->count;
            list->entries[list->count] = entry;
            break;
        }
    }
    unsigned key0 = selected->key0;
    unsigned key1 = selected->key1;
    FlagProviderPrefix* provider = selected->provider;
    int insertionIndex = list->count;
    for (int i = 0; i < list->count; ++i)
        if (list->entries[i]->key0 == key0 && list->entries[i]->key1 == key1)
            return;
    bool activate = false;
    if (owner) {
        if (owner->blocking) {
            activate = true;
        } else {
            bool conflict = false;
            insertionIndex = 0;
            for (; insertionIndex < list->count; ++insertionIndex) {
                OwnerPrefix* active = list->entries[insertionIndex]->activeOwner;
                if (active) {
                    if (conflict) break;
                    if (fn_00332660(owner, active)) conflict = true;
                }
            }
            if (insertionIndex >= list->count) {
                activate = !conflict;
                insertionIndex = list->count;
            }
        }
    }
    EntryPrefix* entry = list->entries[list->count];
    entry->owner = owner;
    entry->key0 = key0;
    entry->key1 = key1;
    entry->activeOwner = activate ? owner : 0;
    entry->provider = provider;
    if (provider) list->flags |= fn_0033466C(provider);
    for (int i = list->count - 1; i >= insertionIndex; --i)
        list->entries[i + 1] = list->entries[i];
    list->entries[insertionIndex] = entry;
    ++list->count;
}
