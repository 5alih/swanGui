#include "swangui.h"

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI V2");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_INTERLACED_HINT);
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("resource/source-sans-pro.bold.ttf", 14, 0, 0);

	bool my_bool_var1= false;
	bool my_bool_var2= false;
	bool my_bool_var3= false;

	SwanGui swanGui({
		sw(Panel)("MY COOL PANEL", sx{ .position= (Vector2){500, 150}, .size= (Vector2){360, 600}, .font= custom_font, .spacing= 10.0f }, util{.can_rescale= true, .can_move= true, .sections= 1}, add(GuiElement){
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			
			sw(RadioGroup)( "Select Language", util{}, add(Checkbox){
				sw(Checkbox)("Turkish", my_bool_var1),
				sw(Checkbox)("English", my_bool_var2),
				sw(Checkbox)("Spanish", my_bool_var3),
			}),

			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),

			sw(Panel)("please work", sx{.padding= 5}, util{.can_minimize= true, .is_minimized= false, .can_rescale= true, .can_move= true}, add(GuiElement){
				sw(Button)("test button", [](){}),
				sw(Button)("test button", [](){}),
				sw(Button)("test button", [](){}),
				sw(Button)("test button", [](){}),

				sw(Panel)("AAAAAAAAAAA",  sx{.padding= 5}, util{.can_minimize= true, .is_minimized= false, .can_rescale= true, .can_move= true}, add(GuiElement){
					sw(Button)("test button", [](){}),
					sw(Button)("test button", [](){}),

					sw(Panel)("BBBBBBBBBB",  sx{.padding= 5}, util{.can_minimize= true, .is_minimized= false, .can_rescale= true, .can_move= true}, add(GuiElement){
						sw(Button)("test button", [](){}),
						sw(Button)("test button", [](){}),
						sw(Button)("test button", [](){}),
						sw(Button)("test button", [](){}),
					}),

					sw(Button)("test button", [](){}),
					sw(Button)("test button", [](){}),
				}),
				sw(Button)("test button", [](){}, sx{.background_color= hex("#dddddd"), .background_color_hover= hex("#eeeeee"), .color= hex("#131313")}),
			}),

			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
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