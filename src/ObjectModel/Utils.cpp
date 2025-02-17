/*
 * ObjectModel.cpp
 *
 *  Created on: 7 Sep 2020
 *      Author: manuel
 */

#include <ObjectModel/Utils.hpp>
#include "Axis.hpp"
#include "BedOrChamber.hpp"
#include "Spindle.hpp"
#include "Tool.hpp"
#include "ListHelpers.hpp"
#include "FlashData.hpp"

namespace OM
{
	void GetHeaterSlots(
			const size_t heaterIndex,
			Slots& slots,
			const SlotType slotType,
			const bool addTools)
	{
		if (addTools)
		{
			IterateToolsWhile(
				[&slots, &heaterIndex, &slotType](Tool*& tool, size_t) {
					if (tool->slot < MaxSlots)
					{
						if (nvData.GetHeaterCombineType() == HeaterCombineType::notCombined)
						{
							tool->IterateHeaters([&tool, &slots, &heaterIndex, &slotType](ToolHeater* th, size_t index) {
								switch (slotType)
								{
								case SlotType::panel:
									{
										const size_t subSlot = tool->slot + index;
										if (subSlot < MaxSlots && th->heaterIndex == (int) heaterIndex)
										{
											slots.Add(subSlot);
										}
									}
									break;
								case SlotType::pJob:
									{
										const size_t subSlot = tool->slotPJob + index;
										if (subSlot < MaxPendantTools && th->heaterIndex == (int) heaterIndex)
										{
											slots.Add(subSlot);
										}
									}
									break;
								}
							});
						}
						else
						{
							if (tool->heaters[0] != nullptr && tool->heaters[0]->heaterIndex == (int) heaterIndex)
							{
								switch (slotType)
								{
								case SlotType::panel:
									{
										if (tool->slot < MaxSlots)
										{
											slots.Add(tool->slot);
										}
									}
									break;
								case SlotType::pJob:
									{
										if (tool->slotPJob < MaxPendantTools)
										{
											slots.Add(tool->slotPJob);
										}
									}
									break;
								}
							}
						}
					}
					return tool->slot < MaxSlots;
				});
		}
	}
}
