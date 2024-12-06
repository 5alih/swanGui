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

Color color_bg= {30, 30, 30, 255};
Color color_button= {50, 50, 50, 255};
Color color_hover= {60, 60, 60, 255};
Color color_click= {10, 10, 10, 255};
Color color_text= {150, 150, 150, 255};

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
	Color m_color_button= color_button;
	Color m_color_hover= color_hover;
	Color m_color_click= color_click;
	Color m_color_text= color_text;
	int m_font_size= font_size;

	void Update() override{
		if(IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(m_callBackFuntion){
				m_callBackFuntion();
			}
		}
	}
    void Draw() override{
		Color currentColor= IsMouseOver() ? (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? m_color_click : m_color_hover) : m_color_button;
		
		DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), currentColor);

		Vector2 pos= { (float)static_cast<int>(m_position.x + m_size.x/2 - MeasureText(m_text.c_str(), m_font_size)/2), (float)static_cast<int>(m_position.y + m_size.y/2 - m_font_size/2)};
		DrawTextEx(m_font, m_text.c_str(), pos, m_font_size, 2.0f, m_color_text);
	}

	Button(const std::string text, std::function<void()> callBackFunction){
		m_text= text;
		m_callBackFuntion= callBackFunction;
	}
};

class Panel: public GuiElement{//___________________________________________________________________________________________________________________________________________________//
public:
	std::vector<std::shared_ptr<GuiElement>> m_elements;
	Font m_custom_font= GetFontDefault();
	Color m_color_bg= color_bg;

	Panel(Vector2 position, Vector2 size){
		SetPosition(position);
		SetSize(size);
	}

	Panel(Vector2 position, Vector2 size, Font custom_font){
		SetPosition(position);
		SetSize(size);
		m_custom_font= custom_font;
	}

	void Update() override;
	void Draw() override;

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

void Panel::Update(){
    for(auto& element : m_elements){
        element->Update();
    }
}

void Panel::Draw(){
    DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y), m_color_bg);
    for(auto& element : m_elements){
        element->Draw();
    }
}

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