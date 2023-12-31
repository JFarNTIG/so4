/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "SGLib.hxx"
#include "ICharacterEntity.hxx"
#include "CharacterCreationPanel.hxx"
#include "CBaseTransitionState.hxx"

#include "MainMenuPanel.hxx"

int CharacterCreationPanel::m_iPanelInstances = 0;

void CharacterCreationPanel::render_panel(float const flDelta)
{
	static char szCharName[16] = "Recruit";
	static int iCurrentItem = 0;

	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Create Character", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::InputText("Name", szCharName, 16, ImGuiInputTextFlags_CharsNoBlank);

	ImGui::Combo("Loadout", &iCurrentItem, "Civilian\0Police\0Debug\0\0");

	if(ImGui::Button("Begin"))
	{
		IIntransientDataManager* pIDM = SG::get_intransient_data_manager();
		pIDM->clear_string_variables();

		ICharacterEntityManager *pEntityManager = pIDM->get_character_entity_manager();
		pEntityManager->clear_entities();
		pEntityManager->create_player_entity();

		IMatManager* pMatManager = SG::get_material_manager();
		pMatManager->reset();
		pMatManager->force_generation_tick();

		ICharacterEntity *pEntity = pEntityManager->get_player_character_entity();
		IEntityInventory *pInventory = pEntity->get_inventory();

		pInventory->set_money(1000);
		pInventory->set_metal(0);
		pInventory->clear_items();

		pEntity->set_name(std::string(szCharName));

		switch(iCurrentItem)
		{
		case 0:
			pEntity->set_level(1);
			pEntity->set_loadout({ 8, 8, 8, 8, 15, 0 });
			pEntity->set_ship(1);
			break;
		case 1:
			pEntity->set_level(1);
			pEntity->set_loadout({ 4, 4, 4, 4, 15, 0 });
			pEntity->set_ship(1);
			break;
		case 2:
			pInventory->set_money(100000);
			pEntity->set_level(400);
			pEntity->set_mana(pEntity->get_max_mana());
			pEntity->set_loadout({ 17, 17, 0, 0, 0, 0 });
			pEntity->set_ship(1);
			break;
		}

		SG::get_game_state_manager()->transition_game_state(new CSectorTransitionState(2, "", Vector2f(-1500.0f, 14000.0f)));
	}

	ImGui::Separator();

	if(ImGui::Button("Back"))
	{
		SG::get_interface_manager()->add_panel(new MainMenuPanel);

		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool CharacterCreationPanel::panel_active(void)
{
	return this->m_bPanelActive;
}