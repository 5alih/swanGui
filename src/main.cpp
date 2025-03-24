#include "swangui.h"

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI V2");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_INTERLACED_HINT);
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);

	SwanGui swanGui;

	while(!WindowShouldClose())
	{
		swanGui.Update();

		BeginDrawing();
			ClearBackground( hex("#101010") );
			swanGui.Draw();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}