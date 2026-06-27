#include "inventory.hpp"

void InventoryLayout::build(const sf::Vector2u& windowsSize, bool open)
{
	slots.clear();

	float hotbarWidth = (Inventory::HOTBAR * slotSize) + (Inventory::HOTBAR - 1) * padding;
	float hotbarStartX = (windowsSize.x - hotbarWidth) / 2.f;
	float hotbarStartY = windowsSize.y - slotSize - (padding * 3.f);

	for (int i = 0; i < Inventory::HOTBAR; i++)
	{
		slotUI slot;
		slot.rect.position.x = hotbarStartX + i * (slotSize + padding);
		slot.rect.position.y = hotbarStartY;
		slot.rect.size.x = slotSize;
		slot.rect.size.y = slotSize;

		slot.inventoryIndex = i;
		slot.isHotbar = true;
		slot.index = i;

		slots.push_back(slot);
	}

	if (open)
	{
		float invWidth = (Inventory::COLUMNS * slotSize) + ((Inventory::COLUMNS - 1) * padding);
		float invHeight = (Inventory::ROWS * slotSize) + ((Inventory::ROWS - 1) * padding);

		float invStartX = (windowsSize.x - invWidth) / 2.f;
		float invStartY = (windowsSize.y - invHeight) / 2.f;

		for (int i = 0; i < Inventory::ROWS; i++)
		{
			for (int j = 0; j < Inventory::COLUMNS; j++)
			{
				slotUI slot;
				slot.rect.position.x = invStartX + j * (slotSize + padding);
				slot.rect.position.y = invStartY + i * (slotSize + padding);
				slot.rect.size.x = slotSize;
				slot.rect.size.y = slotSize;

				slot.inventoryIndex = Inventory::HOTBAR + (i * Inventory::COLUMNS + j);
				slot.isHotbar = false;
				slot.index = i * Inventory::COLUMNS + j;
				slots.push_back(slot);
			}
		}


	}

}