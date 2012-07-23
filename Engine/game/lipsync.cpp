
#include "ac/lipsync.h"
#include "Common/util/stream.h"
#include <stdlib.h>

void SpeechLipSyncLine::Read(CStream *in)
{
    numPhenomes = in->ReadInt16();
    in->Read(filename, 14);
    endtimeoffs = (int*)malloc(numPhenomes * sizeof(int));
    in->Read(endtimeoffs, sizeof(int) * numPhenomes);
    frame = (short*)malloc(numPhenomes * sizeof(short));
    in->Read(frame, sizeof(short) * numPhenomes);
}

void SpeechLipSyncLine::Write(CStream *out)
{

}
