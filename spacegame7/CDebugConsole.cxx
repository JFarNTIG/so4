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
#include "CDebugConsole.hxx"
#include "Util.hxx"
#include <sstream>
#include <iterator>

#define CONSOLE_MAXIMUM_LINES 80

CDebugConsole::CDebugConsole()
	: m_uiLinesCount(0), m_pListHead(nullptr), m_pListTail(nullptr), m_bConsoleUpdated(false), m_uiConsoleTextLines(0), m_szConsoleText("")
{

}

CDebugConsole::~CDebugConsole()
{
	this->clear();
}

void CDebugConsole::console_write_line(std::string const &text)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->write_line(text);
}

void CDebugConsole::console_issue_command(std::string const &text)
{
	this->m_mFieldAccess.lock();

	std::stringstream ss(text);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> command_tokenized(begin, end);

	if(command_tokenized.size() == 0)
	{
		return;
	}

	auto i = this->m_commandMap.find(command_tokenized[0]);

	if(i != this->m_commandMap.end())
	{
		ConsoleCommandCallback cb = i->second;

		this->m_mFieldAccess.unlock();

		cb(command_tokenized);

		this->m_mFieldAccess.lock();
	}
	else
	{
		std::string unknownCommandText = "Unknown command \'" + command_tokenized[0] + "\'";

		this->write_line(unknownCommandText);
	}

	this->m_mFieldAccess.unlock();
}

void CDebugConsole::console_get_latest_n_lines(int const lines, std::string &text)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(lines == this->m_uiConsoleTextLines && this->m_bConsoleUpdated == false)
	{
		text = this->m_szConsoleText;
	}
	else
	{
		std::string consoleText("");

		ConsoleLine* pLine = this->m_pListTail;

		for(int i = 0; i < lines; ++i)
		{
			std::string lineText;

			if(pLine == nullptr)
			{
				break;
			}

			lineText = pLine->m_szLineText;

			if(i == 0)
			{
				consoleText = lineText;
			}
			else
			{
				consoleText = lineText + "\n" + consoleText;
			}
			
			if(pLine)
			{
				pLine = pLine->m_pListPrev;
			}
		}

		//cache the console text in the event of a future request with the same line count
		this->m_szConsoleText = consoleText;
		this->m_uiConsoleTextLines = lines;
		this->m_bConsoleUpdated = false;

		text = consoleText;
	}
}

void CDebugConsole::console_clear()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->clear();
}

void CDebugConsole::console_register_command(std::string const &command, ConsoleCommandCallback const &callback)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_commandMap[command] = callback;
}

void CDebugConsole::clear()
{
	ConsoleLine* pLine = this->m_pListHead;

	while(pLine != nullptr)
	{
		ConsoleLine* pLineNext = pLine->m_pListNext;

		delete pLine;

		pLine = pLineNext;
	}

	this->m_bConsoleUpdated = true;
}

void CDebugConsole::write_line(std::string const &text)
{
	ConsoleLine* pLine = new ConsoleLine;

	pLine->m_szLineText = text;

	if(this->m_pListHead == nullptr && this->m_pListTail == nullptr)
	{
		pLine->m_pListNext = nullptr;
		pLine->m_pListPrev = nullptr;

		this->m_pListHead = pLine;
		this->m_pListTail = pLine;
	}
	else
	{
		pLine->m_pListNext = nullptr;
		pLine->m_pListPrev = this->m_pListTail;

		this->m_pListTail->m_pListNext = pLine;
		this->m_pListTail = pLine;
	}

	//If we are over the maximum lines, "trim" the list by deleting the head, aka the oldest line
	if(this->m_uiLinesCount > CONSOLE_MAXIMUM_LINES)
	{
		ConsoleLine* pOldHead = this->m_pListHead;

		this->m_pListHead = pOldHead->m_pListNext;

		delete pOldHead;
	}
	else
	{
		++this->m_uiLinesCount;
	}

	this->m_bConsoleUpdated = true;
}