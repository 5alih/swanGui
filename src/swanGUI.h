/*
* swanGui.h - a Single-Header GUI library.
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

#pragma once

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
#define add(x) std::vector<std::shared_ptr<x>>

inline Vector2 g_mouse_position= GetMousePosition();
inline int g_font_size= 14;
inline Font g_font= GetFontDefault();

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

bool operator==(const Vector2& lhs, const Vector2& rhs){
	return lhs.x== rhs.x && lhs.y== rhs.y;
}

bool operator==(const Font& lhs, const Font& rhs){
	return lhs.glyphs== rhs.glyphs && lhs.baseSize== rhs.baseSize && lhs.recs== rhs.recs;
}

bool operator==(const Color& lhs, const Color& rhs){
	return lhs.r== rhs.r && lhs.g== rhs.g && lhs.b== rhs.b && lhs.a== rhs.a;
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
	S_HOVERED_CORNER,
	S_CLICKED,
	S_DISABLED,
};

// default styling
// to change the styling for every element
struct Style{
	std::optional<enum_position> display= P_NORMAL;			// position type
	std::optional<Vector2> position= (Vector2){0, 0};		// position in 2D space
	
	std::optional<Vector2> size= (Vector2){100, 100};		// size in (width, height)
	std::optional<Vector2> min_size= (Vector2){100, 100};	// minimum size in (width, height); to limit shrinking when the panel scaling is enabled
	
	std::optional<Color> background_color= hex("#202020");	// background color of the element
	std::optional<Color> background_color_hover= hex("#2C2C2C");
	std::optional<Color> background_color_click= hex("#010101");
	std::optional<Color> background_color_panel= hex("#131313");	// background color of the element
	std::optional<Color> color= hex("#f5f5f5");				// text color of the element
	std::optional<Color> color_hover= hex("#FFFFFF");
	std::optional<Color> color_disabled= hex("#999999");
	// std::optional<Color> color_accent= hex("#fdd835");
	std::optional<Color> color_accent= hex("#dddddd");
	// std::optional<Color> color_accent_hover= hex("#fff176");
	std::optional<Color> color_accent_hover= hex("#eeeeee");
	// std::optional<Color> color_accent_disabled= hex("#ad993c");
	std::optional<Color> color_accent_disabled= hex("#999999");
	std::optional<Color> border_color= hex("#404040");		// border color of the element
	std::optional<Color> border_color_hover= hex("#2C2C2C");
	
	std::optional<bool> border= true;						// enables/ disables border
	std::optional<int> border_radius= 3;						// for corner rounding, doesnt get effected by border is being disabled
	std::optional<float> padding= 3.0;						// horizontal padding between elements and borders; only effects panels
	std::optional<float> margin= 3.0;						// vertical margin between elements; only effects panels
	
	std::optional<int> font_size= g_font_size;				// text size
	std::optional<Font> font= g_font;						// for custom fonts
	std::optional<float> spacing= 2.0f;						// spacing of letters in text

	// std::optional<bool> legacy= false;						// design of legacy version (swanGui 1.0)
};

// utilities for panels, can be set per panel.
struct Utility{
	std::optional<bool> can_minimize= false;
	std::optional<bool> is_minimized= false;

	std::optional<bool> can_rescale= false;
	std::optional<bool> is_rescaling_h= false;
	std::optional<bool> is_rescaling_v= false;
	
	std::optional<bool> can_move= false;
	std::optional<bool> is_moving= false;

	std::optional<bool> grid_align= true;
	std::optional<int> grid_size= 20;
	
	std::optional<int> sections= 1;
	std::optional<float> scroll_amount= 0.0f;
	std::optional<float> scroll_speed= 10.0f;

	Style ow_style;
	std::optional<Style> style= ow_style;
};

class GuiElement{
public:
	std::string text;
	enum_status status= S_NORMAL;
	Style style;

	virtual void Update(bool is_parent)= 0;
	virtual void Draw(bool is_parent)= 0;

	virtual void UpdateElements(){}
	virtual bool CheckCollision(){
		return false;
	}
	virtual float CalcSizeY(){
		return style.font_size.value();
	}
};

class Button: public GuiElement{
public:
	std::function<void()> call_back_function;

	Button(const std::string text_, std::function<void()> call_back_function_){
		text= text_;
		call_back_function= call_back_function_;
	}

	Button(const std::string text_, std::function<void()> call_back_function_, Style style_){
		text= text_;
		call_back_function= call_back_function_;
		style= style_;
	}

	void Update(bool is_parent) override{
		if(status== S_CLICKED){
			if(call_back_function){
				call_back_function();
			}
		}
	}

	void Draw(bool is_parent) override{
		Color color= (status== S_HOVERED)? (status== S_CLICKED)?style.background_color_click.value() :style.background_color_hover.value() :style.background_color.value();
		Rectangle rectangle= {style.position.value().x, style.position.value().y, style.size.value().x, style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, color);
		Vector2 pos= {(style.position.value().x +style.size.value().x/2.0f -MeasureText(text.c_str(), style.font_size.value())/2.0f), (style.position.value().y + style.size.value().y/2.0f -style.font_size.value()/2.5f)};
		DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), style.color.value());
	}
};

class Checkbox: public GuiElement{
public:
	bool *is_checked;

	Checkbox(const std::string text_, bool &is_checked_){
		text= text_;
		is_checked= &is_checked_;
	}

	Checkbox(const std::string text_, bool &is_checked_, Style style_){
		text= text_;
		is_checked= &is_checked_;
		style= style_;
	}

	void Update(bool is_parent) override{
		if(status== S_CLICKED && is_checked!= nullptr){
			*is_checked= !(*is_checked);
		}
	}

	void Draw(bool is_parent) override{
		Color color_box;
		if(*is_checked== true)  color_box= (status== S_HOVERED)? (status== S_CLICKED)? style.background_color_click.value(): style.color_accent_hover.value(): style.color_accent.value();
		if(*is_checked== false) color_box= (status== S_HOVERED)? (status== S_CLICKED)? style.background_color_click.value(): style.background_color_hover.value(): style.background_color.value();
		Color color_text= (status== S_HOVERED)? style.color_hover.value(): style.color.value();

		Rectangle rectangle= {style.position.value().x, style.position.value().y, style.size.value().y, style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, color_box);
		Vector2 pos= {(style.position.value().x +style.padding.value() + style.size.value().y), (style.position.value().y + style.size.value().y/2.0f -style.font_size.value()/2.5f)};
		DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), color_text);
	}
};

class Switch: public GuiElement{
public:
	bool *is_checked;

	Switch(const std::string text_, bool &is_checked_){
		text= text_;
		is_checked= &is_checked_;
	}

	Switch(const std::string text_, bool &is_checked_, Style style_){
		text= text_;
		is_checked= &is_checked_;
		style= style_;
	}

	void Update(bool is_parent) override{
		if(status== S_CLICKED && is_checked!= nullptr){
			*is_checked= !(*is_checked);
		}
	}

	void Draw(bool is_parent) override{
		Color color_box= (status== S_HOVERED)? (status== S_CLICKED)? style.background_color_click.value(): style.color_accent_hover.value(): style.color_accent.value();
		Color color_text= (status== S_HOVERED)? style.color_hover.value(): style.color.value();

		Rectangle rectangle= {style.position.value().x +style.size.value().x/2.0f, style.position.value().y, style.font_size.value() *2.5f, style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, style.color_accent_disabled.value());
		//switch
		float extra= (*is_checked)? style.font_size.value() *1.5f: 0;
		rectangle= {style.position.value().x +style.size.value().x/2.0f +extra, style.position.value().y, (float)style.font_size.value(), style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, color_box);	
		
		Vector2 pos= {(style.position.value().x), (style.position.value().y + style.size.value().y/2.0f -style.font_size.value()/2.5f)};
		DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), color_text);
	}
};

class Slider: public GuiElement{
public:
	float *target;
	float min= 0.0f;
	float max= 100.0f;

	Slider(const std::string text_, float &target_){
		text= text_;
		target= &target_;
	}

	Slider(const std::string text_, float &target_, Style style_){
		text= text_;
		target= &target_;
		style= style_;
	}

    void Update(bool is_parent) override{
        if (status== S_HOVERED && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float mouse= GetMousePosition().x;
            float sliderLeft= style.position.value().x +style.size.value().x/2.0f;
            float sliderRight= sliderLeft +style.size.value().x/2.0f -style.font_size.value();
            float clamped= std::clamp(mouse, sliderLeft, sliderRight);

            float normalizedValue= (clamped -sliderLeft)/(sliderRight -sliderLeft);
            *target= min +normalizedValue *(max -min);
        }
    }

	void Draw(bool is_parent) override{
		Color color_box= (status== S_HOVERED)? (status== S_CLICKED)? style.background_color_click.value(): style.color_accent_hover.value(): style.color_accent.value();
		Color color_text= (status== S_HOVERED)? style.color_hover.value(): style.color.value();

		Rectangle rectangle= {style.position.value().x +style.size.value().x/2.0f, style.position.value().y, style.size.value().x/2.0f, style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, style.color_accent_disabled.value());

		// float extra= (*target/max) *style.size.value().x/2.0f;
		float extra= *target *(((style.size.value().x/2.0f) -style.font_size.value())/max);
		rectangle= {style.position.value().x +style.size.value().x/2.0f +extra, style.position.value().y, (float)style.font_size.value(), style.size.value().y};
		DrawRectangleRounded(rectangle , style.border_radius.value()/10.0f, 2, color_box);	
		
		Vector2 pos= {(style.position.value().x), (style.position.value().y + style.size.value().y/2.0f -style.font_size.value()/2.5f)};
		DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), color_text);
		
		pos= {(style.position.value().x + style.size.value().x/4.0f), (style.position.value().y + style.size.value().y/2.0f -style.font_size.value()/2.5f)};
		DrawTextEx(style.font.value(), (std::to_string(*target)).c_str(), pos, style.font_size.value(), style.spacing.value(), color_text);
	}
};

class Panel: public GuiElement{
private:
	int counter= 0;
	bool run_once= false;
	int ind= 0;

public:
	std::vector<std::shared_ptr<GuiElement>> elements;
	Utility utility;	// utilities for panels

	Panel(std::string text_, Style style_){
		text= text_;
		style= style_;
	}

	Panel(std::string text_, Utility utility_, std::vector<std::shared_ptr<GuiElement>> elements_){
		text= text_;
		utility= utility_;
		elements= elements_;
	}

	Panel(std::string text_, Style style_, std::vector<std::shared_ptr<GuiElement>> elements_){
		text= text_;
		style= style_;
		elements= elements_;
	}

	Panel(std::string text_, std::vector<std::shared_ptr<GuiElement>> elements_){
		text= text_;
		elements= elements_;
	}

	Panel(std::string text_, Style style_, Utility utility_){
		text= text_;
		style= style_;
		utility= utility_;
	}

	Panel(std::string text_, Style style_, Utility utility_, std::vector<std::shared_ptr<GuiElement>> elements_){
		text= text_;
		style= style_;
		utility= utility_;
		elements= elements_;
	}

	void Update(bool is_parent) override{
		if(ind< 2){
			UpdateElements();
			for(auto &element: elements){
				ApplyStyle(element);
			}
			ind++;
		}

		if(style.border.value()){
			if(utility.can_minimize.value() && status== S_HOVERED_HEADER && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
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
			UpdateElements();
		}

		if(utility.can_rescale.value()){
			if((status== S_HOVERED_RIGHT || status== S_HOVERED_CORNER) && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
				utility.is_rescaling_h.value()= true;
			}
			else if(IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)){
				utility.is_rescaling_h.value()= false;
			}

			if((status== S_HOVERED_BOTTOM || status== S_HOVERED_CORNER) && IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
				utility.is_rescaling_v.value()= true;
			}
			else if(IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)){
				utility.is_rescaling_v.value()= false;
			}

			if(utility.is_rescaling_h.value()){
				float delta= GetMouseDelta().x;

				if((style.size.value().x +delta)<= style.min_size.value().x){
					style.size.value().x= style.min_size.value().x;
				}
				else{
					style.size.value().x+= delta;
				}
				UpdateElements();
			}
			if(utility.is_rescaling_v.value()){
				float delta= GetMouseDelta().y;

				if((style.size.value().y +delta)<= style.min_size.value().y){
					style.size.value().y= style.min_size.value().y;
				}
				else{
					style.size.value().y+= delta;
				}
				counter= 0;
				UpdateElements();
			}
		}

		if(!utility.is_minimized.value()){
			for(auto &element: elements){
				if((element->style.position.value().y /*+element->style.size.value().y*/)< (style.position.value().y +style.size.value().y)){

					element->Update(false);
					element->UpdateElements();
				}
			}
		}

		if(is_parent){
			float wheel_delta= GetMouseWheelMove();
			if(!utility.is_minimized.value() && status== S_HOVERED && wheel_delta!= 0){
				float delta= utility.scroll_speed.value() *wheel_delta;	// can be changed for scroll speed
				
				if((utility.scroll_amount.value() +delta)<= 0){	
					for(auto &element: elements){
						element->style.position.value().y+= delta;
						utility.scroll_amount.value()+= delta;
					}
				}
			}
		}
	}

	void Draw(bool is_parent) override{	// use scissoring
		if(is_parent){
			if(!utility.is_minimized.value()){
				// BeginScissorMode(style.position.value().x, style.position.value().y, style.size.value().x, style.size.value().y);
			}
			else{
				// BeginScissorMode(style.position.value().x, style.position.value().y, style.size.value().x, style.font_size.value());
			}
		}
		DrawRectangleV(style.position.value(), style.size.value(), style.background_color_panel.value());
		if(!utility.is_minimized.value()){
			for(auto &element: elements){
				// if(element->style.position.value().y> style.position.value().y && (element->style.position.value().y /*+element->style.size.value().y*/< style.position.value().y +style.size.value().y))
				element->Draw(false);
			}
		}
		if(style.border.value()){
			DrawRectangleLines(style.position.value().x, style.position.value().y, style.size.value().x, style.size.value().y, style.border_color.value());
			DrawRectangle(style.position.value().x, style.position.value().y, style.size.value().x, style.font_size.value(), style.border_color.value());
			Vector2 pos= {(style.position.value().x + style.padding.value()), (float)(style.position.value().y + style.font_size.value()/2 - style.font_size.value()/2.5)};
			DrawTextEx(style.font.value(), text.c_str(), pos, style.font_size.value(), style.spacing.value(), style.color.value());
		}
		if(is_parent){
			// EndScissorMode();
		}
	}

	void ApplyStyle(std::shared_ptr<GuiElement> element){
		Style default_style;

		if(element->style.font_size.value()== default_style.font_size.value())			element->style.font_size.value()= utility.style.value().font_size.value();
		if(element->style.font.value()== default_style.font.value())					element->style.font.value()= style.font.value();
		if(element->style.spacing.value()== default_style.spacing.value())				element->style.spacing.value()= utility.style.value().spacing.value();
		if(element->style.border_radius.value()== default_style.border_radius.value())	element->style.border_radius.value()= utility.style.value().border_radius.value();
		if(element->style.padding.value()== default_style.padding.value())				element->style.padding.value()= utility.style.value().padding.value();
		if(element->style.margin.value()== default_style.margin.value())				element->style.margin.value()= utility.style.value().margin.value();
		
		if(element->style.background_color.value()== default_style.background_color.value())				element->style.background_color.value()= utility.style.value().background_color.value();
		if(element->style.background_color_click.value()== default_style.background_color_click.value())	element->style.background_color_click.value()= utility.style.value().background_color_click.value();
		if(element->style.background_color_hover.value()== default_style.background_color_hover.value())	element->style.background_color_hover.value()= utility.style.value().background_color_hover.value();
		if(element->style.border_color.value()== default_style.border_color.value())						element->style.border_color.value()= utility.style.value().border_color.value();
		if(element->style.border_color_hover.value()== default_style.border_color_hover.value())			element->style.border_color_hover.value()= utility.style.value().border_color_hover.value();
		if(element->style.color.value()== default_style.color.value())										element->style.color.value()= utility.style.value().color.value();
		if(element->style.color_hover.value()== default_style.color_hover.value())							element->style.color_hover.value()= utility.style.value().color_hover.value();
		if(element->style.color_disabled.value()== default_style.color_disabled.value())					element->style.color_disabled.value()= utility.style.value().color_disabled.value();
		if(element->style.color_accent.value()== default_style.color_accent.value())						element->style.color_accent.value()= utility.style.value().color_accent.value();
		if(element->style.color_accent_hover.value()== default_style.color_accent_hover.value())			element->style.color_accent_hover.value()= utility.style.value().color_accent_hover.value();
		if(element->style.color_accent_disabled.value()== default_style.color_accent_disabled.value())		element->style.color_accent_disabled.value()= utility.style.value().color_accent_disabled.value();
	}

	bool CheckCollision() override{
		Vector2 mouse= GetMousePosition();
		for(auto &element: elements){
			if((element->style.position.value().x< mouse.x) && (mouse.x< (element->style.position.value().x +element->style.size.value().x)) &&
			   (element->style.position.value().y< mouse.y) && (mouse.y< (element->style.position.value().y +element->style.size.value().y)) ){
				element->status= IsMouseButtonPressed(MOUSE_BUTTON_LEFT)? S_CLICKED: S_HOVERED;
				if(element->CheckCollision()){
					if((element->style.position.value().x< mouse.x) && (mouse.x< (element->style.position.value().x +element->style.size.value().x)) &&
					   (element->style.position.value().y< mouse.y) && (mouse.y< (element->style.position.value().y +element->style.font_size.value())))
						element->status= S_HOVERED_HEADER;
				}
			}
			else{
				if(element->status!= S_DISABLED)
					element->status= S_NORMAL;
			}
		}
		return true;
	}

	template<typename T>
	void UpdateElement(std::shared_ptr<T> element){
		if(counter>= utility.sections.value()){
			counter= 0;
		}

		Vector2 position_= style.position.value();
		position_.x+= style.padding.value() *2 +(counter *(style.size.value().x/ utility.sections.value()));
		position_.y+= style.border.value()? style.margin.value() +style.font_size.value(): style.margin.value();

		position_.y+= utility.scroll_amount.value()/ (int)elements.size();

		int group= 0;
		for(const auto &elem: elements){
			if(elem== element){break;}

			else if(group== counter)
				position_.y+= elem->style.size.value().y +style.margin.value();
			
			group++;
			if(group== utility.sections.value())
				group= 0;
		}

		Vector2 size_= style.size.value();
		size_.x= style.size.value().x/ utility.sections.value();
		size_.x-= style.padding.value() *4;

		size_.y= element->CalcSizeY();

		element->style.position.value()= position_;
		element->style.size.value()= size_;

		counter++;
	}

	void UpdateElements() override{
		if(utility.is_minimized.value()){
			return;
		}

		counter= 0;
		for(auto &element: elements){
			UpdateElement(element);
		}
	}

	float CalcSizeY() override{
		if(utility.is_minimized.value()){
			return style.font_size.value();
		}

		float size= 0;
		for(auto &element: elements){
			size+= element->style.size.value().y +style.margin.value();
		}
		size+= style.font_size.value() +style.margin.value();
		return size;
	}
};

