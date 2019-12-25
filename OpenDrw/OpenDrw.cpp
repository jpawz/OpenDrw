/**
Retrives drawing (*.dwg) for active model (*.prt or *.asm).
 */

#include <ProCore.h>
#include <ProMenuBar.h>
#include <ProMessage.h>
#include <ProUtil.h>
#include <ProWindows.h>

using namespace std;

int OpenDrw();
static uiCmdAccessState AccessAvailable(uiCmdAccessMode);
void initializeMsgFile();

ProFileName msgFile;

extern "C" int main(int argc, char** argv)
{
	ProToolkitMain(argc, argv);
	return 0;
}

extern "C" int user_initialize()
{
	ProErr err = PRO_TK_NO_ERROR;
	uiCmdCmdId OpenDrwButton_cmd_id;

	initializeMsgFile();

	char openDrwAct[] = "OpenDrwAct";
	ProCmdActionAdd(openDrwAct, (uiCmdCmdActFn)OpenDrw,
	                uiCmdPrioDefault, AccessAvailable, PRO_B_FALSE,
	                PRO_B_FALSE, &OpenDrwButton_cmd_id);
	char util_c[] = "Utilities";
	char item_n[] = "OpenDrwName";
	char item_h[] = "Otworz rysunek";
	ProMenubarmenuPushbuttonAdd(util_c, item_n, item_n, item_h, nullptr,
	                            PRO_B_TRUE, OpenDrwButton_cmd_id, msgFile);

	return 0;
}

extern "C" void user_terminate()
{
}

int OpenDrw()
{
	ProErr err = PRO_TK_NO_ERROR;

	int old_win_id;
	ProWindowCurrentGet(&old_win_id);

	ProMdl model;
	err = ProMdlCurrentGet(&model);
	if (err != PRO_TK_NO_ERROR)
	{
		return 0;
	}

	ProName w_name;
	err = ProMdlNameGet(model, w_name);

	ProMdl drawing_mdl;
	ProName w_drw_name;
	ProType drw_obj = PRO_DRAWING;
	int new_win_id;
	err = ProMdlRetrieve(w_name, PRO_MDL_DRAWING, &drawing_mdl);
	err = ProMdlNameGet(drawing_mdl, w_drw_name);

	if (err != PRO_TK_NO_ERROR)
	{
		char msg[] = "nie znaleziono rysunku";
		ProMessageDisplay(msgFile, msg);
		ProWindowActivate(old_win_id);
		return 0;
	}

	err = ProObjectwindowCreate(w_drw_name, drw_obj, &new_win_id);
	err = ProWindowCurrentSet(new_win_id);
	err = ProWindowActivate(new_win_id);
}

static uiCmdAccessState AccessAvailable(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}

void initializeMsgFile()
{
	char MSGFIL[] = "opendrw.txt";
	ProStringToWstring(msgFile, MSGFIL);
}
