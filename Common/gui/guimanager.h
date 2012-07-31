
//=============================================================================
//
// [IKM] 2012-07-28: draft for the GUI system manager
//
//=============================================================================
#ifndef __AGS_CN_GUI__GUIMANAGER_H
#define __AGS_CN_GUI__GUIMANAGER_H

#include "Common/core/err.h"
#include "Common/util/array.h"

#include "ac/gamesetupstruct.h"
#include "gui/guimain.h"
#include "gui/guibutton.h"
#include "gui/guiinv.h"
#include "gui/guilabel.h"
#include "gui/guilistbox.h"
#include "gui/guislider.h"
#include "gui/guitextbox.h"

namespace AGS
{
namespace Common
{

// Forward-declarations
namespace Util
{
class CStream;
} // namespace Util

namespace GUI
{

// Using-declarations
using AGS::Common::Core::HErr;
using AGS::Common::Util::CArray;
using AGS::Common::Util::CStream;

class CGUIManager
{
public:

    static const int32_t GUIDataVersion;

    CGUIManager();
    ~CGUIManager();    

    HErr    Initialize();
    HErr    ReadGUI(CStream *in, int game_data_version, GameSetupStruct &game);
    HErr    WriteGUI(CStream *out);

    int32_t GetGUICount() const;
    GUIMain *GetGUI(int32_t index);

protected:

    HErr    PostReadInit(int32_t gui_data_version);
    void    UpdateGUIZOrder();
    void    RegisterScriptGUIMains();
    void    RegisterScriptGUIControls(GUIMain *gui_main);

private:
    CArray<GUIMain>     _guiMain;
    CArray<int32_t>     _guiDrawOrder;
    CArray<GUIButton>   _guiButtons;
    CArray<GUIInv>      _guiInventoryWindows;
    CArray<GUILabel>    _guiLabels;
    CArray<GUIListBox>  _guiListBoxes;
    CArray<GUISlider>   _guiSliders;
    CArray<GUITextBox>  _guiTextBoxes;
};

} // namespace GUI
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_GUI__GUIMANAGER_H
