#include "swangui.h"

void DrawDemo3DScene(Camera3D& camera) {
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            for (int z = -2; z <= 2; z++) {
                Color cubeColor = {
                    static_cast<unsigned char>(abs(x) * 50),
                    static_cast<unsigned char>(abs(y) * 50),
                    static_cast<unsigned char>(abs(z) * 50),
                    255
                };
                DrawCube({x * 2.0f, y * 2.0f, z * 2.0f}, 1.0f, 1.0f, 1.0f, cubeColor);
            }
        }
    }
}

auto drawSceneFunction = [](Camera3D& camera) {
	DrawPlane((Vector3){ 0, 0, 0 }, (Vector2){ 20, 20 }, ui_element_body);
	DrawCube( (Vector3){0.0f, 0.5f, 0.0f}, 1.0f, 1.0f, 1.0f, ui_special);
};

int main() 
{
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "SwanGUI Demo");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_INTERLACED_HINT);
	SetExitKey(KEY_NULL);
	SetTargetFPS(144);

	Font custom_font= LoadFontEx("resource/source-sans-pro.bold.ttf", 14, 0, 0);
	Texture2D fish= LoadTexture("resource/fish.png");
	Texture2D logo= LoadTexture("resource/swan.png");

	Texture2D c1= LoadTexture("resource/1.png");
	Texture2D c2= LoadTexture("resource/2.png");
	Texture2D c3= LoadTexture("resource/3.png");
	Texture2D c4= LoadTexture("resource/4.png");
	Texture2D c5= LoadTexture("resource/5.png");
	Texture2D c6= LoadTexture("resource/6.png");
	Texture2D c7= LoadTexture("resource/7.png");
	Texture2D c8= LoadTexture("resource/8.png");

	Vector2 panelPos1= {0, 0};
	Vector2 panelPos2= {10, 0};
	Vector2 panelPos3= {16, 0};
	Vector2 panelPos4= {16, 38};
	Vector2 panelPos5= {28, 0};
	Vector2 panelPos6= {28, 26};

	Vector2 panelSize= {10, 52};
	Vector2 panelSize2= {6, 52};
	Vector2 panelSize3= {12, 38};
	Vector2 panelSize4= {12, 14};
	Vector2 panelSize5= {24, 26};

	SwanGui swanGui;

	auto panel1= std::make_shared<Panel>("BUTTONS AND SLIDERS", panelPos1, panelSize, custom_font);
	auto panel2= std::make_shared<Panel>("COLORS", panelPos2, panelSize2, custom_font);
	auto panel3= std::make_shared<Panel>("BILLBOARDS", panelPos3, panelSize3, custom_font);
	auto panel4= std::make_shared<Panel>("GIF BILLBOARD", panelPos4, panelSize4, custom_font);
	auto panel5= std::make_shared<Panel>("3D CAMERA", panelPos5, panelSize5, custom_font);
	auto panel6= std::make_shared<Panel>("3D CAMERA", panelPos6, panelSize5, custom_font);

	int force= 10;	
	bool yes= true;
	bool no= false;
	int worldSize= 32000;
	int entityCount= 128;
	panel1->addElement(std::make_shared<Button>("Apply Gravity", [](){}, false));
	panel1->addElement(std::make_shared<Slider>("Force", force, 1, -100, 100));
	panel1->addElement(std::make_shared<CheckBox>("Speed Limit", yes));
	panel1->addElement(std::make_shared<CheckBox>("World Border", no));
	panel1->addElement(std::make_shared<Slider>("World Size", worldSize, 10));
	panel1->addElement(std::make_shared<Slider>("Entity Count", entityCount, 1));
	int colorCount= 3;
	panel1->addElement(std::make_shared<Button>("Invert Colors", [](){}, false));
	panel1->addElement(std::make_shared<Slider>("Color Count", colorCount, 1));
	panel1->addElement(std::make_shared<CheckBox>("Invert", no));
	panel1->addElement(std::make_shared<Button>("Add Color", [](){}, false));
	panel1->addElement(std::make_shared<Thumbnail>("logo.png", logo, [](){}, "Rotate 90'"));
	int attachments= 7;
	panel1->addElement(std::make_shared<Button>("Attach Selected", [](){}, false));
	panel1->addElement(std::make_shared<Slider>("Attachments", attachments, 1));
	panel1->addElement(std::make_shared<Slider>("Attach Limit", attachments, 1));
	panel1->addElement(std::make_shared<CheckBox>("Attachments", yes));
	panel1->addElement(std::make_shared<Button>("Deattach", [](){}, false));
	int selection= 23;
	panel1->addElement(std::make_shared<Button>("Merge Meshes", [](){}, false));
	panel1->addElement(std::make_shared<Slider>("Selected", selection, 1));
	panel1->addElement(std::make_shared<CheckBox>("Auto Merge", no));
	panel1->addElement(std::make_shared<CheckBox>("Clipping", yes));
	panel1->addElement(std::make_shared<CheckBox>("Bounding Box", yes));
	int cubeCount= 19;
	panel1->addElement(std::make_shared<Button>("Triangulate", [](){}, false));
	panel1->addElement(std::make_shared<Button>("Reverse", [](){}, false));
	panel1->addElement(std::make_shared<Button>("Subdivide", [](){}, false));
	panel1->addElement(std::make_shared<Button>("Toggle Wires", [](){}, false));
	panel1->addElement(std::make_shared<Button>("Add Cube", [](){}, false));
	panel1->addElement(std::make_shared<Slider>("Cube Count", cubeCount, 1));
	panel1->addElement(std::make_shared<Button>("Lock Cursor", [](){}, false));
	panel1->addElement(std::make_shared<Button>("Add Break Point", [](){}, false));
	panel1->addElement(std::make_shared<CheckBox>("Until Failure", yes));
	panel1->addElement(std::make_shared<Button>("Restore Scene", [](){}, false));
	panel1->addElement(std::make_shared<Button>("SAVE AND EXIT", [](){}, true));

	int colorCount2= 8;
	panel2->addElement(std::make_shared<Comment>("Colors"));
	panel2->addElement(std::make_shared<Button>("Add Color", [](){}, false));
	panel2->addElement(std::make_shared<Button>("Remove Color", [](){}, false));
	panel2->addElement(std::make_shared<Thumbnail>("Red", c1, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Orange", c2, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Yellow", c3, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Green", c4, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Cyan", c5, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Blue", c6, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Purple", c7, [](){}, "Select"));
	panel2->addElement(std::make_shared<Thumbnail>("Pink", c8, [](){}, "Select"));
	panel2->addElement(std::make_shared<Button>("Add List", [](){}, false));
	panel2->addElement(std::make_shared<Button>("SAVE COLORS", [](){}, true));
	panel2->addElement(std::make_shared<Button>("Sort By Red", [](){}, false));
	panel2->addElement(std::make_shared<Slider>("Count", colorCount2, 1));
	panel2->addElement(std::make_shared<CheckBox>("List", no));
	panel2->addElement(std::make_shared<CheckBox>("Edit", no));

	panel3->addElement(std::make_shared<Comment>("Static Image"));
	panel3->addElement(std::make_shared<Button>("Add Image", [](){}, false));
	panel3->addElement(std::make_shared<Billboard>(logo));
	panel3->addElement(std::make_shared<Billboard>(fish));
	panel3->addElement(std::make_shared<Button>("Reverse Images", [](){}, false));
	panel3->addElement(std::make_shared<CheckBox>("Transparency", no));
	panel3->addElement(std::make_shared<CheckBox>("Lazy Loading", no));
	panel3->addElement(std::make_shared<Button>("Apply Post Process", [](){}, false));
	panel3->addElement(std::make_shared<Button>("Add to Scene", [](){}, false));
	panel3->addElement(std::make_shared<Button>("Set as Background", [](){}, false));
	panel3->addElement(std::make_shared<Button>("SAVE IMAGES", [](){}, true));

	int frameDelay= 6;
	panel4->addElement(std::make_shared<Comment>("Animated Image"));
	panel4->addElement(std::make_shared<BillboardGif>("resource/a.gif", frameDelay));

	Camera3D camera= {0};
	camera.position= (Vector3){0.0f, 2.0f, -10.0f};
	camera.target= (Vector3){0.0f, 0.0f, 0.0f};
	camera.up= (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy= 65.0f;
	panel5->addElement(std::make_shared<Comment>("Interactive 3D Camera View"));
	panel5->addElement(std::make_shared<CameraView3D>(camera, DrawDemo3DScene));

	panel6->addElement(std::make_shared<Comment>("Another Interactive 3D Camera View"));
	panel6->addElement(std::make_shared<CameraView3D>(camera, drawSceneFunction));

	panel1->addElement(std::make_shared<ColorPicker>("Accent Color", ui_special));
	panel1->addElement(std::make_shared<ColorPicker>("Panel Body", ui_panel_body));
	panel1->addElement(std::make_shared<ColorPicker>("Element Body", ui_element_body));
	panel1->addElement(std::make_shared<ColorPicker>("Text Light", ui_text_light));

	swanGui.AddPanel(panel1);
	swanGui.AddPanel(panel1);
	swanGui.AddPanel(panel2);
	swanGui.AddPanel(panel3);
	swanGui.AddPanel(panel4);
	swanGui.AddPanel(panel5);
	swanGui.AddPanel(panel6);

	while (!WindowShouldClose())
	{
		swanGui.Update();

		BeginDrawing();
			ClearBackground( BLACK );
			swanGui.Draw();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}