#ifndef SWANGUI_H
#define SWANGUI_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include "raylib.h"

Color ui_background= 	{25, 25, 25, 255};
Color ui_panel_body= 	{35, 35, 35, 255};
Color ui_element_body=	{65, 65, 65, 255};
Color ui_element_hover=	{50, 50, 50, 255};
Color ui_element_click=	{10, 10, 10, 255};
Color ui_text_light= {180, 180, 180, 255};
Color ui_text_dark=	 {180, 180, 180, 255};
Color ui_text_hover= {200, 200, 200, 255};
Color ui_special=  	 {255, 211, 105, 255};
Color ui_special_h=  {255, 220, 145, 255};

const int font_size= 12;
const int element_padding= 5;

class Panel;
class Button;
//class CheckBox;
//class TextBox;
//class Slider;
//class ImageBox;
//class GifBox;
//class CameraView2D;
//class CameraView3D;

class GuiElement {//________________________________________________________________________________________________________________________________________________________________//
public:
	Vector2 m_position= {0, 0};
	Vector2 m_size= {0, 0};
	Font m_font;
	bool m_isVisible= true;

	virtual void Update()= 0;
	virtual void Draw()= 0;

	void SetPosition(Vector2 pos){ m_position= pos; }
	void SetSize(Vector2 size){ m_size= size; }
	void SetFont(Font font){m_font= font;};

	bool IsMouseOver() const;
};

bool GuiElement::IsMouseOver() const{
	Vector2 mousePos= GetMousePosition();

	return (mousePos.x >= m_position.x && mousePos.x <= m_position.x + m_size.x && mousePos.y >= m_position.y && mousePos.y <= m_position.y + m_size.y);
}

class Button: public GuiElement{//__________________________________________________________________________________________________________________________________________________//
public:
	std::string m_text;
	std::function<void()> m_callBackFuntion;
	bool m_isHighlighted;

	Button(const std::string text, std::function<void()> callBackFunction, bool isHighlighted){
		m_text= text;
		m_callBackFuntion= callBackFunction;
		m_isHighlighted= isHighlighted;
	}

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(m_callBackFuntion){
				m_callBackFuntion();
			}
		}
	}

    void Draw() override{
		Color currentColor= ui_element_body;
		if(m_isHighlighted){
			currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_special_h) : ui_special;
		}
		else{
			currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? ui_element_click : ui_element_hover) : ui_element_body;
		}
		
		DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), currentColor);
		Vector2 pos= { (float)static_cast<int>(m_position.x + m_size.x/2 - MeasureText(m_text.c_str(), font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - font_size/2)};
		
		if(m_isHighlighted){
			DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_dark);
		}
		else{
			DrawTextEx(m_font, m_text.c_str(), pos, font_size, 2.0f, ui_text_light);
		}
	}
};

class Panel: public GuiElement{//___________________________________________________________________________________________________________________________________________________//
public:
	std::vector<std::shared_ptr<GuiElement>> m_elements;
	Font m_custom_font= GetFontDefault();

	Panel(Vector2 position, Vector2 size){
		SetPosition(position);
		SetSize(size);
	}

	Panel(Vector2 position, Vector2 size, Font custom_font){
		SetPosition(position);
		SetSize(size);
		m_custom_font= custom_font;
	}

	void Update() override{
		for(auto& element : m_elements){
			element->Update();
		}
	}

	void Draw() override{
		DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), ui_panel_body);
		for(auto& element : m_elements){
			element->Draw();
		}
	}

	template <typename T>
	void addElement(std::shared_ptr<T> element) {
		static_assert(std::is_base_of<GuiElement, T>::value, "Element must derive from GuiElement");

		Vector2 newPosition = m_position;
		newPosition.x += element_padding;
		newPosition.y += element_padding;
		for(const auto& elem : m_elements) {
			newPosition.y += elem->m_size.y + element_padding;
		}

		Vector2 newSize= m_size;
		newSize.x -= element_padding *2;
		newSize.y= font_size *1.25;

		element->SetPosition(newPosition);
		element->SetSize(newSize);
		element->SetFont(m_custom_font);
		m_elements.push_back(element);
	}

	void removeElement(std::shared_ptr<GuiElement> element){
		m_elements.erase(std::remove(m_elements.begin(), m_elements.end(), element), m_elements.end());
	}
};

class SwanGui{//____________________________________________________________________________________________________________________________________________________________________//
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