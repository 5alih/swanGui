/*
* swanGui.h - a Single-Header library.
*
* Usage:
* #include "swanGui.h"
*
* License:
* The MIT License (MIT)
*
* Copyright (c) 2024 Salih
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
#include <cfloat>
#include <iomanip>

inline Color ui_background=		{25, 25, 25, 255};
inline Color ui_panel_body=		{17, 24, 32, 255};
inline Color ui_panel_header=   {9, 12, 15, 255};

inline Color ui_element_body=  {29, 38, 51, 255};
inline Color ui_element_hover= {44, 55, 70, 255};
inline Color ui_element_click= {10, 10, 10, 255};

inline Color ui_text_dark=	{255, 255, 255, 255};
inline Color ui_text_light= {175, 180, 190, 255};
inline Color ui_text_hover= {240, 245, 255, 255};
inline Color ui_text_highl= {210, 215, 225, 255};

inline Color ui_special=	{243, 169, 78, 255};
inline Color ui_special_h=	{255, 180, 100, 255};

inline const int font_size= 14;
inline const int element_padding= 3;
inline const float thumnnail_size= 32.0f;
inline const int grid_size= 20;

class Panel;
class Button;
class CheckBox;
class Slider;
class Comment;
class Thumbnail;
class ThumnailGif;
class Billboard;
class BillboardGif;
class CameraView3D;
class CameraView3DFill;
class ColorPicker;

std::string to_string(int value){
	std::ostringstream stream;
	stream << value;
	return stream.str();
}

inline std::string b2s(bool value){
	std::string is= value ? "Enabled" : "Disabled";
	return is;
}

class GuiElement {//_______________________________________________________________________________ GUI ELEMENTS ____________________________________________________________________________//
public:
	Vector2 m_position= {0, 0};
	Vector2 m_size= {0, 0};
	std::string m_text;
	Font m_font;
	bool m_is_visible= true;

	virtual void Update()= 0;
	virtual void Draw()= 0;

	void SetPosition(Vector2 pos){ m_position= pos; }
	void SetSize(Vector2 size){ m_size= size; }
	void SetFont(Font font){m_font= font;};

	bool IsMouseOver() const;
	bool IsMouseOverEx(Vector2 position, Vector2 size) const;
};

inline bool GuiElement::IsMouseOver() const{
	Vector2 mousePos= GetMousePosition();
	return (mousePos.x >= m_position.x && mousePos.x <= m_position.x + m_size.x && mousePos.y >= m_position.y && mousePos.y <= m_position.y + m_size.y);
}

inline bool GuiElement::IsMouseOverEx(Vector2 position, Vector2 size) const{
	Vector2 mousePos= GetMousePosition();
	return (mousePos.x >= position.x && mousePos.x <= position.x + size.x && mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

class Button: public GuiElement{
public:
	std::function<void()> m_call_back_function;
	bool m_is_special;

	Button(const std::string text, std::function<void()> call_back_function, bool is_special){
		m_text= text;
		m_call_back_function= call_back_function;
		m_is_special= is_special;
	}

	Button(const std::string text, std::function<void()> call_back_function){
		m_text= text;
		m_call_back_function= call_back_function;
		m_is_special= false;
	}

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(m_call_back_function){
				m_call_back_function();
			}
		}
	}

	void Draw() override{
		Color currentColor= ui_element_body;
		if(m_is_special){
			currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_special_h) : ui_special;
		}
		else{
			currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_element_hover) : ui_element_body;
		}
		//DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), currentColor);
		Rectangle rec= {static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_size.x), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);

		Vector2 pos= { (float)static_cast<int>(m_position.x + m_size.x/2 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};

		if(m_is_special){
			DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_panel_header);
		}
		else{
			DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_light);
		}
	}
};

class CheckBox: public GuiElement{
public:
	bool *m_is_true;

	CheckBox(std::string text, bool &is_true){
		m_text= text;
		m_is_true= &is_true;
	}

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			*m_is_true= !(*m_is_true);
		}
	}

	void Draw() override{		Color textColor= IsMouseOver() ? ui_text_hover : ui_text_light;

		// DrawRectangle(static_cast<int>(m_position.x + m_size.x/2), static_cast<int>(m_position.y), static_cast<int>(m_size.x/2), static_cast<int>(m_size.y), ui_element_body);

		Rectangle rec= {static_cast<float>(m_position.x + m_size.x/2), static_cast<float>(m_position.y), static_cast<float>(m_size.x/2), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, ui_element_body);

		Vector2 pos_val= { (float)static_cast<int>(m_position.x + m_size.x/2 + m_size.x/4 - MeasureText(b2s(*m_is_true).c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, b2s(*m_is_true).c_str(), pos_val, font_size, 2.0f, ui_text_light);
		Vector2 pos_text= { (float)static_cast<int>(m_position.x + m_size.x/4 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, textColor);
	}
};

inline bool updateSlider(int* target_val, bool is_mouse_over, bool is_active, int step_size= 1, int min= -INT_MAX, int max= INT_MAX, int max_length= 9){
	if(!target_val) return false;

	auto clamp_value= [&](int val){ 
		return std::max(min, std::min(val, max)); 
	};

	if(is_active){
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			Vector2 delta= GetMouseDelta();
			*target_val+= delta.x * step_size;
		}

		if(is_mouse_over){
			*target_val+= GetMouseWheelMove() *step_size;
		}

		std::string input= std::to_string(*target_val);
		int key= GetCharPressed();

		while(key > 0){
			if ((key>= KEY_ZERO) && (key <= KEY_NINE) && 
				(static_cast<int>(input.length()) < max_length)){
				input+= static_cast<char>(key);
			}
			else if(key== KEY_EQUAL || key== KEY_MINUS){
				*target_val= -*target_val;
				input= std::to_string(*target_val);
			}
			key= GetCharPressed();
		}

		if(IsKeyPressed(KEY_BACKSPACE) && !input.empty()){
			input.pop_back();
			if(input.empty() || (input.size()== 1 && input== "-")){
				input= "0";
			}
		}
		try{
			int new_value= std::stoi(input);
			if(new_value >= min && new_value <= max){
				*target_val= new_value;
			}
		}
		catch(const std::exception&) {}
	}
	*target_val= clamp_value(*target_val);

	return true;
}

class Slider: public GuiElement {
public:
	int *m_target_val;
	int m_step_size= 1;
	int m_min= -INT_MAX;
	int m_max=  INT_MAX;
	bool m_get_input= false;
	int m_maxLength= 9;

	Slider(std::string text, int &target_val, int step_size, int min, int max){
		m_text= text;
		m_target_val= &target_val;
		m_step_size= step_size;
		m_min= min;
		m_max= max;
	}

	Slider(std::string text, int &target_val, int step_size){
		m_text= text;
		m_target_val= &target_val;
		m_step_size= step_size;
	}

	Slider(std::string text, int &target_val){
		m_text= text;
		m_target_val= &target_val;
	}

	void Update() override {
		if(!m_target_val) return;

		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_get_input= true;
		}
		else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)){
			m_get_input= false;
		}

		if(m_get_input){
			int key= GetCharPressed();

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
				Vector2 delta= GetMouseDelta();
				*m_target_val+= delta.x * m_step_size;
			}
			else if(IsMouseOver()){
				*m_target_val+= GetMouseWheelMove() *m_step_size;
			}

			std::string input= std::to_string(*m_target_val);

			while(key > 0){
				if((key >= KEY_ZERO) && (key <= KEY_NINE) && (static_cast<int>(input.length()) < m_maxLength)){
					input+= static_cast<char>(key);
				}
				else if(key== KEY_EQUAL || key== KEY_MINUS){
					*m_target_val= -*m_target_val;
					input= to_string(*m_target_val);
				}
				key= GetCharPressed();
			}

			if(IsKeyPressed(KEY_BACKSPACE) && !input.empty()){
				input.pop_back();
				if(input.empty() || (input.size()== 1 && input== "-"))
					input.push_back('0');
			}

			try{
				int newValue= std::stoi(input);
				if(newValue >= m_min && newValue <= m_max){
					*m_target_val= newValue;
				}
			}catch (const std::exception &e) {}
		}
		*m_target_val= std::max(m_min, std::min(* m_target_val, m_max));
	}

	void Draw() override{
		Color textColor= IsMouseOver() ? ui_text_hover : ui_text_light;
		Color currentColor= m_get_input ? ui_element_hover : ui_element_body;
		Rectangle rec= {static_cast<float>(m_position.x + m_size.x/2), static_cast<float>(m_position.y), static_cast<float>(m_size.x/2), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);
		Vector2 pos_val= { (float)static_cast<int>(m_position.x + m_size.x/2 + m_size.x/4 - MeasureText(to_string(*m_target_val).c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, to_string(*m_target_val).c_str(), pos_val, font_size, 2.0f, ui_text_light);
		Vector2 pos_text= { (float)static_cast<int>(m_position.x + m_size.x/4 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, textColor);
	}
};

class SliderF : public GuiElement{
public:
	float *m_target_val;
	float m_step_size= 0.1f;
	float m_min= -FLT_MAX;
	float m_max= FLT_MAX;
	bool m_get_input= false;
	int m_maxLength= 9;
	std::string m_input_buffer;

	SliderF(std::string text, float &target_val, float step_size, float min, float max){
		m_text= text;
		m_target_val= &target_val;
		m_step_size= step_size;
		m_min= min;
		m_max= max;
		m_input_buffer= format_float(*m_target_val);
	}

	SliderF(std::string text, float &target_val, float step_size){
		m_text= text;
		m_target_val= &target_val;
		m_step_size= step_size;
		m_input_buffer= format_float(*m_target_val);
	}

	SliderF(std::string text, float &target_val){
		m_text= text;
		m_target_val= &target_val;
		m_input_buffer= format_float(*m_target_val);
	}

	void Update() override{
		if(!m_target_val) return;

		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_get_input= true;
		}
		else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)){
			m_get_input= false;
			m_input_buffer= format_float(*m_target_val);
		}

		if(m_get_input){
			int key= GetCharPressed();

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
				Vector2 delta= GetMouseDelta();
				*m_target_val+= delta.x * m_step_size;
				m_input_buffer= format_float(*m_target_val);
			}
		else if(IsMouseOver()){
				*m_target_val+= GetMouseWheelMove() * m_step_size;
				m_input_buffer= format_float(*m_target_val);
			}

			while (key > 0){
				if((key >= KEY_ZERO && key <= KEY_NINE) && (static_cast<int>(m_input_buffer.length()) < m_maxLength)){
					m_input_buffer+= static_cast<char>(key);
				}
		else if(key== KEY_PERIOD && m_input_buffer.find('.')== std::string::npos){
					m_input_buffer+= '.';
				}
		else if(key== KEY_MINUS && m_input_buffer.empty()){
					m_input_buffer= "-";
				}
				key= GetCharPressed();
			}
			if(IsKeyPressed(KEY_BACKSPACE) && !m_input_buffer.empty()){
				m_input_buffer.pop_back();
				if(m_input_buffer.empty() || (m_input_buffer.size()== 1 && m_input_buffer== "-")){
					m_input_buffer= "0";
				}
			}
			try{
				float newValue= std::stof(m_input_buffer);
				if(newValue >= m_min && newValue <= m_max){
					*m_target_val= newValue;
				}
			} catch (const std::exception &e){
				m_input_buffer= format_float(*m_target_val);
			}
		}
		*m_target_val= std::max(m_min, std::min(*m_target_val, m_max));
	}

	void Draw() override{
		Color textColor= IsMouseOver() ? ui_text_hover : ui_text_light;
		Color currentColor= m_get_input ? ui_element_hover : ui_element_body;
		Rectangle rec= {static_cast<float>(m_position.x + m_size.x / 2), static_cast<float>(m_position.y), static_cast<float>(m_size.x / 2), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);

		std::string display_value= m_get_input ? m_input_buffer : format_float(*m_target_val);
		Vector2 pos_val= {(float)static_cast<int>(m_position.x + m_size.x / 2 + m_size.x / 4 - MeasureText(display_value.c_str(), font_size) / 2),
						(float)static_cast<int>(m_position.y + m_size.y / 2 - font_size / 2.5)};
		DrawTextEx(m_font, display_value.c_str(), pos_val, font_size, 2.0f, ui_text_light);

		Vector2 pos_text= {(float)static_cast<int>(m_position.x + m_size.x / 4 - MeasureText(m_text.c_str(), font_size) / 2),
							(float)static_cast<int>(m_position.y + m_size.y / 2 - font_size / 2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, textColor);
	}

private:
	std::string format_float(float value, int precision= 2){
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(precision) << value;
		return oss.str();
	}
};

class InputBox: public GuiElement{
public:
	std::string *m_target_str;
	int m_max_length= 20;
	bool m_get_input= false;

	InputBox(std::string text, std::string &target_str, int max_length){
		m_text= text;
		m_target_str= &target_str;
		m_max_length= max_length;
	}

	InputBox(std::string text, std::string &target_str){
		m_text= text;
		m_target_str= &target_str;
	}

	void Update() override{
		if(!m_target_str) return;
		
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_get_input= true;
		}
		else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)){
			m_get_input= false;
		}

		if(m_get_input){
			int key= GetCharPressed();

			std::string input= *m_target_str;

			while(key > 0){
				if(static_cast<int>(input.length()) < m_max_length){
					input+= static_cast<char>(key);
				}
				key= GetCharPressed();
			}

			if(IsKeyPressed(KEY_BACKSPACE) && !input.empty()){
				input.pop_back();
			}
			*m_target_str= input;
		}
	}

	void Draw() override{
		Color textColor= IsMouseOver() ? ui_text_hover : ui_text_light;
		Color currentColor= m_get_input ? ui_element_hover : ui_element_body;

		Rectangle rec= {static_cast<float>(m_position.x + m_size.x/2), static_cast<float>(m_position.y), static_cast<float>(m_size.x/2), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);
		Vector2 pos_val= { (float)static_cast<int>(m_position.x + m_size.x/2 + m_size.x/4 - MeasureText((*m_target_str).c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, (*m_target_str).c_str(), pos_val, font_size, 2.0f, ui_text_light);
		Vector2 pos_text= { (float)static_cast<int>(m_position.x + m_size.x/4 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, textColor);
	}
};

class Comment: public GuiElement{
public:
	Comment(std::string text){
		m_text= text;
	}

	void Update() override{}

	void Draw() override{
		Vector2 pos= { (float)static_cast<int>(m_position.x), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_light);
	}
};

class Thumbnail: public GuiElement{
public:
	Texture2D *m_texture;
	std::function<void()> m_call_back_function;
	std::string m_text_button;

	Thumbnail(std::string text, Texture2D &texture, std::function<void()> call_back_function, std::string text_button){
		m_text= text;
		m_texture= &texture;
		m_call_back_function= call_back_function;
		m_text_button= text_button;
	}

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(m_call_back_function){
				m_call_back_function();
			}
		}
	}

	void Draw() override{
		Color currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_element_hover) : ui_element_body;
		//DrawRectangle(static_cast<int>(m_position.x + thumnnail_size + element_padding), static_cast<int>(m_position.y +m_size.y/2),
		//			  static_cast<int>(m_size.x - thumnnail_size - element_padding), static_cast<int>(m_size.y/2), currentColor);
		Rectangle rec= {static_cast<float>(m_position.x + thumnnail_size + element_padding), static_cast<float>(m_position.y +m_size.y/2),
						static_cast<float>(m_size.x - thumnnail_size - element_padding), static_cast<float>(m_size.y/2)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);
		Vector2 pos= { (float)static_cast<int>(m_position.x + thumnnail_size + element_padding + (m_size.x - thumnnail_size - element_padding)/2 - MeasureText(m_text_button.c_str(), font_size)/2),
					(float)static_cast<int>(m_position.y + 3*(m_size.y/4) - font_size/2.5)};
		DrawTextEx(m_font, m_text_button.c_str(), pos, font_size, 2.0f, ui_text_light);

		float scale= (m_texture->width> m_texture->height) ? thumnnail_size/(m_texture->width) : thumnnail_size/(m_texture->height);
		DrawTextureEx(*m_texture, m_position, 0.0f, scale, WHITE);

		Vector2 pos2= { (float)static_cast<int>(m_position.x + thumnnail_size + element_padding), (float)static_cast<int>(m_position.y + m_size.y/4 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos2, font_size, 2.0f, ui_text_light);
	}
};

class ThumbnailGif: public GuiElement{
public:
	Image m_image_anim;
	Texture2D m_texture_anim;
	std::function<void()> m_call_back_function;
	std::string m_text_button;
	std::string m_gif_path;
	int m_frames= 0;
	unsigned int m_next_frame= 0;
	int m_current_frame= 0;
	int m_frame_delay= 8;
	int m_frame_counter= 0;

	ThumbnailGif(std::string text, const std::string& gif_path, std::function<void()> call_back_function, std::string text_button, int frame_delay= 8){
		m_text= text;
		m_gif_path= gif_path;
		m_call_back_function= call_back_function;
		m_text_button= text_button;
		m_frame_delay= frame_delay;

		m_image_anim= LoadImageAnim(m_gif_path.c_str(), &m_frames);
		m_texture_anim= LoadTextureFromImage(m_image_anim);
	}

	~ThumbnailGif(){
		UnloadTexture(m_texture_anim);
		UnloadImage(m_image_anim);
	}

	void Update() override{
		m_frame_counter++;
		if(m_frame_counter>= m_frame_delay){
			m_current_frame++;
			if(m_current_frame>= m_frames) m_current_frame= 0;
				m_next_frame= m_image_anim.width *m_image_anim.height *4 *m_current_frame;

			UpdateTexture(m_texture_anim, ((unsigned char*)m_image_anim.data) +m_next_frame);
				m_frame_counter= 0;
		}
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(m_call_back_function){
				m_call_back_function();
			}
		}
	}

	void Draw() override{		Color currentColor= IsMouseOver() ? 
			(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_element_hover) : ui_element_body;
		// DrawRectangle(static_cast<int>(m_position.x + thumnnail_size + element_padding), static_cast<int>(m_position.y + m_size.y/2),
		// 			  static_cast<int>(m_size.x - thumnnail_size - element_padding), static_cast<int>(m_size.y/2), currentColor);

		Rectangle rec= {static_cast<float>(m_position.x + thumnnail_size + element_padding), static_cast<float>(m_position.y + m_size.y/2),
					static_cast<float>(m_size.x - thumnnail_size - element_padding), static_cast<float>(m_size.y/2)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);


		Vector2 pos= {(float)static_cast<int>(m_position.x + thumnnail_size + element_padding + (m_size.x - thumnnail_size - element_padding)/2 - MeasureText(m_text_button.c_str(), font_size)/2),
					(float)static_cast<int>(m_position.y + 3*(m_size.y/4) - font_size/2.5)};
		DrawTextEx(m_font, m_text_button.c_str(), pos, font_size, 2.0f, ui_text_light);

		float scale= (m_texture_anim.width> m_texture_anim.height) ? thumnnail_size/(float)m_texture_anim.width : thumnnail_size/(float)m_texture_anim.height;
		DrawTextureEx(m_texture_anim, m_position, 0.0f, scale, WHITE);

		Vector2 pos2= {(float)static_cast<int>(m_position.x +thumnnail_size +element_padding), (float)static_cast<int>(m_position.y +m_size.y/4 -font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos2, font_size, 2.0f, ui_text_light);
	}
};

class Billboard: public GuiElement{
public:
	Texture2D *m_texture;

	Billboard(Texture2D &texture){
		m_texture= &texture;
	}

	~Billboard(){
		UnloadTexture(*m_texture);
	}

	void Update() override{}

	void Draw() override{
		float scale= (m_texture->width> m_texture->height) ? (float)m_size.x/(m_texture->width) : (float)m_size.x/(m_texture->height);
		DrawTextureEx(*m_texture, m_position, 0.0f, scale, WHITE);
	}
};

class BillboardGif: public GuiElement{
public:
	Image m_image_anim;
	Texture2D m_texture_anim;
	int m_frames= 0;
	unsigned int m_next_frame= 0;
	int m_current_frame= 0;
	int *m_frame_delay;
	int m_frame_counter= 0;
	std::string m_gif_path;

	BillboardGif(const std::string& gif_path, int &frame_delay){
		m_gif_path= gif_path;
		m_frame_delay= &frame_delay;
		m_image_anim= LoadImageAnim(m_gif_path.c_str(), &m_frames);
		m_texture_anim= LoadTextureFromImage(m_image_anim);
	}

	~BillboardGif(){
		UnloadTexture(m_texture_anim);
		UnloadImage(m_image_anim);
	}

	void Update() override{
		m_frame_counter++;
		if(m_frame_counter >= *m_frame_delay){
			m_current_frame++;
			if(m_current_frame>= m_frames) m_current_frame= 0;
				m_next_frame= m_image_anim.width *m_image_anim.height *4 *m_current_frame;

			UpdateTexture(m_texture_anim, ((unsigned char*)m_image_anim.data) + m_next_frame);
				m_frame_counter= 0;
		}
	}

	void Draw() override{
		float scale= (m_texture_anim.width> m_texture_anim.height) ? m_size.x/(float)m_texture_anim.width : m_size.x/(float)m_texture_anim.height;
		DrawTextureEx(m_texture_anim, m_position, 0.0f, scale, WHITE);
	}
};

class CameraView3D : public GuiElement{
public:
	Camera3D *m_camera;
	RenderTexture m_render_texture;
	Color m_color;
	std::function<void(Camera3D&)> m_draw_scene_function;
	int m_width= 0;
	bool m_update_camera= false;
	bool m_is_calculated= false;

	CameraView3D(Camera3D &camera, std::function<void(Camera3D&)> draw_scene_function, Color background_color){
		m_camera= &camera;
		m_draw_scene_function= draw_scene_function;
		m_color= background_color;
	}

	~CameraView3D(){
		UnloadRenderTexture(m_render_texture);
	}

	void Update() override{
		if(m_is_calculated== false){
			m_width= m_size.x;
			m_render_texture= LoadRenderTexture(m_width, m_width);
			m_is_calculated= true;
		}

		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_update_camera= true;
			DisableCursor();
		}
		else if(IsKeyPressed(KEY_ESCAPE)){
			m_update_camera= false;
			EnableCursor();
		}

		if(m_update_camera){
			UpdateCamera(m_camera, CAMERA_CUSTOM);
		}
	}

	void Draw() override{
		BeginTextureMode(m_render_texture);
			ClearBackground(m_color);
				BeginMode3D(*m_camera);
				if(m_draw_scene_function) m_draw_scene_function(*m_camera);
			EndMode3D();
		EndTextureMode();

		Rectangle sourceRec= {0.0f, 0.0f, (float)m_render_texture.texture.width, (float)-m_render_texture.texture.height};
		DrawTextureRec(m_render_texture.texture, sourceRec, m_position, WHITE);
	}

	RenderTexture& GetRenderTexture(){
		return m_render_texture;
	}
};

class CameraView3DFill : public GuiElement{
public:
	Camera3D *m_camera;
	RenderTexture m_render_texture;
	Color m_color;
	std::function<void(Camera3D&)> m_draw_scene_function;
	bool m_update_camera= false;
	bool m_is_calculated= false;

	CameraView3DFill(Camera3D &camera, std::function<void(Camera3D&)> draw_scene_function, Color color){
		m_camera= &camera;
		m_draw_scene_function= draw_scene_function;
		m_color= color;
	}

	~CameraView3DFill(){
		UnloadRenderTexture(m_render_texture);
	}

	void Update() override{
		if(m_is_calculated== false){
			m_render_texture= LoadRenderTexture(m_size.x +12, m_size.y);	//altered "+12"
			m_is_calculated= true;
		}

		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_update_camera= true;
			DisableCursor();
		}
		else if(IsKeyPressed(KEY_ESCAPE)){
			m_update_camera= false;
			EnableCursor();
		}

		if(m_update_camera){
			UpdateCamera(m_camera, CAMERA_CUSTOM);
		}
	}

	void Draw() override{
		BeginTextureMode(m_render_texture);
			ClearBackground(m_color);
			BeginMode3D(*m_camera);
				if(m_draw_scene_function) m_draw_scene_function(*m_camera);
			EndMode3D();
		EndTextureMode();

		Rectangle sourceRec= {0.0f, 0.0f, (float)m_render_texture.texture.width, (float)-m_render_texture.texture.height};
		DrawTextureRec(m_render_texture.texture, sourceRec, (Vector2){m_position.x -6, m_position.y}, WHITE);	//altered "-6"
	}

	RenderTexture& GetRenderTexture(){
		return m_render_texture;
	}
};

class CameraView3DFillBorder : public GuiElement{
public:
	Camera3D *m_camera;
	RenderTexture m_render_texture;
	Color m_color;
	std::function<void(Camera3D&)> m_draw_scene_function;
	bool m_update_camera= false;
	bool m_is_calculated= false;

	CameraView3DFillBorder(Camera3D &camera, std::function<void(Camera3D&)> draw_scene_function, Color color){
		m_camera= &camera;
		m_draw_scene_function= draw_scene_function;
		m_color= color;
	}

	~CameraView3DFillBorder(){
		UnloadRenderTexture(m_render_texture);
	}

	void Update() override{
		if(m_is_calculated== false){
			m_render_texture= LoadRenderTexture(m_size.x, m_size.y);
			m_is_calculated= true;
		}

		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_update_camera= true;
			DisableCursor();
		}
		else if(IsKeyPressed(KEY_ESCAPE)){
			m_update_camera= false;
			EnableCursor();
		}

		if(m_update_camera){
			UpdateCamera(m_camera, CAMERA_CUSTOM);
		}
	}

	void Draw() override{
		BeginTextureMode(m_render_texture);
			ClearBackground(m_color);
			BeginMode3D(*m_camera);
				if(m_draw_scene_function) m_draw_scene_function(*m_camera);
			EndMode3D();
		EndTextureMode();

		Rectangle sourceRec= {0.0f, 0.0f, (float)m_render_texture.texture.width, (float)-m_render_texture.texture.height};
		DrawTextureRec(m_render_texture.texture, sourceRec, m_position, WHITE);
	}

	RenderTexture& GetRenderTexture(){
		return m_render_texture;
	}
};

class ColorPicker: public GuiElement{
public:
	Color *m_color;
	int m_r= 255;
	int m_g= 0;
	int m_b= 0;
	int m_a= 255;

	bool m_calculated= false;

	Vector2 m_slider_size= {m_size.x/4, font_size};
	Vector2 m_pos_r= {m_position.x + (m_size.x/4)*3, m_position.y + font_size + element_padding};
	Vector2 m_pos_g= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*2 + element_padding*2};
	Vector2 m_pos_b= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*3 + element_padding*3};
	Vector2 m_pos_a= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*4 + element_padding*4};

	bool m_is_active_r= false;
	bool m_is_active_g= false;
	bool m_is_active_b= false;
	bool m_is_active_a= false;

	Rectangle m_rec_r= {m_pos_r.x, m_pos_r.y, m_slider_size.x, m_slider_size.y};
	Rectangle m_rec_g= {m_pos_g.x, m_pos_g.y, m_slider_size.x, m_slider_size.y};
	Rectangle m_rec_b= {m_pos_b.x, m_pos_b.y, m_slider_size.x, m_slider_size.y};
	Rectangle m_rec_a= {m_pos_a.x, m_pos_a.y, m_slider_size.x, m_slider_size.y};


	ColorPicker(std::string text, Color &color){
		m_text= text;
		m_color= &color;

		m_r= color.r;
		m_g= color.g;
		m_b= color.b;
		m_a= color.a;
	}

	void CompleteCalculation(){
		m_slider_size= {m_size.x/4, font_size};
		m_pos_r= {m_position.x + (m_size.x/4)*3, m_position.y + font_size + element_padding};
		m_pos_g= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*2 + element_padding*2};
		m_pos_b= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*3 + element_padding*3};
		m_pos_a= {m_position.x + (m_size.x/4)*3, m_position.y + font_size*4 + element_padding*4};

		m_rec_r= {m_pos_r.x, m_pos_r.y, m_slider_size.x, m_slider_size.y};
		m_rec_g= {m_pos_g.x, m_pos_g.y, m_slider_size.x, m_slider_size.y};
		m_rec_b= {m_pos_b.x, m_pos_b.y, m_slider_size.x, m_slider_size.y};
		m_rec_a= {m_pos_a.x, m_pos_a.y, m_slider_size.x, m_slider_size.y};
	}

	void Update() override{
		if(!m_calculated) CompleteCalculation();

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(IsMouseOverEx(m_pos_r, m_slider_size)){
				m_is_active_r= true;
				m_is_active_g= false;	
				m_is_active_b= false;	
				m_is_active_a= false;	
			}
			else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER))		m_is_active_r= false;

			if(IsMouseOverEx(m_pos_g, m_slider_size)){
				m_is_active_r= false;
				m_is_active_g= true;	
				m_is_active_b= false;	
				m_is_active_a= false;	
			}
			else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER))		m_is_active_g= false;

			if(IsMouseOverEx(m_pos_b, m_slider_size)){
				m_is_active_r= false;
				m_is_active_g= false;	
				m_is_active_b= true;	
				m_is_active_a= false;	
			}
			else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER))		m_is_active_b= false;

			if(IsMouseOverEx(m_pos_a, m_slider_size)){
				m_is_active_r= false;
				m_is_active_g= false;	
				m_is_active_b= false;	
				m_is_active_a= true;	
			}
			else if((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER))		m_is_active_a= false;
		}

		updateSlider(&m_r, IsMouseOverEx(m_pos_r, m_slider_size), m_is_active_r, 1, 0, 255, 3);
		updateSlider(&m_g, IsMouseOverEx(m_pos_g, m_slider_size), m_is_active_g, 1, 0, 255, 3);
		updateSlider(&m_b, IsMouseOverEx(m_pos_b, m_slider_size), m_is_active_b, 1, 0, 255, 3);
		updateSlider(&m_a, IsMouseOverEx(m_pos_a, m_slider_size), m_is_active_a, 1, 0, 255, 3);

		m_color->r= m_r;
		m_color->g= m_g;
		m_color->b= m_b;
		m_color->a= m_a;
	}

	void Draw() override{
		std::string str;
		DrawRectangleGradientH(m_position.x, m_position.y, m_size.x/2- element_padding*3, m_size.y, WHITE, *m_color);
		DrawRectangleGradientV(m_position.x, m_position.y, m_size.x/2- element_padding*3, m_size.y, (Color){0, 0, 0, 0}, BLACK);

		Vector2 pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/4)*3 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, ui_text_light);

		DrawRectangleRounded(m_rec_r, 0.3f, 2, (m_is_active_r) ? ui_element_hover : ui_element_body);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*5 - MeasureText("R", font_size)/2), (float)static_cast<int>(m_pos_r.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, "R", pos_text, font_size, 2.0f, ui_text_light);
		str= to_string(m_r);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*7 - MeasureText(str.c_str(), font_size)/2), (float)static_cast<int>(m_pos_r.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, str.c_str(), pos_text, font_size, 2.0f, ui_text_light);

		DrawRectangleRounded(m_rec_g, 0.3f, 2, (m_is_active_g) ? ui_element_hover : ui_element_body);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*5 - MeasureText("G", font_size)/2), (float)static_cast<int>(m_pos_g.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, "G", pos_text, font_size, 2.0f, ui_text_light);
		str= to_string(m_g);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*7 - MeasureText(str.c_str(), font_size)/2), (float)static_cast<int>(m_pos_g.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, str.c_str(), pos_text, font_size, 2.0f, ui_text_light);

		DrawRectangleRounded(m_rec_b, 0.3f, 2, (m_is_active_b) ? ui_element_hover : ui_element_body);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*5 - MeasureText("B", font_size)/2), (float)static_cast<int>(m_pos_b.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, "B", pos_text, font_size, 2.0f, ui_text_light);
		str= to_string(m_b);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*7 - MeasureText(str.c_str(), font_size)/2), (float)static_cast<int>(m_pos_b.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, str.c_str(), pos_text, font_size, 2.0f, ui_text_light);

		DrawRectangleRounded(m_rec_a, 0.3f, 2, (m_is_active_a) ? ui_element_hover : ui_element_body);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*5 - MeasureText("A", font_size)/2), (float)static_cast<int>(m_pos_a.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, "A", pos_text, font_size, 2.0f, ui_text_light);
		str= to_string(m_a);
		pos_text= { (float)static_cast<int>(m_position.x + (m_size.x/8)*7 - MeasureText(str.c_str(), font_size)/2), (float)static_cast<int>(m_pos_a.y + m_slider_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, str.c_str(), pos_text, font_size, 2.0f, ui_text_light);
	}
};

class Panel: public GuiElement{//_____________________________________________________________________ PANEL ________________________________________________________________________________//
public:
	std::vector<std::shared_ptr<GuiElement>> m_elements;
	Font m_custom_font= GetFontDefault();
	int m_header_size= font_size;
	bool m_is_minimized= false;
	bool m_is_moving= false;
	int m_grid_size= grid_size;
	int m_sections= 1;
	bool m_has_header= true;
	int m_counter= 0;

	Panel(std::string text, Vector2 position, Vector2 size){
		m_text= text;
		SetPosition( (Vector2){position.x * m_grid_size, position.y * m_grid_size} );
		SetSize( (Vector2){size.x * m_grid_size, size.y * m_grid_size} );
	}

	Panel(std::string text, Vector2 position, Vector2 size, Font custom_font){
		m_text= text;
		SetPosition( (Vector2){position.x * m_grid_size, position.y * m_grid_size} );
		SetSize( (Vector2){size.x * m_grid_size, size.y * m_grid_size} );
		m_custom_font= custom_font;
	}

	Panel(std::string text, Vector2 position, Vector2 size, int sections, Font custom_font){
		m_text= text;
		SetPosition( (Vector2){position.x * m_grid_size, position.y * m_grid_size} );
		SetSize( (Vector2){size.x * m_grid_size, size.y * m_grid_size} );
		m_sections= sections;
		m_custom_font= custom_font;
	}

	Panel(std::string text, Vector2 position, Vector2 size, bool has_header, int sections, Font custom_font){
		m_text= text;
		SetPosition( (Vector2){position.x * m_grid_size, position.y * m_grid_size} );
		SetSize( (Vector2){size.x * m_grid_size, size.y * m_grid_size} );
		m_has_header= has_header;
		m_sections= sections;
		m_custom_font= custom_font;
	}

	void Update() override{
		if(m_has_header){
			if(IsMouseOverEx(m_position, (Vector2){m_size.x, (float)m_header_size}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
				m_is_minimized= !m_is_minimized;
			}
			if(m_is_moving== false && IsMouseOverEx(m_position, (Vector2){m_size.x, (float)m_header_size}) && IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
				m_is_moving= true;
			}
			else if(m_is_moving== true && (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))){
				Vector2 fixedPos;
				fixedPos.x= (int)(m_position.x / m_grid_size);
				fixedPos.y= (int)(m_position.y / m_grid_size);

				fixedPos.x= ( ((fixedPos.x *m_grid_size) -m_position.x) *(-1) < (((fixedPos.x + 1) *m_grid_size) -m_position.x )) ? fixedPos.x *m_grid_size : (fixedPos.x + 1) *m_grid_size;
				fixedPos.y= ( ((fixedPos.y *m_grid_size) -m_position.y) *(-1) < (((fixedPos.y + 1) *m_grid_size) -m_position.y )) ? fixedPos.y *m_grid_size : (fixedPos.y + 1) *m_grid_size;
					Vector2 delta;
				delta.x= fixedPos.x -m_position.x;
				delta.y= fixedPos.y -m_position.y;

				m_position= fixedPos;

				for(auto &element: m_elements){
					element->m_position.x+= delta.x;
					element->m_position.y+= delta.y;
				}
				m_is_moving= false;
			}

			if(m_is_moving){
				Vector2 delta= GetMouseDelta();
				m_position.x+= delta.x;
				m_position.y+= delta.y;

				for(auto &element: m_elements){
					element->m_position.x+= delta.x;
					element->m_position.y+= delta.y;
				}
			}
		}

		if(m_is_minimized== false){
			for(auto& element : m_elements){
				if(element->m_is_visible && (element->m_position.y + element->m_size.y)< (m_position.y + m_size.y))
					element->Update();
			}
		}

		float wheel_delta= GetMouseWheelMove();

		if(m_is_minimized==false && IsMouseOver() && wheel_delta!= 0){
			float delta= 0;
			if(wheel_delta< 0){
				for(int i= 0; i< m_sections; i++){
					bool changed_first_element= false;
					for(int j= i; j< (int)m_elements.size(); j+= m_sections){
						if(changed_first_element== false && m_elements[j]->m_is_visible== true){
							delta= m_elements[j]->m_size.y +element_padding;
							m_elements[j]->m_is_visible= false;
							m_elements[j]->m_position.y-= delta;
							changed_first_element= true;
						}
						if(m_elements[j]->m_is_visible== true)
							m_elements[j]->m_position.y-= delta;
					}
				}
			}
			else if(wheel_delta> 0){
				for(int i= 0; i< m_sections; i++){
					int counter= 0;
					for(int j= i; j< (int)m_elements.size(); j+= m_sections){
						if(m_elements[j]->m_is_visible== false){
							counter++;
						}
					}
					if(counter> 0){
						delta= m_elements[i +m_sections *(counter -1)]->m_size.y +element_padding;
						m_elements[i +m_sections *(counter -1)]->m_is_visible= true;
						for(int k= i; k<(int)m_elements.size(); k+= m_sections){
							if(m_elements[k]->m_is_visible== true)
								m_elements[k]->m_position.y+= delta;
						}
					}
				}
			}
		}
	}

	void Draw() override{
		if(m_is_minimized== false){
			Color tempColor= m_has_header ? ui_panel_body : ui_panel_header;
			DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), tempColor);
			DrawRectangleLines(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), ui_panel_header);
			for(auto& element : m_elements){
				if(element->m_is_visible && (element->m_position.y + element->m_size.y)< (m_position.y + m_size.y) && (element-> m_position.y > m_position.y))
					element->Draw();
			}
		}
		if(m_has_header){
			DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_header_size), ui_panel_header);
			Vector2 pos= { (float)static_cast<int>(m_position.x + element_padding), (float)static_cast<int>(m_position.y + m_header_size/2 - font_size/2.5)};
			DrawTextEx(m_custom_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_highl);
		}
	}

	template <typename T>
	void addElement(std::shared_ptr<T> element){
		static_assert(std::is_base_of<GuiElement, T>::value, "Element must derive from GuiElement");

		if(m_counter>= m_sections){
			m_counter= 0;
		}

		Vector2 newPosition= m_position;
		newPosition.x+= element_padding *2 +(m_counter *(m_size.x/ m_sections));
		newPosition.y+= (m_has_header ? element_padding +font_size : element_padding);
		int group= 0;
		for(const auto& elem : m_elements){
			if(group== m_counter)
				newPosition.y+= elem->m_size.y +element_padding;
			
			group++;
			if(group== m_sections) group= 0;
		}

		Vector2 newSize= m_size;
		newSize.x= m_size.x/m_sections;
		newSize.x-= element_padding *4;
		if constexpr (std::is_same<T, Thumbnail>::value || std::is_same<T, ThumbnailGif>::value){
			newSize.y= font_size *2 + element_padding;
		}
		else if constexpr (std::is_same<T, Billboard>::value || std::is_same<T, BillboardGif>::value || std::is_same<T, CameraView3D>::value){
			newSize.y= newSize.x;
		}
		else if constexpr (std::is_same<T, CameraView3DFill>::value){
			newSize.y= m_size.y -(newPosition.y -m_position.y) -element_padding;
		}
		else if constexpr (std::is_same<T, CameraView3DFillBorder>::value){
			newSize.y= m_size.y -(newPosition.y -m_position.y) -element_padding;
		}
		else if(std::is_same<T, ColorPicker>::value){
			newSize.y= (font_size + element_padding) *5;
		}
		else{
			newSize.y= font_size;
		}

		element->SetPosition(newPosition);
		element->SetSize(newSize);
		element->SetFont(m_custom_font);
		m_elements.push_back(element);

		m_counter++;
	}

	void removeElement(std::shared_ptr<GuiElement> element){
		m_elements.erase(std::remove(m_elements.begin(), m_elements.end(), element), m_elements.end());
	}
};

//might add drop right and drop left menus
class DropDown: public GuiElement{
public:
	bool m_is_selected= false;
	int m_extra_width= 0;
	int m_element_count= 0;
	Vector2 m_panel_pos;
	Vector2 m_panel_size;
	std::shared_ptr<Panel> *m_panel;

	DropDown(const std::string text, int extra_width, int element_count){
		m_text= text;
		m_extra_width= extra_width;
		m_element_count= element_count;

		// m_panel= std::make_shared<Panel>("dropdown-panel", m_panel_pos, m_panel_size, false, 1, m_font);
	}

	Vector2 GetPanelPos(){
		Vector2 temp;
		temp.x= m_panel_pos.x/20.0;
		temp.y= m_panel_pos.y/20.0;
		return temp;
	}

	Vector2 GetPanelSize(){
		Vector2 temp;
		temp.x= m_panel_size.x/20.0;
		temp.y= m_panel_size.y/20.0;
		return temp;
	}

	void PrintDimensions(){
		static bool did_print= false;
		if(!did_print){
			Vector2 pos= GetPanelPos();
			Vector2 size= GetPanelSize();
			std::cout<<"pos x: "<<pos.x<<std::endl;
			std::cout<<"pos y: "<<pos.y<<std::endl;
			std::cout<<"size x: "<<size.x<<std::endl;
			std::cout<<"size y: "<<size.y<<std::endl;
			did_print= true;
		}
		//size.y is +0.85 for each element
	}

	void SetPanel(std::shared_ptr<Panel> &panel){
		m_panel= &panel;
	}

	void Update() override{
		static int is_initialized= 0;
		if(is_initialized <10){
			m_panel_pos.x= m_position.x;
			m_panel_pos.y= (m_position.y +font_size +element_padding);
			m_panel_size.x= (m_size.x +m_extra_width);
			m_panel_size.y= (element_padding +((font_size +element_padding) *m_element_count));
			is_initialized++;
		}

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(IsMouseOver()){
				m_is_selected= !m_is_selected;
			}
			else if(!(m_is_selected && IsMouseOverEx(m_panel_pos, m_panel_size))){
				m_is_selected= false;
			}
		}
		if(m_is_selected){
			(*m_panel)->Update();
		}
	}

	void Draw() override{
		Color currentColor= IsMouseOver()  ? ui_element_hover : ui_element_body;

		Rectangle rec= {static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_size.x), static_cast<float>(m_size.y)};
		DrawRectangleRounded(rec, 0.3f, 2, currentColor);
		rec= {static_cast<float>(m_position.x +2), static_cast<float>(m_position.y +2), static_cast<float>(m_size.x -4), static_cast<float>(m_size.y -4)};
		DrawRectangleRounded(rec, 0.3f, 2, ui_panel_body);
		Vector2 pos= { (float)static_cast<int>(m_position.x + m_size.x/2 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_light);

		Vector2 pos2= {(float)static_cast<int>(m_position.x + m_size.x -10), pos.y +5};
		DrawTriangle( (Vector2){pos2.x, pos2.y}, (Vector2){pos2.x +3, pos2.y +5}, (Vector2){pos2.x +6, pos2.y}, ui_element_hover);

		if(m_is_selected){
			(*m_panel)->Draw();
		}
	}
};

class SwanGui{//_____________________________________________________________________________________ SWANGUI _______________________________________________________________________________//
public:
	std::vector<std::shared_ptr<Panel>> m_panels;

	SwanGui(){}

	void AddPanel(std::shared_ptr<Panel> panel){
		auto it= std::find(m_panels.begin(), m_panels.end(), panel);
		if(it== m_panels.end()){
			m_panels.push_back(panel);
		}
	}
	void RemovePanel(std::shared_ptr<Panel> panel){
		m_panels.erase(
			std::remove(m_panels.begin(), m_panels.end(), panel), 
			m_panels.end()
		);
	}

	void Update(){
		for(auto& panel : m_panels){
			panel->Update();
		}
	}

	void Draw(){
		for(auto& panel : m_panels){
			panel->Draw();
		}
	}
};

#endif // SWANGUI_H