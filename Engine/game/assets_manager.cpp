
#include "Common/core/out.h"
#include "Common/platform/base/platform_api.h"
#include "Common/util/filestream.h"
#include "Common/util/path.h"
#include "Engine/core/agsengine.h"
#include "Engine/game/assets_manager.h"
#include "ac/common_defines.h"
#include "platform/base/agsplatformdriver.h"
#include "util/misc.h"

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
using AGS::Common::Util::CFile;
using AGS::Common::Util::CFileStream;
using AGS::Engine::Core::CAGSEngine;
namespace Out = AGS::Common::Core::Out;
namespace Err = AGS::Common::Core::Err;
namespace Path = AGS::Common::Util::Path;

CAssetsManager::CAssetsManager(const CString &data_file_name, const CString &data_directory)
    : _gameDataFileName(data_file_name)
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
    const CEngineSetup &setup = engine->GetSetup();

    cfopenpriority = PR_FILEFIRST;

    // initialize the data file
    if (!SetGameDataFileName())
    {
        return Err::FromCode(EXIT_NORMAL);
    }

    int errcod = csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");  // assume it's appended to exe

    engine->SetEIP(-194);
    //  char gamefilenamebuf[200];

    HErr err;
    if ((errcod!=0) && (!setup.MustChangeToGameDataDirectory)) {
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

    if (!err.IsNil()) {
        return err;
    }

    engine->SetEIP(-193);

    err = RegisterAdditionalDataFiles();
    return err;
}

CString CAssetsManager::GetDataDirectory() const
{
    return _gameDataDirectory;
}

CString CAssetsManager::GetDataFileName() const
{
    return _gameDataFileName;
}

CString CAssetsManager::GetMusicFileName() const
{
    return _musicFileName;
}

CString CAssetsManager::GetSpeechFileName() const
{
    return _speechFileName;
}

AssetsSearchPriority CAssetsManager::GetSearchPriority() const
{
    return (AssetsSearchPriority)cfopenpriority;
}

bool CAssetsManager::SetSearchPriority(AssetsSearchPriority pr)
{
    if (pr < kAssetsPriority_Data || pr > kAssetsPriority_File)
    {
        return false;
    }

    cfopenpriority = (int)pr;
    return true;
}

CStream *CAssetsManager::OpenAsset(const CString &data_file, const CString &asset_file, const CString &mode)
{
    if (_currentDataFile.Compare(data_file) != 0)
    {
        if (csetlib(const_cast<char*>(data_file.GetCStr()),"") != 0)
        {
            return NULL;
        }
        _currentDataFile = data_file;
    }

    FILE *f = clibfopen(const_cast<char*>(asset_file.GetCStr()),
                        const_cast<char*>(mode.GetCStr()));
    if (!f)
    {
        return NULL;
    }

    return CFileStream::CreateOwner(CFile::CreateOwner(f));
}

CStream *OpenAsset(const CString &data_file, const CString &asset_file);

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
    const CEngineSetup &setup = CAGSEngine::GetSetup();
    // [IKM] If I understand this right, this method is called only when the game file name was specified
    // in cmdline, but not found
    int errcod = 0;
    _gameDataFileName = ci_find_file(const_cast<char*>(setup.DataFilesDir.GetCStr()),
        const_cast<char*>(setup.MainDataFilename.GetCStr()));

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
        _gameDataFileName = ci_find_file(const_cast<char*>(setup.DataFilesDir.GetCStr()), "ac2game.ags");
        errcod = csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");
    }

    return errcod;
}

HErr CAssetsManager::InitGameDataInternal()
{
    // [IKM] .... is it all about this?
    // FIXME -- setup should be const
    CEngineSetup &setup = CAGSEngine::GetSetup();
    setup.MainDataFilename = _gameDataFileName;
    if (setup.DataFilesDir.Compare(".") == 0)
    {
        setup.DataFilesDir = _gameDataDirectory;
    }

    // ---- old code ------------------------------

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

HErr CAssetsManager::RegisterAdditionalDataFiles()
{
    CEngineSetup &setup = CAGSEngine::GetSetup();
    AGSPlatformDriver *platform = AGSPlatformDriver::GetDriver();

    HErr err;
    if (setup.NoSpeechPack == 0)
    {
        err = RegisterDataFile("speech.vox", _speechFileName);
        if (!err.IsNil())
        {
            platform->DisplayAlert("Unable to initialize speech file - check for corruption and that\nit belongs to this game.\n");
            return Err::FromCode(EXIT_NORMAL);
        }
        platform->WriteConsole("Speech sample file found and initialized.\n");
    }

    _seperateMusicLib = false;
    err = RegisterDataFile("music.vox", _musicFileName);
    if (err.IsNil())
    {
        platform->WriteConsole("Audio vox found and initialized.\n");
        _seperateMusicLib = true;
    }

    return Err::Nil();
}

HErr CAssetsManager::RegisterDataFile(const CString &data_file, CString &actual_file_path)
{
    CEngineSetup &setup = CAGSEngine::GetSetup();

    /* Can't just use fopen here, since we need to change the filename
    so that pack functions, etc. will have the right case later */
    char *filename = ci_find_file(const_cast<char*>(setup.DataFilesDir.GetCStr()),
                                  const_cast<char*>(data_file.GetCStr()));
    /* Don't need to use ci_fopen here, because we've used ci_find_file to get
    the case insensitive matched filename already */
    // Use ci_fopen anyway because it can handle NULL filenames.
    FILE *f = ci_fopen(filename, "rb");
    if (f == NULL)
    {
        // In case they're running in debug, check Compiled folder
        free(filename);
        filename = ci_find_file("Compiled", const_cast<char*>(data_file.GetCStr()));
        f = ci_fopen(filename, "rb");
    }

    if (f != NULL)
    {
        fclose(f);
    }

    if (!filename)
    {
        return Err::FromCode(0); // TODO: actual err code
    }
    actual_file_path = filename;
    free(filename);
    if (csetlib(const_cast<char*>(_speechFileName.GetCStr()),"")!=0)
    {
        return Err::FromCode(0); // TODO: actual err code
    }
    csetlib(const_cast<char*>(_gameDataFileName.GetCStr()),"");
    return Err::Nil();
}

} // namespace Game
} // namespace Engine
} // namespace AGS
