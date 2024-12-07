#include "swangui.h"
#include <iostream>

int main() 
{
	// Initialize the window
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI Demo");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetExitKey(KEY_NULL);
	MaximizeWindow();
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("my_font.otf", 12, 0, 0);

	Vector2 panelPos= {0, 0};
	Vector2 panelSize= {150, 540};

	SwanGui swanGui;

	auto panel= std::make_shared<Panel>(panelPos, panelSize, custom_font);
	auto button1= std::make_shared<Button>("button 1", [](){std::cout<< "button clicked"<< std::endl;}, false);
	auto button2= std::make_shared<Button>("button 2", [](){std::cout<< "button clicked"<< std::endl;}, false);
	auto button3= std::make_shared<Button>("button 3", [](){std::cout<< "button clicked"<< std::endl;}, false);

	panel->addElement(button1);
	panel->addElement(button2);
	panel->addElement(button3);	

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