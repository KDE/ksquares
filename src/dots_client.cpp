/* Generated by GGZComm/ruby version 0.4 */
/* Protocol 'dots', version '4' */
/* Do NOT edit this file! */

#include "dots_client.h"
#include <kggznet/kggzraw.h>

#define ggz__seat_open 1

dots::dots()
: QObject()
{
	requirelink = 0;
	fd = -1;
	raw = NULL;
}

void dots::ggzcomm_reqsync(const reqsync& message)
{
	*raw << dotsOpcodes::message_reqsync;
}

void dots::ggzcomm_reqnewgame(const reqnewgame& message)
{
	*raw << dotsOpcodes::message_reqnewgame;
}

void dots::ggzcomm_sndmoveh(const sndmoveh& message)
{
	*raw << dotsOpcodes::message_sndmoveh;
	*raw << message.x;
	*raw << message.y;
}

void dots::ggzcomm_sndmovev(const sndmovev& message)
{
	*raw << dotsOpcodes::message_sndmovev;
	*raw << message.x;
	*raw << message.y;
}

void dots::ggzcomm_sndoptions(const sndoptions& message)
{
	*raw << dotsOpcodes::message_sndoptions;
	*raw << message.width;
	*raw << message.height;
}

void dots::ggzcomm_msgseat()
{
	msgseat message;
	*raw >> message.seat;
	emit signalNotification(dotsOpcodes::message_msgseat, message);
}

void dots::ggzcomm_msgplayers()
{
	msgplayers message;
	for(int i1 = 0; i1 < 2; i1++)
	{
		*raw >> message.seat[i1];
		if((message.seat[i1] != ggz__seat_open))
		{
			*raw >> message.player[i1];
		}
	}
	emit signalNotification(dotsOpcodes::message_msgplayers, message);
}

void dots::ggzcomm_msgmoveh()
{
	msgmoveh message;
	*raw >> message.nx;
	*raw >> message.ny;
	*raw >> message.s;
	for(int i1 = 0; i1 < message.s; i1++)
	{
		*raw >> message.x[i1];
		*raw >> message.y[i1];
	}
	emit signalNotification(dotsOpcodes::message_msgmoveh, message);
}

void dots::ggzcomm_msgmovev()
{
	msgmovev message;
	*raw >> message.nx;
	*raw >> message.ny;
	*raw >> message.s;
	for(int i1 = 0; i1 < message.s; i1++)
	{
		*raw >> message.x[i1];
		*raw >> message.y[i1];
	}
	emit signalNotification(dotsOpcodes::message_msgmovev, message);
}

void dots::ggzcomm_msggameover()
{
	msggameover message;
	*raw >> message.winner;
	emit signalNotification(dotsOpcodes::message_msggameover, message);
}

void dots::ggzcomm_reqmove()
{
	reqmove message;
	emit signalNotification(dotsOpcodes::message_reqmove, message);
}

void dots::ggzcomm_rspmove()
{
	rspmove message;
	*raw >> message.status;
	*raw >> message.s;
	for(int i1 = 0; i1 < message.s; i1++)
	{
		*raw >> message.x[i1];
		*raw >> message.y[i1];
	}
	emit signalNotification(dotsOpcodes::message_rspmove, message);
}

void dots::ggzcomm_sndsync()
{
	sndsync message;
	*raw >> message.turn;
	*raw >> message.score0;
	*raw >> message.score1;
	emit signalNotification(dotsOpcodes::message_sndsync, message);
}

void dots::ggzcomm_msgoptions()
{
	msgoptions message;
	*raw >> message.width;
	*raw >> message.height;
	emit signalNotification(dotsOpcodes::message_msgoptions, message);
}

void dots::ggzcomm_reqoptions()
{
	reqoptions message;
	emit signalNotification(dotsOpcodes::message_reqoptions, message);
}

void dots::ggzcomm_network_main()
{
	int opcode;

	*raw >> opcode;

	if(requirelink)
	{
		if(opcode != nextlink) emit signalError();
		requirelink = 0;
	}

	switch(opcode)
	{
		case dotsOpcodes::message_msgseat:
			ggzcomm_msgseat();
			break;
		case dotsOpcodes::message_msgplayers:
			ggzcomm_msgplayers();
			break;
		case dotsOpcodes::message_msgmoveh:
			ggzcomm_msgmoveh();
			break;
		case dotsOpcodes::message_msgmovev:
			ggzcomm_msgmovev();
			break;
		case dotsOpcodes::message_msggameover:
			ggzcomm_msggameover();
			break;
		case dotsOpcodes::message_reqmove:
			ggzcomm_reqmove();
			break;
		case dotsOpcodes::message_rspmove:
			ggzcomm_rspmove();
			break;
		case dotsOpcodes::message_sndsync:
			ggzcomm_sndsync();
			break;
		case dotsOpcodes::message_msgoptions:
			ggzcomm_msgoptions();
			break;
		case dotsOpcodes::message_reqoptions:
			ggzcomm_reqoptions();
			break;
	}
}

void dots::ggzcomm_set_fd(int usefd)
{
	fd = usefd;
	raw = new KGGZRaw();
	raw->setNetwork(fd);
}

void dots::ggzcomm_set_socket(KGGZRaw *useraw)
{
	raw = useraw;
}

