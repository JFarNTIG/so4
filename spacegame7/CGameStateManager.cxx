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
#include <mutex>
#include "CGameStateManager.hxx"
#include "SGLib.hxx"
#include "CLoadingScreenState.hxx"

CGameStateManager::CGameStateManager()
{
	this->m_currentState = nullptr;
	this->m_bLoading = false;

	this->m_loadingState = new CLoadingScreenState;
	this->m_loadingState->state_initializing();
}

CGameStateManager::~CGameStateManager()
{
	if(this->m_loadingState != nullptr)
	{
		delete this->m_loadingState;
	}
}

void CGameStateManager::transition_game_state(IGameState *pNewState)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_pendingState != nullptr)
	{
		throw SGException("attempt to transition game state with another state already pending");
	}

	this->m_pendingState = pNewState;
}

IGameState *CGameStateManager::get_game_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bLoading)
	{
		return this->m_loadingState;
	}

	return this->m_currentState;
}

bool CGameStateManager::has_pending_game_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_pendingState != nullptr;
}

void CGameStateManager::shift_to_pending_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_pendingState == nullptr)
	{
		throw SGException("attempt to shift game state to a null pending state");
	}

	if (this->m_currentState != nullptr)
	{
		this->m_currentState->state_terminating();

		delete this->m_currentState;
	}

	this->flush_semitransient_states();

	this->m_currentState = this->m_pendingState;
	this->m_pendingState = nullptr;

	this->m_currentState->state_initializing();
}

void CGameStateManager::flush_semitransient_states(void)
{
	ITransientStateStructure *transientStates[] =
	{
		SG::get_audio_manager(),
		SG::get_comms_manager(),
		SG::get_interface_manager(),
		SG::get_particle_manager(),
		SG::get_script_engine(),
		SG::get_world()
	};

	for (ITransientStateStructure *transientState : transientStates)
	{
		transientState->shifting_out();
	}
}

void CGameStateManager::notify_initiate_loading()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bLoading == true)
	{
		throw SGException("Duplicate call to CGameStateManager::notify_initiate_loading()");
	}

	this->m_bLoading = true;
}

void CGameStateManager::notify_loading_finished()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bLoading == false)
	{
		throw SGException("Duplicate call to CGameStateManager::notify_loading_finished()");
	}

	this->m_bLoading = false;
}