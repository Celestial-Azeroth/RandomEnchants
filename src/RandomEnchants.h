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

    void OnPlayerLogin(Player* player) override;
    void OnPlayerLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
    void OnPlayerCreateItem(Player* player, Item* item, uint32 count) override;
    void OnPlayerGroupRollRewardItem(Player* player, Item* item, uint32 count, RollVote voteType, Roll* roll) override;
};

// Add all scripts in one
void AddRandomEnchantsScripts()
{
    new RandomEnchantsPlayer();
}
