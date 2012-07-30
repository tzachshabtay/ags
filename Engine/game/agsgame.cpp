
#include "util/wgt2allg.h"
#include "Common/core/out.h"
#include "Common/gui/guimanager.h"
#include "Common/util/stream.h"
#include "Engine/core/agsengine.h"
#include "Engine/core/engine_setup.h"
#include "Engine/game/agsgame.h"
#include "Engine/game/assets_manager.h"
#include "Engine/game/_dialogtopic.h"
#include "Engine/game/_room.h"
#include "Engine/game/_view.h"
#include "Engine/platform/base/agsplatformdriver.h"
#include "script/exports.h"
#include "media/audio/audio.h"

// from cc_error
extern char ccErrorString[400];
// from gui
extern int ifacepopped;

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
//using AGS::Common::Util::StreamSeek;
using AGS::Engine::Core::CAGSEngine;
using AGS::Engine::Core::CEngineSetup;
namespace Out = AGS::Common::Core::Out;
namespace Err = AGS::Common::Core::Err;
namespace StreamSeek = AGS::Common::Util;

/* static */ const GameDataVersion CAGSGame::LatestGameDataVersion = kGameData_321;

CAGSGame::CAGSGame()
    : _guiManager(*new CGUIManager())
{
}

CAGSGame::~CAGSGame()
{
}

/*const*/ GameState &CAGSGame::GetGameState() /*const*/
{
    return _play;
}

GameDataVersion CAGSGame::GetThisGameDataVersion() const
{
    return _gameDataVersion;
}

HErr CAGSGame::Initialize()
{
    HErr err = InitGameState();
    if (!err.IsNil())
    {
        return err;
    }

    err = InitRooms();
    if (!err.IsNil())
    {
        return err;
    }

    err = InitSpeech();
    if (!err.IsNil())
    {
        return err;
    }

    err = LoadGameData();
    if (!err.IsNil())
    {
        return err;
    }

    return Err::Nil();
}

void CAGSGame::Shutdown()
{

}

void CAGSGame::SetGlobalMessage(int32_t id, const CString &message)
{
    _globalMessages.Add(id, message);
}

CString CAGSGame::GetGlobalMessage(int32_t id)
{
    CString s;
    _globalMessages.Find(id, s);
    return s;
}

HErr CAGSGame::InitGameState()
{
    HErr err;
    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();

    // init general state
    _play.recording      = 0;
    _play.playback       = 0;
    _play.takeover_data  = 0;

    err = InitRooms();
    err = InitSpeech();

    Out::Notify("Load game data");

    CAGSEngine::SetEIP(-17);
    err = LoadGameData();
    if (err.GetErrorCodeOrSomething() != 0) {
        //proper_exit=1;
        platform->FinishedUsingGraphicsMode();

        if (err.GetErrorCodeOrSomething() == -1)
            platform->DisplayAlert("Main game file not found. This may be from a different AGS version, or the file may have got corrupted.\n");
        else if (err.GetErrorCodeOrSomething() == -2)
            platform->DisplayAlert("Invalid file format. The file may be corrupt, or from a different\n"
            "version of AGS.\nThis engine can only run games made with AGS 2.5 or later.\n");
        else if (err.GetErrorCodeOrSomething() == -3)
            platform->DisplayAlert("Script link failed: %s\n",ccErrorString);
        return Err::FromCode(EXIT_NORMAL);
    }

    return Err::Nil();
}

HErr CAGSGame::InitRooms()
{
    Out::Notify("Initializing rooms");
    // Not needed here since using dynamic arrays
    /*
    roomstats=(RoomStatus*)calloc(sizeof(RoomStatus),MAX_ROOMS);
    for (int ee=0;ee<MAX_ROOMS;ee++) {
        
    }
    */

    return Err::Nil();
}

