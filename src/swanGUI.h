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
#include <cfloat>
#include <iomanip>

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