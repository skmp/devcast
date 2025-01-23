/*
	This file is part of libswirl
*/
#include "license/bsd"


#include "types.h"

#include "imgread_common.h"


Disc* iso_parse(const wchar* file)
{
	// if not ending in .iso return 0
	if (strlen(file) < 4 || strcmp(file + strlen(file) - 4, ".iso"))
		return 0;

	core_file* fsource=core_fopen(file);

	if (!fsource)
		return 0;

	Disc* rv= new Disc();

	{
		Session s;
		s.StartFAD=150;
		s.FirstTrack=1;
		rv->sessions.push_back(s);	
	}

	{
		Track t;
		t.ADDR=1;//hmm is that ok ?

		t.CTRL=0;
		t.StartFAD=150;
		t.EndFAD=0;
		t.file = new RawTrackFile(core_fopen(file),0,t.StartFAD,2352);

		rv->tracks.push_back(t);
	}

	{
		Session s;
		s.StartFAD=150 + 11702;
		s.FirstTrack=2;
		rv->sessions.push_back(s);	
	}

	{
		Track t;
		t.ADDR=1;

		t.CTRL=4;
		t.StartFAD=150 + 11702;
		t.EndFAD=0;
		t.file = new RawTrackFile(core_fopen(file),0,t.StartFAD,2048);

		rv->tracks.push_back(t);
	}

	core_fclose(fsource);

	rv->type=CdRom_XA;

	rv->LeadOut.StartFAD=rv->EndFAD;
	rv->LeadOut.ADDR=0;
	rv->LeadOut.CTRL=0;

	return rv;
}