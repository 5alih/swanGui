#include "swangui.h"
#include "fa_api.h"

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI V2");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_INTERLACED_HINT);
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("resource/Inter-Regular.ttf", 14, 0, 0);
	Font custom_font32= LoadFontEx("resource/Inter-Regular.ttf", 32, 0, 0);

	bool my_bool_var1= false;
	bool my_bool_var2= false;
	bool my_bool_var3= false;
	bool my_bool_var4= false;
	bool my_bool_var5= false;
	bool my_bool_var6= false;
	bool my_bool_var7= false;
	bool my_bool_var8= false;
	float my_float_var1= 15.0f;
	float my_float_var2= 15.0f;
	int my_int_var1= 15;
	std::string my_string_var= "Lorem ipsum is a dummy or placeholder text commonly used in graphic design, publishing, and web development. Its purpose is to permit a page layout to be designed, independently of the copy that will subsequently populate it, or to demonstrate various fonts of a typeface without meaningful text that could be distracting. Lorem ipsum is a dummy or placeholder text commonly used in graphic design, publishing, and web development. Its purpose is to permit a page layout to be designed, independently of the copy that will subsequently populate it, or to demonstrate various fonts of a typeface without meaningful text that could be distracting.";

	SwanGui swanGui("resource/FontAwesome.ttf", 12, {
		sw(Panel)("SETTINGS", sx{ .position= (Vector2){500, 120}, .size= (Vector2){360, 600}, .font= custom_font }, util{.can_rescale= true, .can_move= true, .sections= 1}, add(GuiElement){
			sw(Button)("Run scene", [](){}, sx{.icon= FA_face_grin_hearts, .icon_color= RED}),
			sw(Button)("Reset scene", [](){}),
			sw(Panel)("Add level to", sx{.icon= FA_face_grin_hearts, .icon_color= RED}, util{.can_minimize= true}, add(GuiElement){
				sw(Switch)("Indoor", my_bool_var1, sx{.icon= FA_face_grin_hearts, .icon_color= RED}),
				sw(Switch)("Outdoor", my_bool_var2),
				sw(Switch)("Day", my_bool_var3),
				sw(Switch)("Night", my_bool_var4),
				sw(Switch)("Easy", my_bool_var5),
				sw(Slider)("FOV", my_float_var1, 60, 120, sx{.icon= FA_face_grin_hearts, .icon_color= RED}),
				sw(Stepper)("vm_x", my_int_var1, 60, 120, 5, sx{.icon= FA_face_grin_hearts, .icon_color= RED}),
				sw(Switch)("Moderate", my_bool_var6),
			}),
			sw(Button)("Save scene", [](){}),
			sw(TextField)("Enter name", my_string_var, 2),
			sw(Divider)(1.0f, 0.9f),
			sw(TextField)("This is a very long text", my_string_var, 10),
			
			sw(Panel)( "Networking Settings", util{.can_minimize= true}, add(GuiElement){
				sw(RadioGroup)( "Connection Type", util{.can_minimize= true}, add(Checkbox){
					sw(Checkbox)("TCP", my_bool_var1, sx{.icon= FA_face_grin_hearts, .icon_color= RED}),
					sw(Checkbox)("UDP", my_bool_var2),
				}),
				sw(RadioGroup)( "Second Window", util{.can_minimize= true}, add(Checkbox){
					sw(Checkbox)("Left side", my_bool_var3),
					sw(Checkbox)("Right side", my_bool_var4),
					sw(Checkbox)("Open external", my_bool_var5),
				}),
				sw(RadioGroup)( "Server Region", util{}, add(Checkbox){
					sw(Checkbox)("Europe", my_bool_var6),
					sw(Checkbox)("Asia", my_bool_var7),
					sw(Checkbox)("America", my_bool_var8),
				}),
			}),
			
			sw(Slider)("FOV", my_float_var1, 60, 120),
			sw(Slider)("Health", my_float_var2, 0, 100),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),
			
			sw(RadioGroup)( "Select Language", util{ .can_minimize= true }, add(Checkbox){
				sw(Checkbox)("Turkish", my_bool_var1),
				sw(Checkbox)("English", my_bool_var2),
				sw(Checkbox)("Spanish", my_bool_var3),
			}),

			sw(Button)("test button", [](){}),
			sw(Button)("test button", [](){}),

			sw(Panel)("please work", sx{.padding= 5}, util{.can_minimize= true, .is_minimized= false, .can_rescale= true, .can_move= true}, add(GuiElement){
				sw(Button)("test button", [](){}),
				sw(Button)("test button", [](){}),
				sw(RadioGroup)( "Select Language", util{}, add(Checkbox){
					sw(Checkbox)("Turkish", my_bool_var1),
					sw(Checkbox)("English", my_bool_var2),
					sw(Checkbox)("Spanish", my_bool_var3),
				}),
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

		sw(Panel)( "TEXT FIELD", sx{ .position= (Vector2){1000, 180}, .size= (Vector2){360, 600}, .icon= FA_face_laugh_squint, .icon_color= SKYBLUE, .font= custom_font }, util{.can_rescale= true, .can_move= true, .sections= 1}, add(GuiElement){
			sw(TextField)("Enter name", my_string_var),
			sw(TextField)("Enter name", my_string_var, 10, sx{.font_size= 32, .font= custom_font32}),
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