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

		sw(Panel)("MY COOL PANEL", sx{ .position= (Vector2){500, 150}, .size= (Vector2){350, 600}, .font= custom_font, .spacing= 10.0f }, util{ .can_rescale= true, .can_move= true,}, add{
			sw(Button)("AAAAA", [](){}, sx{.background_color= DARKPURPLE, .color= YELLOW, .font= GetFontDefault()} ),
			sw(Button)("BBBBBB", [](){}, sx{.background_color= DARKBLUE, .color= YELLOW} ),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("CCCC", [](){}),
		}),
	});


	while(!WindowShouldClose())
	{
		swanGui.Update();

		BeginDrawing();
			ClearBackground( hex("#000000") );
			swanGui.Draw();
		EndDrawing();
	}
	UnloadFont(custom_font);
	CloseWindow();
	return 0;
}