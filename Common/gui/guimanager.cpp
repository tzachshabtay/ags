
#include "util/wgt2allg.h"
#include "Common/gui/guimanager.h"
#include "Common/util/stream.h"

namespace AGS
{
namespace Common
{
namespace GUI
{

// Using-declarations
namespace Err = AGS::Common::Core::Err;

const int32_t CGUIManager::GUIDataVersion = 115;

CGUIManager::CGUIManager()
{

}

CGUIManager::~CGUIManager()
{

}

HErr CGUIManager::Initialize()
{
    return Err::Nil();
}

HErr CGUIManager::ReadGUI(CStream *in, int game_data_version, GameSetupStruct &game)
{
    HErr err;

    int32_t gui_magic = in->ReadInt32();
    if (gui_magic != (int)GUIMAGIC)
    {
        return Err::FromString("read_gui: file is corrupt");
    }

    int32_t gui_data_version = in->ReadInt32();
    if (gui_data_version < 100)
    {
        game.numgui         = gui_data_version;
        gui_data_version    = 0;
    }
    else if (gui_data_version > GUIDataVersion)
    {
        return Err::FromString("read_gui: this game requires a newer version of AGS");
    }
    else
    {
        game.numgui = in->ReadInt32();
    }

                          // CHECKME: is this really needed?
    if ((game.numgui < 0) /*|| (game.numgui > 1000)*/)
    {
        return Err::FromString("read_gui: invalid number of GUIs, file corrupt?");
    }

    /*
    if (allocate != NULL)
    {
        *allocate = (GUIMain*)malloc(sizeof(GUIMain) * game.numgui);
        guiread = *allocate;
    }
    */

    int i;
    // import the main GUI elements
    _guiMain.SetCount(game.numgui);
    for (i = 0; i < game.numgui; ++i)
    {
        _guiMain[i].init();
        err = _guiMain[i].Read(in, game_data_version, gui_data_version, i);
        if (!err.IsNil())
        {
            return err;
        }
    }

    // import the buttons
    int32_t num_gui_objects = in->ReadInt32();
    _guiButtons.SetCount(num_gui_objects);
    for (i = 0; i < numguibuts; ++i)
    {
        err = _guiButtons[i].Read(in, gui_data_version);
        if (!err.IsNil())
        {
            return err;
        }
    }

    // labels
    num_gui_objects = in->ReadInt32();
    _guiLabels.SetCount(num_gui_objects);
    for (i = 0; i < numguibuts; ++i)
    {
        err = _guiLabels[i].Read(in, gui_data_version);
        if (!err.IsNil())
        {
            return err;
        }
    }

    // inv controls
    num_gui_objects = in->ReadInt32();
    _guiInventoryWindows.SetCount(num_gui_objects);
    for (i = 0; i < numguibuts; ++i)
    {
        err = _guiInventoryWindows[i].Read(in, gui_data_version);
        if (!err.IsNil())
        {
            return err;
        }
    }

    if (gui_data_version >= 100)
    {
        // sliders
        num_gui_objects = in->ReadInt32();
        _guiSliders.SetCount(num_gui_objects);
        for (i = 0; i < numguibuts; ++i)
        {
            err = _guiSliders[i].Read(in, gui_data_version);
            if (!err.IsNil())
            {
                return err;
            }
        }
    }

    if (gui_data_version >= 101)
    {
        // text boxes
        num_gui_objects = in->ReadInt32();
        _guiTextBoxes.SetCount(num_gui_objects);
        for (i = 0; i < numguibuts; ++i)
        {
            err = _guiTextBoxes[i].Read(in, gui_data_version);
            if (!err.IsNil())
            {
                return err;
            }
        }
    }

    if (gui_data_version >= 102) {
        // list boxes
        num_gui_objects = in->ReadInt32();
        _guiListBoxes.SetCount(num_gui_objects);
        for (i = 0; i < numguibuts; ++i)
        {
            err = _guiListBoxes[i].Read(in, gui_data_version);
            if (!err.IsNil())
            {
                return err;
            }
        }
    }

    guis_need_update = 1;

    return PostReadInit(gui_data_version);
}

HErr CGUIManager::WriteGUI(CStream *out)
{
    return Err::Nil();
}

HErr CGUIManager::PostReadInit(int32_t gui_data_version)
{
    HErr err;

    _guiDrawOrder.SetCount(_guiMain.GetCount());

    for (int i = 0; i < _guiMain.GetCount(); ++i)
    {
        err = _guiMain[i].RebuildChildArray(gui_data_version);
        if (!err.IsNil())
        {
            return err;
        }
    }

    return Err::Nil();
}

} // namespace GUI
} // namespace Common
} // namespace AGS
