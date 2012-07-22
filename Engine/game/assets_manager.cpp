
#include "Common/core/out.h"
#include "Common/platform/base/platform_api.h"
#include "Common/util/path.h"
#include "Engine/core/agsengine.h"
#include "Engine/game/assets_manager.h"
#include "ac/common_defines.h"
#include "platform/base/agsplatformdriver.h"

// from clib32
extern "C" int cfopenpriority;
extern char *ci_find_file(char *dir_name, char *file_name);
extern "C"
{
    extern int csetlib(char *namm, char *passw);
    extern FILE *clibfopen(char *, char *);
    extern long cliboffset(char *);
    extern long clibfilesize(char *);
    extern long last_opened_size;
}

namespace AGS
{
namespace Engine
{
namespace Game
{

// Using-declarations
using AGS::Engine::Core::CAGSEngine;
namespace Out = AGS::Common::Core::Out;
namespace Err = AGS::Common::Core::Err;
namespace Path = AGS::Common::Util::Path;

CAssetsManager::CAssetsManager(CEngineSetup &setup, const CString &data_file_name, const CString &data_directory)
    : _theSetup(setup)
    , _gameDataFileName(data_file_name)
    , _gameDataDirectory(data_directory)
{
}

CAssetsManager::~CAssetsManager()
{
}

HErr CAssetsManager::RegisterGameData()
{
    Out::Notify("Initializing game data");

    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();
    CAGSEngine *engine = CAGSEngine::GetInstance();

    cfopenpriority=2; // Clib32

    // initialize the data file
    if (!SetGameDataFileName())
    {
        return Err::FromCode(EXIT_NORMAL);
    }

    int errcod = csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");  // assume it's appended to exe

    engine->SetEIP(-194);
    //  char gamefilenamebuf[200];

    HErr err;
    if ((errcod!=0) && (!_theSetup.MustChangeToGameDataDirectory)) {
        // it's not, so look for the file
        errcod = InitGameDataExternal();
    }
    else {
        // set the data filename to the EXE name
        err = InitGameDataInternal();
    }

    if (errcod!=0) {  // there's a problem
        if (errcod==-1) {  // file not found
            CString msg;
            msg.Format("You must create and save a game first in the AGS Editor before you can use "
                "this engine.\n\n"
                "If you have just downloaded AGS, you are probably running the wrong executable.\n"
                "Run AGSEditor.exe to launch the editor.\n\n"
                "(Unable to find '%s')\n", _gameDataFileName);
            platform->DisplayAlert(msg.GetCStr());
        }
        else if (errcod==-4)
            platform->DisplayAlert("ERROR: Too many files in data file.");
        else platform->DisplayAlert("ERROR: The file is corrupt. Make sure you have the correct version of the\n"
            "editor, and that this really is an AGS game.\n");
        return Err::FromCode(EXIT_NORMAL);
    }

    if (!err->IsNil()) {
        return err;
    }

    engine->SetEIP(-193);
    return Err::Nil();
}

CString CAssetsManager::GetDataFileName() const
{
    return _gameDataFileName;
}

CString CAssetsManager::GetDataDirectory() const
{
    return _gameDataDirectory;
}

bool CAssetsManager::SetGameDataFileName()
{
#ifdef WINDOWS_VERSION
    //WCHAR buffer[MAX_PATH];
    //LPCWSTR dataFilePath = wArgv[datafile_argv];
    // Hack for Windows in case there are unicode chars in the path.
    // The normal argv[] array has ????? instead of the unicode chars
    // and fails, so instead we manually get the short file name, which
    // is always using ANSI chars.
    CString buffer;
    if (!Path::IsPath(_gameDataFileName))
    {
        _gameDataFileName = Path::MakePath(Path::GetCurrentDirectory(), _gameDataFileName);
    }

    //if (wcschr(dataFilePath, '\\') == NULL)
    //{
    //    GetCurrentDirectoryW(MAX_PATH, buffer);
    //    wcscat(buffer, L"\\");
    //    wcscat(buffer, dataFilePath);
    //    dataFilePath = &buffer[0];
    //}
    _gameDataFileName = Path::GetShortPath(_gameDataFileName);
    if (_gameDataFileName.IsEmpty())
    {
        platform->DisplayAlert("Unable to determine startup path: GetShortPathNameW failed. The specified game file might be missing.");
        return false;
    }

#elif defined(PSP_VERSION) || defined(ANDROID_VERSION) || defined(IOS_VERSION)
    _gameDataFileName = psp_game_file_name;
#else
    //game_file_name = (char*)malloc(MAX_PATH);
    //strcpy(game_file_name, get_filename(global_argv[datafile_argv]));
    _gameDataFileName = Path::GetFileName(_gameDataFileName);
#endif
    return true;
}

int CAssetsManager::InitGameDataExternal()
{
    // [IKM] If I understand this right, this method is called only when the game file name was specified
    // in cmdline, but not found
    int errcod = 0;
    _gameDataFileName = ci_find_file(const_cast<char*>(_theSetup.DataFilesDir.GetCStr()),
        const_cast<char*>(_theSetup.MainDataFilename.GetCStr()));

#if !defined(WINDOWS_VERSION) && !defined(PSP_VERSION) && !defined(ANDROID_VERSION) && !defined(IOS_VERSION)
    // Search the exe files for the game data
    if ((_gameDataFileName.IsEmpty()) || (access(_gameDataFileName.GetCStr(), F_OK) != 0))
    {
        DIR* fd = NULL;
        struct dirent* entry = NULL;
        version_info_t version_info;

        if ((fd = opendir(".")))
        {
            while ((entry = readdir(fd)))
            {
                // Exclude the setup program
                if (stricmp(entry->d_name, "winsetup.exe") == 0)
                    continue;

                // Filename must be >= 4 chars long
                int length = strlen(entry->d_name);
                if (length < 4)
                    continue;

                if (stricmp(&(entry->d_name[length - 4]), ".exe") == 0)
                {
                    if (!getVersionInformation(entry->d_name, &version_info))
                        continue;
                    if (strcmp(version_info.internal_name, "acwin") == 0)
                    {
                        _gameDataFileName = entry->d_name;
                        break;
                    }
                }
            }
            closedir(fd);
        }
    }
#endif

    errcod=csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");
    if (errcod) {
        //sprintf(gamefilenamebuf,"%s\\ac2game.ags",_theSetup.data_files_dir);
        _gameDataFileName = ci_find_file(const_cast<char*>(_theSetup.DataFilesDir.GetCStr()), "ac2game.ags");
        errcod = csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");
    }

    return errcod;
}

HErr CAssetsManager::InitGameDataInternal()
{
    // [IKM] .... is it all about this?
    _theSetup.MainDataFilename = _gameDataFileName;
    if (_theSetup.DataFilesDir.Compare(".") == 0)
    {
        _theSetup.DataFilesDir = _gameDataDirectory;
    }

    /*
    if (((strchr(game_file_name, '/') != NULL) ||
    (strchr(game_file_name, '\\') != NULL)) &&
    (stricmp(_theSetup.data_files_dir, ".") == 0)) {
    // there is a path in the game file name (and the user
    // has not specified another one)
    // save the path, so that it can load the VOX files, etc
    _theSetup.data_files_dir = (char*)malloc(strlen(game_file_name) + 1);
    strcpy(_theSetup.data_files_dir, game_file_name);

    if (strrchr(_theSetup.data_files_dir, '/') != NULL)
    strrchr(_theSetup.data_files_dir, '/')[0] = 0;
    else if (strrchr(_theSetup.data_files_dir, '\\') != NULL)
    strrchr(_theSetup.data_files_dir, '\\')[0] = 0;
    else {
    platform->DisplayAlert("Error processing game file name: slash but no slash"); // what? :)
    return Err::FromCode(EXIT_NORMAL);
    }
    }
    */

    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
