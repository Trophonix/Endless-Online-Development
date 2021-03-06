#include "..\stdafx.h"
#include "SPaperdoll.h"
#include "..\Game.h"

void SPaperdoll::SendPaperdollRequest(pt::ipstream* ClientStream, int PlayerID,LPVOID game)
{
	Game* gme = (Game*)game;
	PacketBuilder builder = PacketBuilder(PACKET_PAPERDOLL, PACKET_REQUEST);
	builder.AddShort(PlayerID);
	World::Send(gme,ClientStream,builder);
}

void SPaperdoll::SendPaperdollRemove(pt::ipstream* ClientStream, int RemoveID, int SubID, LPVOID game)
{
	Game* gme = (Game*)game;
	PacketBuilder builder = PacketBuilder(PACKET_PAPERDOLL, PACKET_REMOVE);
	builder.AddShort(RemoveID);
	builder.AddChar(SubID);
	World::Send(gme, ClientStream, builder);
}

void SPaperdoll::SendPaperdollAdd(pt::ipstream* ClientStream, int AddID, int SubID, LPVOID game)
{
	Game* gme = (Game*)game;
	PacketBuilder builder = PacketBuilder(PACKET_PAPERDOLL, PACKET_ADD);
	builder.AddShort(AddID);
	builder.AddChar(SubID);
	World::Send(gme, ClientStream, builder);
}