class RadioGroup: public GuiElement{
private:
	Panel panel;
	int ind= 0;

public:
	std::vector<std::shared_ptr<Checkbox>> checkboxes;

	RadioGroup(std::string text_, Utility utility_, std::vector<std::shared_ptr<Checkbox>> checkboxes_): panel(text_, utility_, {}){
		checkboxes= checkboxes_;
		for(auto &checkbox: checkboxes){
			panel.elements.push_back(checkbox);
		}

		panel.style.position.value()= style.position.value();
	}

	void Update(bool is_parent) override{
		if(ind< 2){
			panel.style= style;
			UpdateElements();
			for(auto &element: panel.elements){
				panel.ApplyStyle(element);
			}
			ind++;
		}
		if(panel.utility.can_minimize.value() && status== S_HOVERED_HEADER && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			panel.utility.is_minimized.value()= !panel.utility.is_minimized.value();
		}

		panel.style.position= style.position;
		panel.style.size= style.size;

		for(auto &element: panel.elements){
			auto checkbox = std::dynamic_pointer_cast<Checkbox>(element);
			bool old= *(checkbox->is_checked);
			checkbox->Update(false);
			if(old== false && *(checkbox->is_checked)== true){
				for(auto &elem: panel.elements){
					auto chbx = std::dynamic_pointer_cast<Checkbox>(elem);
					if(chbx!= checkbox)
					*(chbx->is_checked)= false;
				}
				break;
			}
		}
		panel.UpdateElements();
	}

