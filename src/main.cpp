#include "swangui.h"

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI V2");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_INTERLACED_HINT);
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("resource/source-sans-pro.bold.ttf", 14, 0, 0);

	SwanGui swanGui({
		sw(Panel)("test", sx{ .position= (Vector2){0, 0}, .size= (Vector2){200, 1040}, .color= hex("#9999dd"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){200, 0}, .size= (Vector2){200, 1040}, .color= hex("#99dd99"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){400, 0}, .size= (Vector2){200, 1040}, .color= hex("#dd9999"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){600, 0}, .size= (Vector2){200, 1040}, .color= hex("#dddd99"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){800, 0}, .size= (Vector2){200, 1040}, .color= hex("#dd99dd"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){1000, 0}, .size= (Vector2){200, 1040}, .color= hex("#99dddd"), .font= custom_font }),
		sw(Panel)("test", sx{ .position= (Vector2){1200, 0}, .size= (Vector2){200, 1040}, .color= hex("#dddddd"), .font= custom_font }),

		sw(Panel)("my cool panel", sx{ .position= (Vector2){1500, 150}, .size= (Vector2){350, 500}, .font= custom_font }),
	});

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