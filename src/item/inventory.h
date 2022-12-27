#pragma once

namespace item {
    class inventory {
    public:
        static std::map<RE::TESBoundObject*, std::pair<int, std::unique_ptr<RE::InventoryEntryData>>> get_inventory_magic_items();
    };
}
