#ifndef _MapUICursor_
#define _MapUICursor_
#include "..\Map.h"
#include "..\..\Utilities\UI_Element.h"
class Map;
class Map_UI_Cursor : public UI_Element
{
private:
	Map* p_Map;
	IDirect3DDevice9Ptr p_Device;
	ID3DXSprite* p_Sprite;
public:
	int x = 0;
	int y = 0;

	int Height = 60;
	bool Cursor_Show = true;
	enum CursorType ///Note: Unsure if this enumerator can differentiate from other indexes
	{
		None = 0,
		Object,
		Special,
		Invisible,
		Player,
		NPC,
		Vault,
		Shop,
		Board,
		Bank
	};
	struct CursorDat
	{
		CursorType _CType;
		int x, y;
		int index;
	};
	CursorDat cursordat;
	std::basic_string<wchar_t> cur_istring;
	std::basic_string<char> scur_istring;
	CursorType m_CursorType;
	Map_UI_Cursor();
	Map_UI_Cursor(VOID* m_Game, Map* m_Map, IDirect3DDevice9Ptr m_Device);
	~Map_UI_Cursor();
	void Update();
	void Render(ID3DXSprite* m_sprite, float depth);
};
#endif

