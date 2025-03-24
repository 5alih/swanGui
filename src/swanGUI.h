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

inline Vector2 g_mouse_position= GetMousePosition();
inline int g_font_size= 14;

inline Color hex(const std::string &hex_code){
	std::string m_hex= hex_code;
    if(m_hex[0]== '#'){
        m_hex.erase(0, 1);
    }
    int r= 0, g= 0, b= 0;
    if(m_hex.length()>= 6){
        sscanf(m_hex.c_str(), "%02x%02x%02x", &r, &g, &b);
    }
    return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
}

enum enum_position{
	P_NORMAL,
	p_FIXED,
	P_RELATIVE,
};

struct Position{
public:
	enum_position type;
	Vector2 position;
};

struct Style{
public:
	std::optional<Color> color= hex("#f5f5f5"); 
	std::optional<Color> background_color= hex("#131313");
	std::optional<Color> border_color= hex("#202020");
	std::optional<int> font_size= g_font_size;
	std::optional<Position> position= (Position){ P_NORMAL, (Vector2){0, 0} };
};

enum enum_status{
	S_NORMAL,
	S_HOVERED,
	S_CLICKED,
	S_DISABLED,
};

class GuiElement{
public:
	Vector2 m_position= {0, 0};
	Vector2 m_size= {0, 0};
	std::string m_text;
	Font m_font;
	enum_status m_status= S_NORMAL;
	Style style;

	virtual void Update()= 0;
	virtual void Draw()= 0;

	void SetPosition(Vector2 position){ m_position= position; }
	void SetSize(Vector2 size){ m_size= size; }
	void SetFont(Font font){ m_font= font; }
};



class Panel{
public:
	void Update(){}
	void Draw(){}
};

class SwanGui{
public:
	std::vector<std::shared_ptr<Panel>> m_panels;
	
	SwanGui(){}

	void AddPanel(std::shared_ptr<Panel> panel){
		auto it= std::find(m_panels.begin(), m_panels.end(), panel);
		if(it== m_panels.end()){
			m_panels.push_back(panel);
		}
	}

	void Update(){
		for(auto &panel: m_panels){
			panel->Update();
		}
	}

	void Draw(){
		for(auto &panel: m_panels){
			panel->Draw();
		}
	}
};

#endif // SWANGUI_H