	void Draw(bool is_parent) override{
		panel.Draw(false);
	}

	bool CheckCollision() override {
		return panel.CheckCollision();
	}

	float CalcSizeY() override{	
		return panel.CalcSizeY();
	}
};

class Selection: public GuiElement{
private:
	Panel panel;
	int ind= 0;

public:
	std::vector<std::shared_ptr<GuiElement>> elements;

	Selection(std::string text_, Utility utility_, std::vector<std::shared_ptr<GuiElement>> elements_): panel(text_, utility_, elements_){
		elements= elements_;
		panel.style.position.value()= style.position.value();
	}

	void Update(bool is_parent) override{
		if(ind< 2){
			panel.style= style;
			UpdateElements();
			for(auto &element: panel.elements){
				panel.ApplyStyle(element);
			}
			ind++;
		}
		panel.style.position= style.position;
		panel.style.size= style.size;

		panel.Update(false);

        for(auto &element: panel.elements){
            if(element->status== S_CLICKED){
                panel.utility.is_minimized.value()= true;
                break;
            }
        }
		if(status== S_HOVERED_HEADER && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			panel.utility.is_minimized.value()= false;
			
		}

		panel.UpdateElements();
	}

	void Draw(bool is_parent) override{
		panel.Draw(false);
	}

