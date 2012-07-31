
#include "util/wgt2allg.h"
#include "Common/core/out.h"
#include "Common/gui/guimanager.h"
#include "Common/util/stream.h"
#include "Engine/core/agsengine.h"
#include "Engine/core/engine_setup.h"
#include "Engine/game/agsgame.h"
#include "Engine/game/assets_manager.h"
#include "Engine/game/_character.h"
#include "Engine/game/_dialogtopic.h"
#include "Engine/game/_room.h"
#include "Engine/game/_view.h"
#include "Engine/platform/base/agsplatformdriver.h"
#include "media/audio/audio.h"
#include "script/exports.h"
#include "script/script.h"
#include "script/script_runtime.h"

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

    CAGSEngine::SetEIP(-17);

    err = LoadGameData();
    if (!err.IsNil())
    {
        // TODO: actually move these to error handling system later
        //proper_exit=1;
        platform->FinishedUsingGraphicsMode(); // TODO: this one goes to engine shutdown

        if (err.GetErrorCodeOrSomething() == -1)
            platform->DisplayAlert("Main game file not found. This may be from a different AGS version, or the file may have got corrupted.\n");
        else if (err.GetErrorCodeOrSomething() == -2)
            platform->DisplayAlert("Invalid file format. The file may be corrupt, or from a different\n"
            "version of AGS.\nThis engine can only run games made with AGS 2.5 or later.\n");
        else if (err.GetErrorCodeOrSomething() == -3)
            platform->DisplayAlert("Script link failed: %s\n",ccErrorString);
        return Err::FromCode(EXIT_NORMAL);
    }

    CAGSEngine::SetEIP(-11);

    err = InitGameObjects();
    if (!err.IsNil())
    {
        return err;
    }

    RegisterScriptObjects();

    CAGSEngine::SetEIP(-23);

    platform->StartPlugins();

    CAGSEngine::SetEIP(-24);

    err = InitScripts();
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
    _globalMessages.Set(id, message);
}

CString CAGSGame::GetGlobalMessage(int32_t id)
{
    CString s;
    _globalMessages.Find(id, s);
    return s;
}

HErr CAGSGame::SetPlayerCharacter(int32_t char_id)
{
    _game.playercharacter = char_id;
    _playerChar = &_chars[char_id];
    _scPlayerCharPtr = ccGetObjectHandleFromAddress((const char*)&_playerChar->GetInfo());
    return Err::Nil();
}

HErr CAGSGame::InitGameState()
{
    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();

    // init general state
    _play.recording      = 0;
    _play.playback       = 0;
    _play.takeover_data  = 0;

    return Err::Nil();
}

