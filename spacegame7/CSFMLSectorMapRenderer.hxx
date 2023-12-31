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

#include "ISectorMapRenderer.hxx"
#include "SFMLIncludes.hxx"
#include <vector>
#include <mutex>

class CSFMLSectorMapRenderer : public ISectorMapRenderer
{
public:
	CSFMLSectorMapRenderer();
	~CSFMLSectorMapRenderer();

	virtual void initialize_map(Vector2f const&) final;
	virtual void add_map_object(MapObjectShape const, Vector2f const&, MapColor const&) final;
	virtual void add_map_zone_circular(Vector2f const&, float const, MapColor const&) final;
	virtual void add_map_zone_rectangular(Vector2f const&, Vector2f const&, MapColor const&) final;
	virtual void update_player_position(Vector2f const&) final;
	virtual void set_map_dimensions(Vector2f const&) final;
	virtual void finalize_map(void) final;
	virtual void clear_map(void) final;
	virtual void draw_map(void) final;
	virtual void* get_map_texture_handle(void) final;
	virtual void enable_player_icon(bool) final;

	const sf::Texture& get_map_texture(void);
private:
	Vector2f get_map_position_from_absolute(Vector2f const &);
	Vector2f get_absolute_position_from_map(Vector2f const &);
	float get_map_distance_from_absolute(float const);

	std::mutex m_mFieldAccess;

	struct MapObject
	{
		MapObjectShape shape;
		Vector2f position;
		MapColor color;
	};

	struct MapZoneCircular
	{
		Vector2f position;
		float radius;
		MapColor color;
	};

	struct MapZoneRectangular
	{
		Vector2f position;
		Vector2f dimensions;
		MapColor color;
	};

	Vector2f m_mapDimensions;
	Vector2f m_playerPosition;
	std::vector<MapObject> m_vMapObjects;
	std::vector<MapZoneCircular> m_vMapZonesCircular;
	std::vector<MapZoneRectangular> m_vMapZonesRectangular;

	sf::RenderTexture m_sfRenderTexture;
	bool m_bPlayerIconEnabled;
};