	bool CheckCollision() override {
		return panel.CheckCollision();
	}

	float CalcSizeY() override{	
		return panel.CalcSizeY();
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
	// and if so search the elements in panel to set hovered element's status
	void UpdateElementHovered(){
		Vector2 mouse= GetMousePosition();
		bool did_click_L= IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
		bool did_click_M= IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);
		bool clicked_once= false;

		for(int i= (int)panels.size() -1; i>= 0; i--){
			auto &panel= panels[i]; 
			// if the cursor is on the panel
			if((panel->style.position.value().x< mouse.x) && (mouse.x< (panel->style.position.value().x +panel->style.size.value().x)) &&
			   (panel->style.position.value().y< mouse.y) && (mouse.y< (panel->style.position.value().y +panel->style.size.value().y)) ){

				// if the cursor is on the element
				for(auto &element: panel->elements){
					if((element->style.position.value().x< mouse.x) && (mouse.x< (element->style.position.value().x +element->style.size.value().x)) &&
					   (element->style.position.value().y< mouse.y) && (mouse.y< (element->style.position.value().y +element->style.size.value().y)) && !clicked_once){
						element->status= did_click_L? S_CLICKED: S_HOVERED;
						clicked_once= true;
						if(element->CheckCollision()){
							if((element->style.position.value().x< mouse.x) && (mouse.x< (element->style.position.value().x +element->style.size.value().x)) &&
							   (element->style.position.value().y< mouse.y) && (mouse.y< (element->style.position.value().y +element->style.font_size.value()))){
									element->status= S_HOVERED_HEADER;
								}
							}
					}
					else{
						if(element->status!= S_DISABLED)
							element->status= S_NORMAL;
					}
				}

				if(!clicked_once){
					// if the cursor is on the panel header
					if((panel->style.position.value().y< mouse.y) && (mouse.y< (panel->style.position.value().y +panel->style.font_size.value())) ){
						panel->status= S_HOVERED_HEADER;
						if(did_click_M || did_click_L){
							auto temp= panel;
							panels.erase(panels.begin() +i);
							panels.push_back(temp);
						}
						clicked_once= true;
					}
					else if((mouse.x> panel->style.position.value().x +panel->style.size.value().x -(panel->style.padding.value()*2)) &&
							(mouse.y> panel->style.position.value().y +panel->style.size.value().y -(panel->style.margin.value()*2)) ){
						panel->status= S_HOVERED_CORNER;
					}
					// if the cursor is on the right border
					else if(mouse.x> panel->style.position.value().x +panel->style.size.value().x -(panel->style.padding.value()*2)){
						panel->status= S_HOVERED_RIGHT;
					}
					//if the cursor is on the bottom border
					else if(mouse.y> panel->style.position.value().y +panel->style.size.value().y -(panel->style.margin.value()*2)){
						panel->status= S_HOVERED_BOTTOM;
					}
					else{
						panel->status= S_HOVERED;
					}
				}
			}
			else{
				if(panel->status!= S_DISABLED)
					panel->status= S_NORMAL;
			}
		}
	}

	void Update(){
		UpdateElementHovered();
		for(auto &panel: panels){
			panel->Update(true);
			panel->UpdateElements();
		}
	}

	void Draw(){
		for(auto &panel: panels){
			panel->Draw(true);
		}
	}
};