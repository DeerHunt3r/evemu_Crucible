/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2008 The EVEmu Team
	For the latest information visit http://evemu.mmoforge.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Bloody.Rabbit
*/

#ifndef __SHIP__H__INCL__
#define __SHIP__H__INCL__

#include "inventory/Type.h"
#include "inventory/InventoryItem.h"

/**
 * Basic container for raw ship type data.
 */
class ShipTypeData {
public:
	ShipTypeData(
		uint32 _weaponTypeID = 0,
		uint32 _miningTypeID = 0,
		uint32 _skillTypeID = 0
	);

	// Content:
	uint32 weaponTypeID;
	uint32 miningTypeID;
	uint32 skillTypeID;
};

/**
 * Class managing ship type data.
 */
class ShipType
: public Type
{
	friend class Type; // to let them construct us
public:
	/**
	 * Loads ship type.
	 *
	 * @param[in] factory
	 * @param[in] shipTypeID ID of ship type to load.
	 * @return Pointer to new ShipType object; NULL if failed.
	 */
	static ShipType *Load(ItemFactory &factory, uint32 shipTypeID) { return Type::_Load<ShipType>(factory, shipTypeID); }

	/*
	 * Access methods:
	 */
	const Type *weaponType() const { return m_weaponType; }
	const Type *miningType() const { return m_miningType; }
	const Type *skillType() const { return m_skillType; }

protected:
	ShipType(
		uint32 _id,
		// Type stuff:
		const Group &_group,
		const TypeData &_data,
		// ShipType stuff:
		const Type *_weaponType,
		const Type *_miningType,
		const Type *_skillType,
		const ShipTypeData &stData
	);

	/*
	 * Member functions:
	 */
	static ShipType *_Load(ItemFactory &factory, uint32 shipTypeID
	);
	static ShipType *_Load(ItemFactory &factory, uint32 shipTypeID,
		// Type stuff:
		const Group &group, const TypeData &data
	);
	static ShipType *_Load(ItemFactory &factory, uint32 shipTypeID,
		// Type stuff:
		const Group &group, const TypeData &data,
		// ShipType stuff:
		const Type *weaponType, const Type *miningType, const Type *skillType, const ShipTypeData &stData
	);

	virtual bool _Load(ItemFactory &factory) { return Type::_Load(factory); }

	/*
	 * Data content:
	 */
	const Type *m_weaponType;
	const Type *m_miningType;
	const Type *m_skillType;
};

/**
 * InventoryItem which represents ship.
 */
class Ship
: public InventoryItem
{
	friend class InventoryItem;	// to let it construct us
public:
	/**
	 * Loads ship from DB.
	 *
	 * @param[in] factory
	 * @param[in] shipID ID of ship to load.
	 * @param[in] recurse Whether all items contained within this item should be loaded.
	 * @return Pointer to Ship object; NULL if failed.
	 */
	static Ship *Load(ItemFactory &factory, uint32 shipID, bool recurse=false) { return InventoryItem::_Load<Ship>(factory, shipID, recurse); }
	/**
	 * Spawns new ship.
	 *
	 * @param[in] factory
	 * @param[in] data Item data for ship.
	 * @return Pointer to new Ship object; NULL if failed.
	 */
	static Ship *Spawn(ItemFactory &factory, ItemData &data);

	/*
	 * Primary public interface:
	 */
	Ship *IncRef() { return static_cast<Ship *>(InventoryItem::IncRef()); }

	/*
	 * Public fields:
	 */
	const ShipType &    type() const { return static_cast<const ShipType &>(InventoryItem::type()); }

protected:
	Ship(
		ItemFactory &_factory,
		uint32 _shipID,
		// InventoryItem stuff:
		const ShipType &_shipType,
		const ItemData &_data
	);

	/*
	 * Member functions
	 */
	static Ship *_Load(ItemFactory &factory, uint32 shipID
	);
	static Ship *_Load(ItemFactory &factory, uint32 shipID,
		// InventoryItem stuff:
		const ShipType &shipType, const ItemData &data
	);

	virtual bool _Load(bool recurse=false) { return InventoryItem::_Load(recurse); }

	static uint32 _Spawn(ItemFactory &factory,
		// InventoryItem stuff:
		ItemData &data
	);
};

#endif /* !__SHIP__H__INCL__ */


