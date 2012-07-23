#ifndef __AC_LIPSYNC_H
#define __AC_LIPSYNC_H

// Forward-declarations
namespace AGS
{
namespace Common
{
namespace Util
{
class CStream;
} // namespace Util
} // namespace Common
} // namespace AGS

// Using-declarations
using AGS::Common::Util::CStream;

struct SpeechLipSyncLine {
    char  filename[14];
    int  *endtimeoffs;
    short*frame;
    short numPhenomes;

    void Read(CStream *in);
    void Write(CStream *out);
};

#endif // __AC_LIPSYNC_H
