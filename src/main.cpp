#include "swangui.h"
#include <iostream>

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI Demo");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	MaximizeWindow();
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("source-sans-pro.bold.ttf", 14, 0, 0);

	Vector2 panelPos=  {0, 0};
	Vector2 panelSize= {150, 1080};

	SwanGui swanGui;

	auto panel= std::make_shared<Panel>(panelPos, panelSize, custom_font);
	
	auto button1= std::make_shared<Button>("RENDER", [](){std::cout<< "button clicked"<< std::endl;});
	auto button2= std::make_shared<Button>("Change Resolution", [](){std::cout<< "button clicked"<< std::endl;});
	auto button3= std::make_shared<Button>("Render", [](){std::cout<< "button clicked"<< std::endl;});
	
	auto textbox1= std::make_shared<TextBox>("this is a textbox");
	auto textbox2= std::make_shared<TextBox>("and another textbox");

	auto checkbox1= std::make_shared<CheckBox>("Bloom");
	auto checkbox2= std::make_shared<CheckBox>("Stars", true);

	auto slider1= std::make_shared<Slider>("Resolution", GetMonitorWidth(0));

	panel->addElement(button1);
	panel->addElement(button2);

	panel->addElement(checkbox1);
	panel->addElement(checkbox2);

	panel->addElement(slider1);

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