HErr CAGSGame::InitSpeech()
{
    _play.want_speech=-2;

    CEngineSetup &setup = CAGSEngine::GetSetup();
    if (setup.NoSpeechPack != 0)
    {
        return Err::Nil();
    }

    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();
    
    CAssetsManager *assets      = CAGSEngine::GetAssetsManager();

    CStream *speechsync = assets->OpenAsset(assets->GetSpeechFileName(), "syncdata.dat", "rb");
    if (speechsync != NULL) {
        // this game has voice lip sync
        if (speechsync->ReadInt32() != 4)
        { 
            // Don't display this warning.
            // platform->DisplayAlert("Unknown speech lip sync format (might be from older or newer version); lip sync disabled");
        }
        else {
            int num_lip_lines = speechsync->ReadInt32();
            for (int i = 0; i < num_lip_lines; ++i)
            {
                _splipsync[i].Read(speechsync);
            }
        }
        delete speechsync;
    }
    _play.want_speech=1;
        //}
    //}

    return Err::Nil();
}

HErr CAGSGame::LoadGameData()
{
    // TODO: move somewhere else
    _gamePaused = 0;  // reset the game paused flag
    ifacepopped = -1;

    CStream *in = OpenGameDataFile();
    if (!in)
    {
        return Err::FromCode(-1);
    }

    CAGSEngine::SetEIP(-18);
    setup_script_exports();
    CAGSEngine::SetEIP(-16);

    HErr err = ReadGameDataVersion(in);
    if (!err.IsNil())
    {
        return err;
    }

    _game.charScripts = NULL;
    _game.invScripts = NULL;
    memset(&_game.spriteflags[0], 0, MAX_SPRITES);

    GameSetupStructBase *gameBase = (GameSetupStructBase *) &_game;
    gameBase->Read(in);

    if (_gameDataVersion <= kGameData_310) // <= 3.1
    {
        // Fix animation speed for old formats
        _game.options[OPT_OLDTALKANIMSPD] = 1;
    }

    //if (_game.numfonts > MAX_FONTS)
    //    quit("!This game requires a newer version of AGS. Too many fonts for this version to handle.");

    char saveGameSuffix[MAX_SG_EXT_LENGTH + 1];
    GameSetupStruct::GAME_STRUCT_READ_DATA read_data;
    read_data.filever        = _gameDataVersion;
    read_data.saveGameSuffix = saveGameSuffix;
    read_data.max_audio_types= MAX_AUDIO_TYPES;
    //read_data.game_file_name = game_file_name; //<--- redundant with CAssetsManager

    //-----------------------------------------------------
    _game.ReadFromStream_Part1(in, read_data);
    //-----------------------------------------------------
    if (_game.compiled_script == NULL)
    {
        return Err::FromString("No global script in game; data load error");
    }

    err = ReadScripts(in);
    if (!err.IsNil())
    {
        return err;
    }
    
    CAGSEngine::SetEIP(-15);

    AllocGameObjects();

    err = ReadViews(in);
    if (!err.IsNil())
    {
        return err;
    }

    CAGSEngine::SetEIP(-14);

    if (_gameDataVersion <= kGameData_251) // <= 2.1 skip unknown data /* [IKM] mean 19 == 2.5.1 ? */
    {
        int count = in->ReadInt32();
        in->Seek(StreamSeek::kSeekCurrent, count * 0x204);
    }

    //-----------------------------------------------------
    _game.ReadFromStream_Part2(in, read_data);
    //-----------------------------------------------------

    SetDefaultGlobalMessages();

    CAGSEngine::SetEIP(-13);

    err = ReadDialogs(in);
    if (!err.IsNil())
    {
        return err;
    }

    _guiManager.ReadGUI(in, _gameDataVersion, _game);

    if (_gameDataVersion >= kGameData_260) // >= 2.60
    {
        platform->ReadPluginsFromDisk(in);
    }

    //-----------------------------------------------------
    _game.ReadFromStream_Part3(in, read_data);
    //-----------------------------------------------------

    SetScoreSound(read_data.score_sound);

    delete in;
    return Err::Nil();
}

CStream *CAGSGame::OpenGameDataFile()
{
    CStream *in = CAGSEngine::GetAssetsManager()->OpenAsset("game28.dta", "rb"); // 3.x data file name
    if (!in) {
        in = CAGSEngine::GetAssetsManager()->OpenAsset("ac2game.dta", "rb"); // 2.x data file name
    }

    return in;
}

