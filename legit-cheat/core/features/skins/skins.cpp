#include "../../../dependencies/common_includes.hpp"
#include "../../../dependencies/utilities/json.hpp"
#include "../features.hpp"
#include "../../menu/menu.h"
#include <filesystem>
#include "parser.h"

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

void features::skins::run() {
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto active_weapon = local_player->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_player->get_weapons();
	for (int i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
		if (!weapon)
			continue;
		
		if (weapon->item_definition_index() == WEAPON_FRAG_GRENADE || weapon->item_definition_index() == WEAPON_FLASHBANG || weapon->item_definition_index() == WEAPON_C4 || weapon->item_definition_index() == WEAPON_MOLOTOV || weapon->item_definition_index() == WEAPON_DECOY || weapon->item_definition_index() == WEAPON_SMOKEGRENADE)
			continue;
		
		const auto knife_index = local_player->team() == team::team_ct ? WEAPON_KNIFE : WEAPON_KNIFE_T;
		const auto index = weapon->client_class()->class_id == class_ids::cknife ? knife_index : weapon->item_definition_index();
		auto test = weapon->fallback_paint_kit();

		if (weapon_skin_quality_list.find(index) != weapon_skin_quality_list.end()) {
			auto random_skin = std::get<2>(weapon_skin_quality_list[index].at(math::random_int(0, weapon_skin_quality_list[index].size())));
			weapon->fallback_paint_kit() = random_skin;
		}

		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->fallback_seed() = 661;
		weapon->item_id_high() = -1;
		weapon->fallback_wear() = 0;
	}
}

std::map <int, std::vector<std::tuple<std::string, std::string, int>>> features::skins::weapon_skin_quality_list =
{
	{WEAPON_CZ75A, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_DEAGLE, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_ELITE, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_FIVESEVEN, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_GLOCK, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_HKP2000, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_P250, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_REVOLVER, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_TEC9, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_USP_SILENCER, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_AK47, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_AUG, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_AWP, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_FAMAS, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_G3SG1, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_GALILAR, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_M4A1_SILENCER, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_M4A1, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_SCAR20, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_SG556, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_SSG08, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_MAC10, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_MP5SD, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_MP7, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_MP9, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_BIZON, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_P90, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_UMP45, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_MAG7, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_NOVA, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_SAWEDOFF, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_XM1014, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_M249, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_NEGEV, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_GYPSY_NOMAD, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_SKELETON, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_SURV, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_CORD, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_CSS, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_GYPSY_JACKKNIFE, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_STILETTO, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_WIDOWMAKER, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_URSUS, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_BAYONET, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_SURVIVAL_BOWIE, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_BUTTERFLY, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_FALCHION, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_FLIP, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_GUT, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_TACTICAL, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_KARAMBIT, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_M9_BAYONET, std::vector<std::tuple<std::string, std::string, int>>{}},
	{WEAPON_KNIFE_PUSH, std::vector<std::tuple<std::string, std::string, int>>{}},
};

std::list<int> skin_order =
{
	WEAPON_CZ75A,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_HKP2000,
	WEAPON_P250,
	WEAPON_REVOLVER,
	WEAPON_TEC9,
	WEAPON_USP_SILENCER,
	WEAPON_AK47,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR,
	WEAPON_M4A1_SILENCER,
	WEAPON_M4A1,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_MAC10,
	WEAPON_MP5SD,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_BIZON,
	WEAPON_P90,
	WEAPON_UMP45,
	WEAPON_MAG7,
	WEAPON_NOVA,
	WEAPON_SAWEDOFF,
	WEAPON_XM1014,
	WEAPON_M249,
	WEAPON_NEGEV,
	WEAPON_KNIFE_GYPSY_NOMAD,
	WEAPON_KNIFE_SKELETON,
	WEAPON_KNIFE_SURV,
	WEAPON_KNIFE_CORD,
	WEAPON_KNIFE_CSS,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_URSUS,
	WEAPON_BAYONET,
	WEAPON_KNIFE_SURVIVAL_BOWIE,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_FALCHION,
	WEAPON_KNIFE_FLIP,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_PUSH,
};

void features::skins::get_weapon_skins()
{
	std::pair<std::vector<std::string>, std::vector<int>> paint_kit_list;
	for (auto skin : parser_skins)
	{
		paint_kit_list.first.push_back(skin.name);
		paint_kit_list.second.push_back(skin.id);
	}

	for (auto& weapon : weapon_skin_quality_list) {
		for (auto& skin_list : weapon.second) {
			auto it = std::find(paint_kit_list.first.begin(), paint_kit_list.first.end(), std::get<0>(skin_list));
			auto index = std::distance(paint_kit_list.first.begin(), it);

			if (index != paint_kit_list.second.size()) {
				auto paint_kid_id = paint_kit_list.second.at(index);
				std::get<2>(skin_list) = paint_kid_id;
			}
		}
	}
}

void features::skins::load()
{
	PWSTR path_to_doc;
	std::filesystem::path path_to_main;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path_to_doc)))
	{
		path_to_main = path_to_doc;
		path_to_main /= "overflow";
		CoTaskMemFree(path_to_doc);
	}

	std::ifstream file{ path_to_main / "items.txt" };
	std::string weapon_skins_list;
	weapon_skins_list.append((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	weapon_skins_list.erase(0, 3); // removes weird 3 charchters at begginginggg ggegsgGseggsgs fuck

	for (auto& order : skin_order)
	{		
		auto& it = weapon_skin_quality_list[order];
		auto weapon_left = weapon_skins_list.find("["); // [___
		auto weapon_right = weapon_skins_list.find("]", weapon_left); // ___]
		if (weapon_left != std::string::npos && weapon_right != std::string::npos)
		{
			auto weapon = weapon_skins_list.substr(weapon_left + 1, weapon_right - weapon_left - 1);
			weapon_skins_list.erase(0, weapon_right + 1);

			auto opening_top = weapon_skins_list.find("{"); // {___
			auto opening_bottom = weapon_skins_list.find("}"); // {___
			auto skins_list = weapon_skins_list.substr(opening_top + 1, opening_bottom - opening_top - 1);
			skins_list.erase(0, 1); // removes empty line

			std::vector<std::tuple<std::string, std::string, int>> skins_list_vector;
			std::istringstream skins_stream{ skins_list };
			std::string line{};
			while (std::getline(skins_stream, line))
			{
				auto middle = line.find(":");
				auto skin_name = line.substr(0, middle);
				auto skin_quality = line.substr(middle + 1, line.length());
				skins_list_vector.push_back(std::tuple<std::string, std::string, int>{skin_name, skin_quality, 0});
			}

			it = skins_list_vector;
		}
	}

	file.close();

	get_weapon_skins();
}
