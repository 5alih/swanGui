#ifndef SWANGUI_H
#define SWANGUI_H

//TO DO:
//	add handle/ header to panels (panels can be minimized)
//	add carrying panels (panels can be grabbed and moved in a grid)
//	add scrolling to panels
//	add super panels that can hold other panels (panels can be placed in a super panel)
//	add panels that are grid based (can expand vertically)
//	add multi functionality to thumnails (can have 2 buttons instead of 1) example: [image: select, apply] 
//	add multiple variables control to sliders (more than 1 variable can be controlled with only 1 slider) example: [position: x, y, z]
//	add dropdown menus
//	add dropdown predefined values to the buttons and sliders

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "raylib.h"

Color ui_background= 	{25, 25, 25, 255};
Color ui_panel_body= 	{35, 35, 35, 255};
Color ui_panel_header= 	{15, 15, 15, 255};
Color ui_element_body=	{65, 65, 65, 255};
Color ui_element_hover=	{50, 50, 50, 255};
Color ui_element_click=	{10, 10, 10, 255};
Color ui_text_dark=	 	{40, 40, 40, 255};
Color ui_text_light= {195, 195, 195, 255};
Color ui_text_hover= {245, 245, 245, 255};
Color ui_text_highl= {210, 210, 210, 255};
Color ui_special=		{0, 85, 155, 255};
Color ui_special_h=		{0, 65, 135, 255};

const int font_size= 14;
const int element_padding= 3;
const float thumnnail_size= 32.0f;

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

std::string to_string(int value){
	std::ostringstream stream;
	stream << value;
	return stream.str();
}

std::string b2s(bool value){
	std::string is= value ? "Enabled" : "Disabled";
	return is;
}

class GuiElement {//_______________________________________________________________________________ GUI ELEMENTS ____________________________________________________________________________//
public:
	Vector2 m_position= {0, 0};
	Vector2 m_size= {0, 0};
	std::string m_text;
	Font m_font;
	bool m_isVisible= true;

	virtual void Update()= 0;
	virtual void Draw()= 0;

	void SetPosition(Vector2 pos){ m_position= pos; }
	void SetSize(Vector2 size){ m_size= size; }
	void SetFont(Font font){m_font= font;};

	bool IsMouseOver() const;
	bool IsMouseOverEx(Vector2 position, Vector2 size) const;
};

bool GuiElement::IsMouseOver() const{
	Vector2 mousePos= GetMousePosition();
	return (mousePos.x >= m_position.x && mousePos.x <= m_position.x + m_size.x && mousePos.y >= m_position.y && mousePos.y <= m_position.y + m_size.y);
}