HErr CAGSGame::ReadGameDataVersion(CStream *in)
{
    char teststr[31];

    teststr[30]=0;
    in->Read(&teststr[0], 30);
    _gameDataVersion = (GameDataVersion)in->ReadInt32();

    if (_gameDataVersion < kGameData_321) {
        // Allow loading of 2.x+ datafiles
        if (_gameDataVersion < kGameData_250) // < 2.5.0
        {
            return Err::FromCode(-2);
        }
        _isOldDataFile = true;
    }

    int engineverlen = in->ReadInt32();
    char engineneeds[20];
    // MACPORT FIX 13/6/5: switch 'size' and 'nmemb' so it doesn't treat the string as an int
    in->Read(&engineneeds[0], sizeof(char) * engineverlen);
    engineneeds[engineverlen] = 0;

    if (_gameDataVersion > LatestGameDataVersion) {
        platform->DisplayAlert("This game requires a newer version of AGS (%s). It cannot be run.", engineneeds);
        return Err::FromCode(-2);
    }

    if (strcmp (engineneeds, CAGSEngine::GetEngineVersion().GetCStr()) > 0)
        platform->DisplayAlert("This game requires a newer version of AGS (%s). It may not run correctly.", engineneeds);

    {
        int major, minor;
        sscanf(engineneeds, "%d.%d", &major, &minor);
        _gameEngineRequiredVersion = 100 * major + minor;
    }

    //loaded_game_file_version = filever; // [IKM] seem redundant

    return Err::Nil();
}

HErr CAGSGame::ReadScripts(CStream *in)
{
    _gameScript = ccScript::CreateFromStream(in);
    if (!_gameScript)
    {
        return Err::FromString("Global script load failed; need newer version?");
    }

    if (_gameDataVersion > kGameData_310) // 3.1.1+ dialog script
    {
        _dialogScriptsScript = ccScript::CreateFromStream(in);
        if (!_dialogScriptsScript)
        {
            return Err::FromString("Dialog scripts load failed; need newer version?");
        }
    }
    else // 2.x and < 3.1.1 dialog
    {
        _dialogScriptsScript = NULL;
    }

    if (_gameDataVersion >= kGameData_270) // 2.7.0+ script modules
    {
        int num_script_modules = in->ReadInt32();

        for (int i = 0; i < num_script_modules; ++i) {
            ccScript *script = ccScript::CreateFromStream(in);
            if (!script)
            {
                return Err::FromString("Script module load failure; need newer version?");
            }
            _scriptModules.Add(script);
            // [IKM] TODO: not sure this is best way, probably it would be nicer
            // to have maps with script address or script's tag as a key
            _moduleInst.Add(NULL);
            _moduleInstFork.Add(NULL);
            _moduleRepExecAddr.Add("");
        }
    }

    return Err::Nil();
}

HErr CAGSGame::ReadViews(CStream *in)
{
    HErr err;
    _views.SetCount(_game.numviews);

    for (int i = 0; i < _game.numviews; ++i)
    {
        err = _views[i].Read(in, _gameDataVersion);
        if (!err.IsNil())
        {
            return err;
        }
    }

    return Err::Nil();
}

