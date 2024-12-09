![swanGui](https://github.com/user-attachments/assets/43ec2c5c-dfe7-405c-9088-fd1b4abfa510)
# SwanGui: A Lightweight Raylib-based GUI Library

## Overview

SwanGui is a single-header C++ GUI library designed for use with Raylib, providing a flexible and extensible user interface framework. The library supports various GUI elements and allows for easy creation of panels and interactive components.

## Features

### Preview

![preview](https://github.com/user-attachments/assets/4228af8f-b8ba-4de2-bd4f-8743681ad9c2)
NOTE: The size, position, color and font of panels are fully customisable.
      This is just a screenshot, there is a lot more to see for yourself.

### Supported GUI Elements

-   Buttons
-   Checkboxes
-   Sliders
-   Comments
-   Thumbnails (Static and Animated)
-   Billboards (Static and Animated)
-   3D Camera Views

### Key Components

-   `SwanGui`: Main management class for panels
-   `Panel`: Container for GUI elements
-   Various GUI element classes inheriting from `GuiElement`
-   GUI elements are fully compatible with each other

## Usage Example

```cpp
// Create a SwanGui instance
SwanGui gui;

// Create a panel
auto panel = std::make_shared<Panel>({50,  50},  {300,  400});

// Add elements to the panel
panel->addElement(std::make_shared<Button>("Click Me",  []()  {  /* callback */  }));
panel->addElement(std::make_shared<Slider>("Value", myVariable,  1,  0,  100));

// Add panel to the GUI
gui.AddPanel(panel);

// In your main loop
gui.Update();
gui.Draw();`
```
## To-Do List

### Completed Features

-   Basic GUI element classes (Button, CheckBox, Slider, Comment)
-   Thumbnail support (static and animated)
-   Billboard support (static and animated)
-   3D Camera View integration
-   Panel management system
-   Flexible element positioning
-   Color theming
-   Font customization

### Pending Features

-   Implement panel carrying (panels can be grabbed and moved in a grid)
-   Add scrolling functionality to panels
-   Create super panels that can hold other panels
-   Develop grid-based panels (can expand vertically)
-   Enhance thumbnails with multiple buttons (e.g., [image: select, apply])
-   Allow multiple variables control with a single slider
-   Implement dropdown menus
-   Add dropdown predefined values to buttons and sliders

### Potential Improvements

-   Add error handling and input validation
-   Implement more sophisticated layout management
-   Create a more robust theming system
-   Add accessibility features
-   Develop comprehensive documentation and examples
-   Create unit tests
-   Optimize performance for larger numbers of GUI elements

## Dependencies

-   Raylib

## Limitations

-   Currently tied closely to Raylib
-   Limited advanced layout capabilities
-   Minimal built-in styling options
## License
SwanGui is licensed under the MIT License
