#include "GameUtility.h"

using namespace app;

std::vector<PlayerControl*> GetAllPlayers()
{
	std::vector<PlayerControl*> players = std::vector<PlayerControl*>();
	auto playerList = (*PlayerControl__TypeInfo)->static_fields->AllPlayerControls;

	for (int i = 0; i < List_1_PlayerControl__get_Count(playerList, NULL); i++)
		players.push_back(List_1_PlayerControl__get_Item(playerList, i, NULL));

	return players;
}