HErr CAGSGame::ReadDialogs(CStream *in)
{
    HErr err;

    _dialogTopics.SetCount(_game.numdialog);
    for (int i = 0; i < _game.numdialog; ++i)
    {
        err = _dialogTopics[i].Read(in);
        if (!err.IsNil())
        {
            return err;
        }
    }

    if (_gameDataVersion <= kGameData_310) // Dialog script
    {
        _dialogScripts310.SetCount(_game.numdialog);
        //old_dialog_scripts = (unsigned char**)malloc(game.numdialog * sizeof(unsigned char**));

        for (int i = 0; i < _game.numdialog; ++i)
        {
            //old_dialog_scripts[i] = (unsigned char*)malloc(dialog[i].codesize);
            DialogTopic &topic = _dialogTopics[i].GetDialogTopic();
            _dialogScripts310.SetCount(topic.codesize);
            in->Read(_dialogScripts310.arr, topic.codesize * sizeof(byte));
            //fread(old_dialog_scripts[i], dialog[i].codesize, 1, iii);

            // Skip encrypted text script
            unsigned int script_size = in->ReadInt32();
            in->Seek(StreamSeek::kSeekCurrent, script_size);
        }

        // Read the dialog lines
        //_speechLines310.SetCount()
        //old_speech_lines = (char**)malloc(10000 * sizeof(char**));
        if (_gameDataVersion <= kGameData_260)
        {
            // Plain text on <= 2.60
            char buffer[1000];
            bool end_reached = false;
            //int speech_line_index = 0;
            CString new_line;
            while (!end_reached)
            {
                char* nextchar = buffer;
                while (1)
                {
                    *nextchar = in->ReadInt8();
                    if (*nextchar == 0)
                    {
                        break;
                    }

                    if ((unsigned char)*nextchar == 0xEF)
                    {
                        end_reached = true;
                        in->Seek(StreamSeek::kSeekCurrent, -1);
                        break;
                    }
                    nextchar++;
                }

                if (end_reached)
                {
                    break;
                }

                //old_speech_lines[i] = (char*)malloc(strlen(buffer) + 1);
                //strcpy(old_speech_lines[i], buffer);
                new_line = buffer;
                _speechLines310.Add(new_line);
                //speech_line_index++;
            }
        }
        else
        {
            // Encrypted text on > 2.60
            //int speech_line_index = 0;
            CString new_line;
            while (1)
            {
                unsigned int newlen = in->ReadInt32();
                if (newlen == 0xCAFEBEEF)  // GUI magic
                {
                    in->Seek(StreamSeek::kSeekCurrent, -4);
                    break;
                }

                //old_speech_lines[i] = (char*)malloc(newlen + 1);
                //fread(old_speech_lines[i], newlen, 1, iii);
                //old_speech_lines[i][newlen] = 0;
                CString new_line;
                new_line.SetLength(newlen);
                in->Read(new_line.s, newlen * sizeof(char));
                decrypt_text(new_line.s);
                _speechLines310.Add(new_line);
                //speech_line_index++;
            }
        }
        //old_speech_lines = (char**)realloc(old_speech_lines, i * sizeof(char**));
    }

    return Err::Nil();
}

void CAGSGame::AllocGameObjects()
{
    _chars.SetCount(_game.numcharacters);
    _moveList.SetCount(_game.numcharacters + MAX_INIT_SPR + 1);

    int act_sps_count = _game.numcharacters + MAX_INIT_SPR + 2;
    _actSps.SetCount(act_sps_count);
    //_actSpsBmp.SetCount(act_sps_count);
    _actSpsWb.SetCount(act_sps_count);
    //_actSpsWbBmp.SetCount(act_sps_count);
    _actSpsWbCache.SetCount(act_sps_count);

    // FIXME: check if may use separate array
    _game.charProps = (CustomProperties*)calloc(_game.numcharacters, sizeof(CustomProperties));
}

void CAGSGame::SetDefaultGlobalMessages()
{
    SetGlobalMessage (983, "Sorry, not now.");
    SetGlobalMessage (984, "Restore");
    SetGlobalMessage (985, "Cancel");
    SetGlobalMessage (986, "Select a game to restore:");
    SetGlobalMessage (987, "Save");
    SetGlobalMessage (988, "Type a name to save as:");
    SetGlobalMessage (989, "Replace");
    SetGlobalMessage (990, "The save directory is full. You must replace an existing game:");
    SetGlobalMessage (991, "Replace:");
    SetGlobalMessage (992, "With:");
    SetGlobalMessage (993, "Quit");
    SetGlobalMessage (994, "Play");
    SetGlobalMessage (995, "Are you sure you want to quit?");
}

void CAGSGame::SetDefaultGlobalMessage(int32_t id, const CString &message)
{
    SetGlobalMessage(id - 500, message);
}

void CAGSGame::SetScoreSound(int32_t sound_id)
{
    if (_gameDataVersion >= 41) {
        _play.score_sound = sound_id;
    }
    else {
        _play.score_sound = -1;
        if (_game.options[OPT_SCORESOUND] > 0)
        {
            ScriptAudioClip* clip = get_audio_clip_for_old_style_number(false, _game.options[OPT_SCORESOUND]);
            if (clip)
            {
                _play.score_sound = clip->id;
            }
            else
            {
                _play.score_sound = -1;
            }
        }
    }
}


} // namespace Game
} // namespace Engine
} // namespace AGS
