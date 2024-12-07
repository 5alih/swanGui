#include "swangui.h"
#include <iostream>

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI Demo");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	MaximizeWindow();
	SetTargetFPS(60);

	Font custom_font= LoadFontEx("source-sans-pro.bold.ttf", 14, 0, 0);

	Vector2 panelPos=  {0, 0};
	Vector2 panelSize= {150, 1080};

	SwanGui swanGui;

	auto panel= std::make_shared<Panel>(panelPos, panelSize, custom_font);
	
	auto button1= std::make_shared<Button>("Do Something", [](){std::cout<< "did something"<< std::endl;});
	auto button2= std::make_shared<Button>("CHANGE RES", [](){std::cout<< "changed resolution"<< std::endl;}, false);
	auto button3= std::make_shared<Button>("RENDER", [](){std::cout<< "rendered"<< std::endl;}, true);
	
	auto textbox1= std::make_shared<TextBox>("this is a textbox");
	auto textbox2= std::make_shared<TextBox>("and another textbox");

	bool is_this_enabled= false;
	auto checkbox1= std::make_shared<CheckBox>("Bloom", is_this_enabled);
	auto checkbox2= std::make_shared<CheckBox>("Stars", is_this_enabled);

	int speed= 50;
	auto slider1= std::make_shared<Slider>("Speed", speed, 1, 0, 100);
	auto slider2= std::make_shared<Slider>("Speed", speed, 1, 0, 100);

	panel->addElement(button1);
	panel->addElement(button2);

	panel->addElement(checkbox1);
	panel->addElement(checkbox2);

	panel->addElement(slider1);
	panel->addElement(slider2);

	panel->addElement(button3);	

	panel->addElement(textbox2);
	panel->addElement(textbox1);

	swanGui.AddPanel(panel);

	while (!WindowShouldClose())
	{
		swanGui.Update();

		BeginDrawing();
			ClearBackground( (Color){20, 20, 20, 255} );
			swanGui.Draw();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}