bool GuiElement::IsMouseOverEx(Vector2 position, Vector2 size) const{
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
		
		DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), currentColor);
		Vector2 pos= { (float)static_cast<int>(m_position.x + m_size.x/2 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};

		if(m_is_special){
			DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_highl);
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

	void Draw() override{
		Color textColor= IsMouseOver() ? ui_text_hover : ui_text_light;

		DrawRectangle(static_cast<int>(m_position.x + m_size.x/2), static_cast<int>(m_position.y), static_cast<int>(m_size.x/2), static_cast<int>(m_size.y), ui_element_body);
		Vector2 pos_val= { (float)static_cast<int>(m_position.x + m_size.x/2 + m_size.x/4 - MeasureText(b2s(*m_is_true).c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, b2s(*m_is_true).c_str(), pos_val, font_size, 2.0f, ui_text_light);
		
		Vector2 pos_text= { (float)static_cast<int>(m_position.x + m_size.x/4 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, m_text.c_str(), pos_text, font_size, 2.0f, textColor);
	}
};

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

		if(m_get_input) {
			int key= GetCharPressed();

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
				Vector2 delta = GetMouseDelta();
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

		DrawRectangle(static_cast<int>(m_position.x + m_size.x/2), static_cast<int>(m_position.y), static_cast<int>(m_size.x/2), static_cast<int>(m_size.y), currentColor);
		Vector2 pos_val= { (float)static_cast<int>(m_position.x + m_size.x/2 + m_size.x/4 - MeasureText(to_string(*m_target_val).c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2.5)};
		DrawTextEx(m_font, to_string(*m_target_val).c_str(), pos_val, font_size, 2.0f, ui_text_light);
		
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
		
		DrawRectangle(static_cast<int>(m_position.x + thumnnail_size + element_padding), static_cast<int>(m_position.y +m_size.y/2),
					  static_cast<int>(m_size.x - thumnnail_size - element_padding), static_cast<int>(m_size.y/2), currentColor);
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

	void Draw() override{
		Color currentColor= IsMouseOver() ? 
			(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_element_hover) : ui_element_body;
		
		DrawRectangle(static_cast<int>(m_position.x + thumnnail_size + element_padding), static_cast<int>(m_position.y + m_size.y/2),
					  static_cast<int>(m_size.x - thumnnail_size - element_padding), static_cast<int>(m_size.y/2), currentColor);

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
		m_gif_path = gif_path;
		m_frame_delay = &frame_delay;
		
		m_image_anim = LoadImageAnim(m_gif_path.c_str(), &m_frames);
		
		m_texture_anim = LoadTextureFromImage(m_image_anim);
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
	Camera3D m_camera;
	RenderTexture m_render_texture;
	std::function<void(Camera3D&)> m_draw_scene_function;
	int m_width= 0;
	bool m_update_camera= false;

	CameraView3D(Camera3D camera, int width, std::function<void(Camera3D&)> draw_scene_function){
		m_camera= camera;

		m_width= width -element_padding *4;
		m_render_texture= LoadRenderTexture(m_width, m_width);
		m_draw_scene_function= draw_scene_function;
	}

	~CameraView3D(){
		UnloadRenderTexture(m_render_texture);
	}

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_update_camera= true;
			DisableCursor();
		}
		else if(m_update_camera && ((!IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE))){
			m_update_camera= false;
			EnableCursor();
		}

		if(m_update_camera){
			UpdateCamera(&m_camera, CAMERA_CUSTOM);
		}
	}

	void Draw() override{
		BeginTextureMode(m_render_texture);
			ClearBackground(BLACK);
			
			BeginMode3D(m_camera);
				if(m_draw_scene_function) m_draw_scene_function(m_camera);
			EndMode3D();
		EndTextureMode();

		Rectangle sourceRec= {0.0f, 0.0f, (float)m_render_texture.texture.width, (float)-m_render_texture.texture.height};
		DrawTextureRec(m_render_texture.texture, sourceRec, m_position, WHITE);
	}

	Camera3D& GetCamera() {
		return m_camera;
	}

	RenderTexture& GetRenderTexture() {
		return m_render_texture;
	}
};

class Panel: public GuiElement{//_____________________________________________________________________ PANEL ________________________________________________________________________________//
public:
	std::vector<std::shared_ptr<GuiElement>> m_elements;
	Font m_custom_font= GetFontDefault();
	int m_header_size= font_size;
	bool m_is_minimized= false;

	Panel(std::string text, Vector2 position, Vector2 size){
		m_text= text;
		SetPosition(position);
		SetSize(size);
	}

	Panel(std::string text, Vector2 position, Vector2 size, Font custom_font){
		m_text= text;
		SetPosition(position);
		SetSize(size);
		m_custom_font= custom_font;
	}

	void Update() override{
		if(IsMouseOverEx(m_position, (Vector2){m_size.x, (float)m_header_size}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			m_is_minimized= !m_is_minimized;
		}

		if(m_is_minimized== false){
			for(auto& element : m_elements){
				element->Update();
			}
		}
	}

	void Draw() override{
		if(m_is_minimized== false){
			DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), ui_panel_body);
			for(auto& element : m_elements){
				element->Draw();
			}
		}
		DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_header_size), ui_panel_header);
		Vector2 pos= { (float)static_cast<int>(m_position.x + element_padding), (float)static_cast<int>(m_position.y + m_header_size/2 - font_size/2.5)};
		DrawTextEx(m_custom_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_light);
	}

	template <typename T>
	void addElement(std::shared_ptr<T> element) {
		static_assert(std::is_base_of<GuiElement, T>::value, "Element must derive from GuiElement");

		Vector2 newPosition = m_position;
		newPosition.x += element_padding *2;
		newPosition.y += element_padding +font_size;
		for(const auto& elem : m_elements) {
			newPosition.y += elem->m_size.y + element_padding;
		}

		Vector2 newSize= m_size;
		newSize.x-= element_padding *4;
		if constexpr (std::is_same<T, Thumbnail>::value || std::is_same<T, ThumbnailGif>::value){
			newSize.y= font_size *2 + element_padding;
		}
		else if constexpr (std::is_same<T, Billboard>::value || std::is_same<T, BillboardGif>::value || std::is_same<T, CameraView3D>::value){
			newSize.y= newSize.x;
		}
		else{
			newSize.y= font_size;
		}

		element->SetPosition(newPosition);
		element->SetSize(newSize);
		element->SetFont(m_custom_font);
		m_elements.push_back(element);
	}

	void removeElement(std::shared_ptr<GuiElement> element){
		m_elements.erase(std::remove(m_elements.begin(), m_elements.end(), element), m_elements.end());
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