#ifndef GUI_H
#define GUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Output.H>
#include <string>


class GUI{
public: 
    Fl_Window *window;
    Fl_Button *button;
    Fl_Output *display;

    GUI(int w, int h, const char *title){
        window = new Fl_Window(w, h, title);
        
        button = new Fl_Button(20, 20, 80, 30, "Start");
        display = new Fl_Output(120, 20, 100, 30, "Time: ");

        window -> end();
    }   

    void show(int argc, char **argv){
        window->show(argc, argv);
    }
};   

int run_gui(int argc, char **argv){
    Fl_Window window(300, 200, "Timer");
    Fl_Button button(100,70,100,40, "Start");
    window.end();
    window.show(argc, argv);
    return Fl::run();
}

#endif