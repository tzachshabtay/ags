
//=============================================================================
//
// AGS Game class
//
// [IKM] 2012-07-11: draft version
//
//=============================================================================
#ifndef __AGS_EE_GAME__AGSGAME_H
#define __AGS_EE_GAME__AGSGAME_H

#include "Common/core/err.h"
#include "Common/gui/guimanager.h"
#include "Common/util/array.h"
#include "Common/util/map.h"
#include "Common/util/ptrarray.h"
#include "Common/util/string.h"
#include "Common/ac/gamesetupstruct.h"
#include "Engine/ac/gamestate.h"

#include "ac/draw.h"
#include "ac/lipsync.h"
#include "ac/movelist.h"
#include "script/cc_instance.h"
#include "ac/dynobj/all_dynamicclasses.h"
#include "ac/dynobj/all_scriptclasses.h"

namespace AGS
{

namespace Common
{
namespace GUI
{
    class CGUIManager;
} // namespace GUI
} // namespace Common

namespace Engine
{

// Forward declarations
namespace Graphics
{
class CBitmap;
} // namespace Graphics

namespace Game
{

// Forward declarations
class CRoom;
class CCharacter;
class CDialogTopic;
class CView;

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::GUI::CGUIManager;
using AGS::Common::Util::CArray;
using AGS::Common::Util::CMap;
using AGS::Common::Util::CPtrArray;
using AGS::Common::Util::CString;
using AGS::Engine::Graphics::CBitmap;

/*

Game data versions and changes:
-------------------------------

12 : 2.3 + 2.4

Versions above are incompatible at the moment.

19 : 2.5.1
22 : 2.5.5

Variable number of sprites.
24 : 2.5.6
25 : 2.6.0

Encrypted global messages and dialogs.
26 : 2.6.1
27 : 2.6.2

Script modules. Fixes bug in the inventory display.
31 : 2.7.0
32 : 2.7.2

Interactions are now scripts. The number for "not set" changed from 0 to -1 for
a lot of variables (views, sounds).
37 : 3.0 + 3.1.0

Dialogs are now scripts. New character animation speed.
39 : 3.1.1
40 : 3.1.2

Audio clips
41 : 3.2.0
42 : 3.2.1

*/

// TODO: Move this to Common perhaps
enum GameDataVersion
{
    kGameData_230   = 12,
    kGameData_240   = 12,
    kGameData_250   = 18,
    kGameData_251   = 19,
    kGameData_255   = 22,
    kGameData_256   = 24,
    kGameData_260   = 25,
    kGameData_261   = 26,
    kGameData_262   = 27,
    kGameData_270   = 31,
    kGameData_272   = 32,
    kGameData_300   = 37,
    kGameData_310   = 37,
    kGameData_311   = 39,
    kGameData_312   = 40,
    kGameData_320   = 41,
    kGameData_321   = 42
};

class CAGSGame
{
public:

    static const GameDataVersion LatestGameDataVersion;

    CAGSGame();
    ~CAGSGame();

    // TODO: this should be made const
    /*const*/ GameState &GetGameState() /*const*/;
    GameDataVersion     GetThisGameDataVersion() const;

    HErr    Initialize();
    void    Shutdown();

    void    SetGlobalMessage(int32_t id, const CString &message);
    CString GetGlobalMessage(int32_t id);

    HErr    SetPlayerCharacter(int32_t char_id);

protected:

    HErr    InitGameState();
    HErr    InitRooms();
    HErr    InitSpeech();
    HErr    LoadGameData();
    HErr    InitGameObjects();
    void    RegisterScriptObjects();
    HErr    InitScripts();

    CStream *OpenGameDataFile();
    HErr    ReadGameDataVersion(CStream *in);
    HErr    ReadScripts(CStream *in);
    HErr    ReadViews(CStream *in);
    HErr    ReadDialogs(CStream *in);
    HErr    ReadGUI(CStream *in);
    void    AllocGameObjects();
    void    SetDefaultGlobalMessages();
    void    SetDefaultGlobalMessage(int32_t id, const CString &message);
    void    SetScoreSound(int32_t sound_id);

