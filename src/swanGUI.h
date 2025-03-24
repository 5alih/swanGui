/*
* swanGui.h - a Single-Header library.
*
* Usage:
* #include "swanGui.h"
*
* License:
* The MIT License (MIT)
*
* Copyright (c) 2024 5alih
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef SWANGUI_H
#define SWANGUI_H

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <sstream>
#include <iostream>
#include "raylib.h"
#include <optional>

#define rgb(red, green, blue) (Color){red, green, blue, 255}
#define sx (Style)
#define sw(x) std::make_shared<x>

inline Vector2 g_mouse_position= GetMousePosition();
inline int g_font_size= 14;
inline Font g_font= GetFontDefault();
inline bool g_left_clicked= IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

inline Color hex(const std::string &hex_code_){
	std::string hex_code= hex_code_;
    if(hex_code[0]== '#'){
        hex_code.erase(0, 1);
    }
    int r= 0, g= 0, b= 0;
    if(hex_code.length()>= 6){
        sscanf(hex_code.c_str(), "%02x%02x%02x", &r, &g, &b);
    }
    return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
}

enum enum_position{
	P_NORMAL,		// default position of the element decided by the parent panel
	P_FIXED,		// fixed position relative to top left corner, doesnt get effected by anything
	p_FIXED_RIGHT,	// fixed position relative to top right corner, right side effects the position
	p_FIXED_BOTTOM,	// fixed position relative to bottom left corner, bottom side effects the position
	P_RELATIVE,		// relative to the default position of the element, changes when default position changes
	P_HORIZONTAL,	// relative to width of the window, keeps the distance in % to the right side
	P_VERTICAL,		// relative to height of the window, keeps the distance in % to the bottom side
	P_CENTERAL,		// relative to both width and height of the window, keeps the distance in % to the (max, max) point
};

enum enum_status{
	S_NORMAL,
	S_HOVERED,
	S_CLICKED,
	S_DISABLED,
};

struct Style{
	std::optional<enum_position> display= P_NORMAL;
	std::optional<Vector2> position= (Vector2){0, 0};
	std::optional<Vector2> size= (Vector2){0, 0};
	std::optional<Color> background_color= hex("#131313");
	std::optional<Color> color= hex("#f5f5f5");
	std::optional<Color> border_color= hex("#202020");
	std::optional<bool> border= true;
	std::optional<float> padding= 3.0;
	std::optional<int> font_size= g_font_size;
	std::optional<Font> font= g_font;
	std::optional<float> spacing= 2.0f;
};

class GuiElement{
public:
	std::string text;
	enum_status status= S_NORMAL;
	Style style;

	virtual void Update()= 0;
	virtual void Draw()= 0;
};



class Panel: public GuiElement{
public:
	std::vector<std::shared_ptr<GuiElement>> elements;
	int sections= 1;	// column count
	bool can_minimize= false;

	Panel(std::string text_, Style style_){
		text= text_;
		style= style_;
	}

	Panel(std::string text_, Style style_, bool can_minimize_){
		text= text_;
		style= style_;
		can_minimize= can_minimize_;
	}

	void Update() override{
		if(style.border.value()){
			if(status== S_HOVERED && g_left_clicked){
				
			}
		}

		for(auto &element: elements){
			element->Update();
		}
	}

	void Draw() override{
		DrawRectangleV(style.position.value(), style.size.value(), style.background_color.value());
		
		if(style.border.value()){
			DrawRectangleLines(style.position.value().x, style.position.value().y, style.size.value().x, style.size.value().y, style.border_color.value());
			DrawRectangle(style.position.value().x, style.position.value().y, style.size.value().x, style.font_size.value(), style.border_color.value());
			Vector2 pos= {(style.position.value().x + style.padding.value()), (float)(style.position.value().y + style.font_size.value()/2 - style.font_size.value()/2.5)};
			DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), style.color.value());
		}
		for(auto &element: elements){
			element->Draw();
		}
	}
};

class SwanGui{
public:
	std::vector<std::shared_ptr<Panel>> panels;
	
	SwanGui(){}

	SwanGui( std::vector<std::shared_ptr<Panel>> panels_ ){
		panels= panels_;
	}

	void AddPanel(std::shared_ptr<Panel> panel){
		auto it= std::find(panels.begin(), panels.end(), panel);
		if(it== panels.end()){
			panels.push_back(panel);
		}
	}

	// will check each panel to see if mouse is over
	// and if so searcg the elements in panel to set hovered element's status
	void UpdateElementHovered(){}

	void Update(){
		for(auto &panel: panels){
			panel->Update();
		}
	}

	void Draw(){
		for(auto &panel: panels){
			panel->Draw();
		}
	}
};

#endif // SWANGUI_H