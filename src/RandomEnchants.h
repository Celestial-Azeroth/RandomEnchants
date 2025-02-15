#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

enum ItemQuality
{
    Grey = 0,
    White,
    Green,
    Blue,
    Purple,
    Orange
};

void RollEnchant(Player* player, Item* item);
[[nodiscard]] uint32 GetRandomEnchantment(Item* item);

// Add player scripts
class RandomEnchantsPlayer : public PlayerScript
{
public:
    RandomEnchantsPlayer() : PlayerScript("RandomEnchantsPlayer") { }

    void OnLogin(Player* player) override;
    void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
    void OnCreateItem(Player* player, Item* item, uint32 count) override;
    void OnGroupRollRewardItem(Player* player, Item* item, uint32 count, RollVote voteType, Roll* roll) override;
};

// Add all scripts in one
void AddRandomEnchantsScripts()
{
    new RandomEnchantsPlayer();
}