HErr CAGSGame::InitRooms()
{
    Out::Notify("Initializing rooms");
    // Not needed here since using dynamic arrays
    /*
    roomstats=(RoomStatus*)calloc(sizeof(RoomStatus),MAX_ROOMS);
    for (int i=0;i<MAX_ROOMS;i++) {
        
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
    Out::Notify("Load game data");

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

    if (gameBase->numfonts == 0)
    {
        return Err::FromCode(-2);  // old v2.00 version
    }

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

HErr CAGSGame::InitGameObjects()
{
    HErr err = LoadFonts();
    if (!err.IsNil())
    {
        return err;
    }

    // TODO: move elsewhere
    wtexttransparent(TEXTFG);
    _play.fade_effect = _game.options[OPT_FADETYPE];
    return Err::Nil();
}

void CAGSGame::RegisterScriptObjects()
{
    RegisterScriptAudio();
    RegisterScriptCharacters();
    RegisterScriptDialogs();
    RegisterScriptDialogOptionsRenderer();
    RegisterScriptGUIs();
    RegisterScriptHotspots();
    RegisterScriptInventoryItems();
    RegisterScriptRegions();
    RegisterScriptRoomObjects();

    ccAddExternalSymbol("character",&_game.chars[0]);
    SetPlayerCharacter(_game.playercharacter);
    ccAddExternalSymbol("player", &_scPlayerCharPtr);
    ccAddExternalSymbol("object",&_scrObj[0]);
    ccAddExternalSymbol("gui",&_scrGui[0]);
    ccAddExternalSymbol("hotspot",&_scrHotspot[0]);
    ccAddExternalSymbol("region",&_scrRegion[0]);
    ccAddExternalSymbol("inventory",&_scrInv[0]);
    ccAddExternalSymbol("dialog", &_scrDialog[0]);
}

HErr CAGSGame::InitScripts()
{
    ccSetScriptAliveTimer(150000);
    ccSetStringClassImpl(&_myScriptStringImpl);

    if (create_global_script())
    {
        return Err::FromCode(-3);
    }

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
        //old_dialog_scripts = (unsigned char**)malloc(_game.numdialog * sizeof(unsigned char**));

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
    SetDefaultGlobalMessage (983, "Sorry, not now.");
    SetDefaultGlobalMessage (984, "Restore");
    SetDefaultGlobalMessage (985, "Cancel");
    SetDefaultGlobalMessage (986, "Select a game to restore:");
    SetDefaultGlobalMessage (987, "Save");
    SetDefaultGlobalMessage (988, "Type a name to save as:");
    SetDefaultGlobalMessage (989, "Replace");
    SetDefaultGlobalMessage (990, "The save directory is full. You must replace an existing game:");
    SetDefaultGlobalMessage (991, "Replace:");
    SetDefaultGlobalMessage (992, "With:");
    SetDefaultGlobalMessage (993, "Quit");
    SetDefaultGlobalMessage (994, "Play");
    SetDefaultGlobalMessage (995, "Are you sure you want to quit?");
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

void CAGSGame::RegisterScriptAudio()
{
    int i;
    for (i = 0; i <= _scrAudioChannel.GetCount(); ++i) 
    {
        _scrAudioChannel[i].id = i;
        ccRegisterManagedObject(&_scrAudioChannel[i], &_ccDynamicAudio);
    }

    for (i = 0; i < _scrAudioClips.GetCount(); ++i)
    {
        _scrAudioClips[i].id = i;
        ccRegisterManagedObject(&_scrAudioClips[i], &_ccDynamicAudioClip);
        ccAddExternalSymbol(_scrAudioClips[i].scriptName, &_scrAudioClips[i]);
    }

    calculate_reserved_channel_count();
}

void CAGSGame::RegisterScriptCharacters()
{
    for (int i = 0; i < _chars.GetCount(); ++i)
    {
        // TODO: move to CCharacter init or something
        CharacterInfo &chinfo = _chars[i].GetInfo();
        chinfo.walking = 0;
        chinfo.animating = 0;
        chinfo.pic_xoffs = 0;
        chinfo.pic_yoffs = 0;
        chinfo.blinkinterval = 140;
        chinfo.blinktimer = chinfo.blinkinterval;
        chinfo.index_id = i;
        chinfo.blocking_width = 0;
        chinfo.blocking_height = 0;
        chinfo.prevroom = -1;
        chinfo.loop = 0;
        chinfo.frame = 0;
        chinfo.walkwait = -1;
        ccRegisterManagedObject(&chinfo, &_ccDynamicCharacter);

        // export the character's script object
        //characterScriptObjNames[i] = (char*)malloc(strlen(chinfo.scrname) + 5);
        _characterScriptObjNames[i] = chinfo.scrname;

        ccAddExternalSymbol(characterScriptObjNames[i], &chinfo);
    }
}

void CAGSGame::RegisterScriptDialogs()
{
    for (int i = 0; i < _scrDialog.GetCount(); ++i)
    {
        _scrDialog[i].id = i;
        _scrDialog[i].reserved = 0;

        ccRegisterManagedObject(&_scrDialog[i], &_ccDynamicDialog);

        if (!_dialogScriptNames[i].IsEmpty())
            ccAddExternalSymbol(const_cast<char*>(_dialogScriptNames[i].GetCStr()), &_scrDialog[i]);
    }
}

void CAGSGame::RegisterScriptDialogOptionsRenderer()
{
    ccRegisterManagedObject(&_ccDialogOptionsRendering, &_ccDialogOptionsRendering);

    _dialogOptionsRenderingSurface = new ScriptDrawingSurface();
    _dialogOptionsRenderingSurface->isLinkedBitmapOnly = true;
    long dorsHandle = ccRegisterManagedObject(_dialogOptionsRenderingSurface, _dialogOptionsRenderingSurface);
    ccAddObjectReference(dorsHandle);
}

void CAGSGame::RegisterScriptGUIs()
{
    int i;

    //scrGui = (ScriptGUI*)malloc(sizeof(ScriptGUI) * _game.numgui);
    _scrGui.SetCount(_game.numgui);
    for (i = 0; i < _scrGui.GetCount(); ++i)
    {
        _scrGui[i].gui = NULL;
        _scrGui[i].id = -1;
    }

//    guiScriptObjNames = (char**)malloc(sizeof(char*) * _game.numgui);
    _guiScriptObjNames.SetCount(_game.numgui);
    for (i = 0; i < _guiManager.GetGUICount(); ++i)
    {
        GUIMain *gui_main = _guiManager.GetGUI(i);
        // export all the GUI's controls
        RegisterScriptGUIControls(gui_main);

        // copy the script name to its own memory location
        // because ccAddExtSymbol only keeps a reference
        //guiScriptObjNames[i] = (char*)malloc(21);
        //strcpy(guiScriptObjNames[i], guis[i].name);
        _guiScriptObjNames[i] = gui_main->name;

        _scrGui[i].gui = gui_main;
        _scrGui[i].id = i;

        ccAddExternalSymbol(const_cast<char*>(_guiScriptObjNames[i].GetCStr()), &_scrGui[i]);
        ccRegisterManagedObject(&_scrGui[i], &_ccDynamicGUI);
    }
}

void CAGSGame::RegisterScriptGUIControls(GUIMain *gui_main)
{
    for (int i = 0; i < gui_main->numobjs; ++i) {
        if (gui_main->objs[i]->scriptName[0] != 0)
            ccAddExternalSymbol(gui_main->objs[i]->scriptName, gui_main->objs[i]);

        ccRegisterManagedObject(gui_main->objs[i], &_ccDynamicGUIObject);
    }
}

void CAGSGame::RegisterScriptHotspots()
{
    for (int i = 0; i < _scrHotspot.GetCount(); ++i) {
        _scrHotspot[i].id = i;
        _scrHotspot[i].reserved = 0;

        ccRegisterManagedObject(&_scrHotspot[i], &_ccDynamicHotspot);
    }
}

void CAGSGame::RegisterScriptInventoryItems()
{
    for (int i = 0; i < _scrInv.GetCount(); ++i) {
        _scrInv[i].id = i;
        _scrInv[i].reserved = 0;

        ccRegisterManagedObject(&_scrInv[i], &_ccDynamicInv);

        if (!_invScriptNames[i].IsEmpty())
            ccAddExternalSymbol(const_cast<char*>(_invScriptNames[i].GetCStr()), &_scrInv[i]);
    }
}

void CAGSGame::RegisterScriptRegions()
{
    for (int i = 0; i < _scrRegion.GetCount(); ++i) {
        _scrRegion[i].id = i;
        _scrRegion[i].reserved = 0;

        ccRegisterManagedObject(&_scrRegion[i], &_ccDynamicRegion);
    }
}

void CAGSGame::RegisterScriptRoomObjects()
{
    for (int i = 0; i < _scrObj.GetCount(); ++i) {
        ccRegisterManagedObject(&_scrObj[i], &_ccDynamicObject);
    }
}

HErr CAGSGame::LoadFonts()
{
    CAGSEngine::SetEIP(-22);
    for (int i = 0; i < _game.numfonts; ++i)
    {
        int fontsize = _game.fontflags[i] & FFLG_SIZEMASK;
        if (fontsize == 0)
            fontsize = 8;

        if ((_game.options[OPT_NOSCALEFNT] == 0) && (_game.default_resolution > 2))
            fontsize *= 2;

        if (!wloadfont_size(i, fontsize))
        {
            return Err::FromFormatString("Unable to load font %d, no renderer could load a matching file", i);
        }
    }

    return Err::Nil();
}

//ccRegisterManagedObject(&dummygui, NULL);
//ccRegisterManagedObject(&dummyguicontrol, NULL);



} // namespace Game
} // namespace Engine
} // namespace AGS
