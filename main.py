import os
import sys
import json
import traceback
from datetime import datetime

log_name = f"{datetime.now()}".replace(" ", "_").replace(":", "")
log_file = open(f"logs/{log_name}.txt", 'w+')
sys.stdout = log_file
sys.stderr = log_file

from tkinter import *
import tkinter.messagebox as msgbox


def run_python(func_name, *args, **kwargs):
    try:
        namespace = globals()
        if func_name in namespace:
            func = namespace[func_name]
        else:
            return "{'code': 0, 'message': '[from function run_python] Cannot find requested function in the module.'}"
        result = func(*args, **kwargs)
        if result is not None:
            if not (isinstance(result, dict) and 'code' in result and 'message' in result):
                result = {'code': 0, 'message': result}
            result = json.dumps(result)
        else:
            result = "{'code': 0, 'message': 'None'}"
    except:
        message = traceback.format_exc()
        print(message)
        result = json.dumps({'code': -1, 'message': message})
    return result


def run_qtconsole():
    from qtconsole.qtconsoleapp import main
    main()


def test_msgbox():
    msgbox.showinfo("MessageBox", "Hello world")


def test_gui():
    print(3, 5)
    tk = Tk()
    tk.title('길이 변환기')

    def Ft2Cm():
        ft2cm = entry1.get()
        entry2.delete(0,"end")
        entry2.insert(0,round(float(ft2cm)*30.48,4))

    def Cm2Ft():
        cm2ft = entry2.get()
        entry1.delete(0,"end")
        entry1.insert(0,round(float(cm2ft)/30.48,4))

    label1 = Label(tk,text='피트(ft)').grid(row=0, column=0)
    label2 = Label(tk,text='센티미터(cm)').grid(row=1,column=0)

    # 각 단위 입력받는 부분 만들기
    entry1 = Entry(tk)
    entry2 = Entry(tk)


    entry1.grid(row=0,column=1)
    entry2.grid(row=1,column=1)

    btn1 = Button(tk,text='ft->cm',bg='black',fg='white',command=Ft2Cm).grid(row=2,column=0)
    btn2 = Button(tk,text='cm->ft',bg='black',fg='white',command=Cm2Ft).grid(row=2,column=1)

    tk.mainloop()
    return "Run Success!"


def test_multiply(a, b):
    msgbox.showinfo("multiply", f"Will compute {a} times {b}")
    c = 0
    for i in range(0, int(a)):
        c = c + int(b)
    msgbox.showinfo("multiply", f"Result is {c}")
    return c


def test_pyside6():
    from pc_task_monitor_pyside6 import QApplication, WindowClass
    app = QApplication(sys.argv)
    myWindow = WindowClass()

    myWindow.show()
    return app.exec()


def run_pip(option, *args):
    msgbox.showinfo("pip", f"start {option} {args}")
    try:
        from pip._internal import main as pipmain
    except ImportError:
        from pip import main as pipmain
    code = pipmain([option, "--trusted-host", "pypi.python.org", "--trusted-host", "files.pythonhosted.org", "--trusted-host", "pypi.org", *args])
    msgbox.showinfo("pip", f"setup finished - {code}")
