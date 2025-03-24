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
#define util (Utility)
#define sw(x) std::make_shared<x>

inline Vector2 g_mouse_position= GetMousePosition();
inline int g_font_size= 14;
inline Font g_font= GetFontDefault();
inline bool g_left_clicked= IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
inline bool g_right_clicked= IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
inline bool g_middle_clicked= IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);

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
	S_HOVERED_HEADER,	// panel only, to detect if cursor is on header. used for panel utility
	S_HOVERED_RIGHT,	// panel only, to detect if cursor is on the right border of the panel. used for resizing
	S_HOVERED_BOTTOM,	// panel only, to detect if cursor is on the bottom border of the panel. used for resizing
	S_CLICKED,
	S_DISABLED,
};

struct Style{
	std::optional<enum_position> display= P_NORMAL;
	std::optional<Vector2> position= (Vector2){0, 0};
	std::optional<Vector2> size= (Vector2){0, 0};
	std::optional<Vector2> min_size= (Vector2){20, 20};
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

struct Utility{
	std::optional<bool> can_minimize= false;
	std::optional<bool> is_minimized= true;

	std::optional<bool> can_rescale= false;
	std::optional<bool> is_rescaling= false;
	
	std::optional<bool> can_move= false;
	std::optional<bool> is_moving= false;

	std::optional<bool> grid_align= true;
	std::optional<int> grid_size= 40;
};

class Panel: public GuiElement{
public:
	std::vector<std::shared_ptr<GuiElement>> elements;
	int sections= 1;	// column count
	Utility utility;	// utilities for panels

	Panel(std::string text_, Style style_){
		text= text_;
		style= style_;
	}

	Panel(std::string text_, Style style_, Utility utility_){
		text= text_;
		style= style_;
		utility= utility_;
	}

	void Update() override{
		if(style.border.value()){
			if(utility.can_minimize.value() && status== S_HOVERED_HEADER && g_left_clicked){
				utility.is_minimized.value()= !utility.is_minimized.value();
			}
			if(utility.can_move.value() && utility.is_moving.value()== false && status== S_HOVERED_HEADER && IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
				utility.is_moving.value()= true;
			}
			else if(utility.can_move.value() && utility.is_moving.value() && (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))){
				if(utility.grid_align.value()){
					Vector2 fixed_pos;
					fixed_pos.x= (int)(style.position.value().x/ utility.grid_size.value());
					fixed_pos.y= (int)(style.position.value().y/ utility.grid_size.value());

					fixed_pos.x= ( ((fixed_pos.x *utility.grid_size.value()) -style.position.value().x) *(-1)< (((fixed_pos.x +1) *utility.grid_size.value()) -style.position.value().x)) ?
						fixed_pos.x *utility.grid_size.value(): (fixed_pos.x +1) *utility.grid_size.value();
					fixed_pos.y= ( ((fixed_pos.y *utility.grid_size.value()) -style.position.value().y) *(-1)< (((fixed_pos.y +1) *utility.grid_size.value()) -style.position.value().y)) ?
						fixed_pos.y *utility.grid_size.value(): (fixed_pos.y +1) *utility.grid_size.value();
					
					Vector2 delta;
					delta.x= fixed_pos.x -style.position.value().x;
					delta.y= fixed_pos.y -style.position.value().y;

					style.position.value()= fixed_pos;

					for(auto &element: elements){
						element->style.position.value().x+= delta.x;
						element->style.position.value().y+= delta.y;
					}
					utility.is_moving.value()= false;
				}
			}
		}
		if(utility.can_move.value() && utility.is_moving.value()){
			Vector2 delta= GetMouseDelta();
			style.position.value().x+= delta.x;
			style.position.value().y+= delta.y;

			for(auto &element: elements){
				element->style.position.value().x+= delta.x;
				element->style.position.value().y+= delta.y;
			}
		}

		if(utility.can_rescale.value() && !utility.is_rescaling.value()){
			if(1 && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
				float delta= GetMouseDelta().x;

				if((style.size.value().x +delta)<= style.min_size.value().x){
					style.size.value().x= style.min_size.value().x;
				}
				else{
					style.size.value().x+= delta;
				}
			}
			if(1 && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
				float delta= GetMouseDelta().y;

				if((style.size.value().y +delta)<= style.min_size.value().y){
					style.size.value().y= style.min_size.value().y;
				}
				else{
					style.size.value().y+= delta;
				}
			}
		}

		if(!utility.is_minimized.value()){
			for(auto &element: elements){
				if((element->style.position.value().y /*+element->style.size.value().y*/)< (style.position.value().y +style.size.value().y))
					element->Update();
			}
		}

		float wheel_delta= GetMouseWheelMove();

		if(!utility.is_minimized.value() && status== S_HOVERED && wheel_delta!= 0){
			float delta= wheel_delta;	// can be changed for scroll speed
			for(auto &element: elements){
				element->style.position.value().y+= delta;
			}
		}
	}

	void Draw() override{	// use scissoring
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