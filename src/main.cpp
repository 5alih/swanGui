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
	auto textbox2= std::make_shared<TextBox>("this is another textbox");

	auto checkbox1= std::make_shared<CheckBox>("Bloom", false);

	panel->addElement(button1);
	panel->addElement(button2);
	panel->addElement(button3);	

	panel->addElement(textbox1);
	panel->addElement(textbox2);

	panel->addElement(checkbox1);

	swanGui.AddPanel(panel);

	while (!WindowShouldClose())
	{
		swanGui.Update();

		BeginDrawing();
			ClearBackground( (Color){20, 20, 20, 255} );
			//ClearBackground(BLACK);

			swanGui.Draw();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}