#include "RandomEnchants.h"

void RollEnchant(Player* player, Item* item)
{
    if (!sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true))
    {
        return;
    }

    uint32 itemQuality = item->GetTemplate()->Quality;
    uint32 itemClass = item->GetTemplate()->Class;
    uint32 itemLevel = item->GetTemplate()->ItemLevel;

    // If Item Quality is Grey (0) or out of scope (6) we skip
    if (itemQuality < 1 || itemQuality > 5)
    {
        return;
    }

    // If Item Class is not Weapon (2) or Armor (4) we skip
    if (itemClass != 2 && itemClass != 4)
    {
        return;
    }

    uint32 enchantChance = sConfigMgr->GetOption<float>("RandomEnchants.EnchantChance", 35.0f);

    // Roll if the item should get enchanted or not (0 - 100)
    if (rand_chance() > enchantChance)
    {
        return;
    }

    uint32 enchantID = GetRandomEnchantment(item);

    if (enchantID == -1)
    {
        return;
    }

    if (sSpellItemEnchantmentStore.LookupEntry(enchantID))
    {
        player->ApplyEnchantment(item, EnchantmentSlot(5), false);
        item->SetEnchantment(EnchantmentSlot(5), enchantID, 0, 0);
        player->ApplyEnchantment(item, EnchantmentSlot(5), true);

        ChatHandler chatHandle = ChatHandler(player->GetSession());

        chatHandle.PSendSysMessage("Aquired Enchanted Equipment! Received Enchant with ID  |cffFF0000 {} |r", enchantID);
    }

}

uint32 GetRandomEnchantment(Item* item)
{
    uint32 itemLevel = item->GetTemplate()->ItemLevel;

    auto GetEnchantTier = [](uint32 itemLevel, uint32 arraySize) -> uint32
        {
            if (itemLevel <= 0) return 0;
            if (itemLevel >= 60) return arraySize - 1;

            float normalizedLevel = (itemLevel / 60.0f) * (arraySize - 1);
            return static_cast<uint32>(normalizedLevel);
        };

    // At the moment the length of the arrays are all the same...
   // So we can just do it here
    uint32 enchantTier = GetEnchantTier(itemLevel, 10);
    float rnd = rand_chance();

    // Stamina
    if (rnd < 20)
    {
        // 1, 3, 5, 7, 9
        static const std::vector<uint32> staminaEnchants = { 71, 72, 73, 102, 103, 104, 105, 353, 356, 361 };
        return staminaEnchants[enchantTier];

        // Custom System (not working yet)
        //return (50000 + enchantTier);
    }
    else if (rnd < 40) // Strength
    {
        static const std::vector<uint32> strengthEnchants = { 68, 69, 70, 106, 107, 108, 109, 352, 357, 362 };
        return strengthEnchants[enchantTier];

        // Custom System (not working yet)
        //return (50010 + enchantTier);
    }
    else if (rnd < 60)
    {
        static const std::vector<uint32> agilityEnchants = { 74, 75, 76, 90, 91, 92, 93, 343, 349, 358 };
        return agilityEnchants[enchantTier];

    }
    else if (rnd < 80)
    {
        static const std::vector<uint32> intellectEnchants = { 79, 80, 81, 94, 95, 96, 97, 350, 354, 359 };
        return intellectEnchants[enchantTier];
    }
    else
    {
        static const std::vector<uint32> spiritEnchants = { 82, 83, 84, 98, 99, 100, 101, 351, 355, 360 };
        return spiritEnchants[enchantTier];
    }

    return -1;
}

void RandomEnchantsPlayer::OnPlayerLogin(Player* player)
{
    bool announceOnLogin = sConfigMgr->GetOption<bool>("RandomEnchants.AnnounceOnLogin", true);
    bool enabled = sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true);

    if (announceOnLogin && enabled)
    {
        ChatHandler(player->GetSession()).SendSysMessage("Random Enchants is Enabled.");
    }
}

// When Item gets Looted
void RandomEnchantsPlayer::OnPlayerLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid)
{
    bool onLoot = sConfigMgr->GetOption<bool>("RandomEnchants.OnLoot", true);
    bool enabled = sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true);

    if (onLoot && enabled)
    {
        RollEnchant(player, item);
    }
}

// When Item gets Crafted
void RandomEnchantsPlayer::OnPlayerCreateItem(Player* player, Item* item, uint32 count)
{
    bool onCraft = sConfigMgr->GetOption<bool>("RandomEnchants.OnCraft", true);
    bool enabled = sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true);

    if (onCraft && enabled)
    {
        RollEnchant(player, item);
    }
}

// When Item gets rolled for
void RandomEnchantsPlayer::OnPlayerGroupRollRewardItem(Player* player, Item* item, uint32 count, RollVote voteType, Roll* roll)
{
    bool onGroupRoll = sConfigMgr->GetOption<bool>("RandomEnchants.OnGroupRoll", true);
    bool enabled = sConfigMgr->GetOption<bool>("RandomEnchants.Enable", true);

    if (onGroupRoll && enabled)
    {
        RollEnchant(player, item);
    }
}
