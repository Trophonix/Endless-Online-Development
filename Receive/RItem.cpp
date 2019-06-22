#include "Handler.h"


CLIENT_F_FUNC(Item)
{	
		switch(action)
		{		
			case PACKET_GET: 
				{
					short UID = reader.GetShort();
					game->map->RemoveItem(UID);
					Map_UI_Inventory::InventoryItem newitem;
					newitem.id= reader.GetShort();
					newitem.amount = reader.GetThree();
					int weight = reader.GetChar();
					int maxweight = reader.GetChar();
					game->map->m_Players[World::WorldCharacterID]->weight = weight;
					game->map->m_Players[World::WorldCharacterID]->maxweight = maxweight;
					game->Map_UserInterface->map_inventory->AddItem(newitem);
					
					break;
				}
			case PACKET_DROP:
			{
				Map::Map_Item m_item;
				m_item.ItemID = reader.GetShort();
				m_item.amount = reader.GetThree();
				int CharacterHasItem = reader.GetInt();
				int ItemIndex = reader.GetShort();
				m_item.x = reader.GetChar();
				m_item.y = reader.GetChar();
				int weight = reader.GetChar();
				int maxweight = reader.GetChar();

				//if (CharacterHasItem == 0)
				{
					Map_UI_Inventory::InventoryItem invitem;
					invitem.amount = m_item.amount;
					invitem.id = m_item.ItemID;
					game->Map_UserInterface->map_inventory->RemoveItem(invitem);
					game->map->m_Players[World::WorldCharacterID]->weight = weight;
					game->map->m_Players[World::WorldCharacterID]->maxweight = maxweight;
				}

				game->map->AddItem(ItemIndex, m_item);

				break;
			}
			default:
				return false;
		}
		return true;
}