    HErr    LoadFonts();

    void    RegisterScriptAudio();
    void    RegisterScriptCharacters();
    void    RegisterScriptDialogs();
    void    RegisterScriptDialogOptionsRenderer();
    void    RegisterScriptGUIs();
    void    RegisterScriptGUIControls(GUIMain *gui_main);
    void    RegisterScriptHotspots();
    void    RegisterScriptInventoryItems();
    void    RegisterScriptRegions();   
    void    RegisterScriptRoomObjects();

private:

    // Unfortunately it won't be that easy to get rid of those structs;
    // They are referenced from script directly by address, and Editor
    // wants to have them too.
    GameSetupStruct             _game;
    GameState                   _play;

    CArray<CRoom>               _roomStatus;
    CArray<SpeechLipSyncLine>   _splipsync;
    ccScript                    *_gameScript;
    ccScript                    *_dialogScriptsScript;
    CPtrArray<ccScript>         _scriptModules;
    CPtrArray<ccInstance>       _moduleInst;
    CPtrArray<ccInstance>       _moduleInstFork;
    CArray<CString>             _moduleRepExecAddr;
    CArray<CCharacter>          _chars;
    CArray<MoveList>            _moveList;
    CPtrArray<CBitmap>          _actSps;
    //CPtrArray<IDriverDependantBitmap> _actSpsBmp;
    CPtrArray<CBitmap>          _actSpsWb;
    //CPtrArray<IDriverDependantBitmap> _actSpsWbBmp;
    CArray<CachedActSpsData>    _actSpsWbCache;
    CArray<CView>               _views;
    CMap<int32_t, CString>      _globalMessages;
    CArray<CDialogTopic>        _dialogTopics;
    // Version 3.1.0 and lower dialog scripts
    CArray< CArray<byte> >      _dialogScripts310;
    CArray<CString>             _speechLines310;

    CGUIManager                 &_guiManager;
    CArray<ScriptObject>        _scrObj;
    CArray<ScriptGUI>           _scrGui;
    CArray<ScriptHotspot>       _scrHotspot;
    CArray<ScriptRegion>        _scrRegion;
    CArray<ScriptInvItem>       _scrInv;
    CArray<ScriptDialog>        _scrDialog;
    CArray<ScriptAudioChannel>  _scrAudioChannel;
    CArray<ScriptAudioClip>     _scrAudioClips;
    CCGUIObject                 _ccDynamicGUIObject;
    CCCharacter                 _ccDynamicCharacter;
    CCHotspot                   _ccDynamicHotspot;
    CCRegion                    _ccDynamicRegion;
    CCInventory                 _ccDynamicInv;
    CCGUI                       _ccDynamicGUI;
    CCObject                    _ccDynamicObject;
    CCDialog                    _ccDynamicDialog;
    ScriptDialogOptionsRendering _ccDialogOptionsRendering;
    ScriptDrawingSurface        *_dialogOptionsRenderingSurface;
    CCAudioChannel              _ccDynamicAudio;
    CCAudioClip                 _ccDynamicAudioClip;
    ScriptString                _myScriptStringImpl;
    CArray<CString>             _characterScriptObjNames;
    CArray<CString>             _dialogScriptNames;
    CArray<CString>             _guiScriptObjNames;
    CArray<CString>             _invScriptNames;

    GameDataVersion             _gameDataVersion;
    bool                        _isOldDataFile;
    int32_t                     _gameEngineRequiredVersion;

    bool                        _gamePaused;
    CCharacter                  *_playerChar;
    intptr_t                    _scPlayerCharPtr;
};

} // namespace Game
} // namespace Engine
} // namespace AGS

#endif // __AGS_EE_GAME__AGSGAME_H
