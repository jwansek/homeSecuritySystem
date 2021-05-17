import serial
import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
import datetime

class SerialReader(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self,  *args, **kwargs)
        self.title("Home Security System")
        self.resizable(0, 0)

        self.lbl_state = tk.Label(self, font = "-weight bold")
        self.lbl_state.pack()

        self.txt_log = scrolledtext.ScrolledText(self)
        self.txt_log.pack(padx = 10, pady = 10)
        self.txt_log.config(state = tk.DISABLED)

        self.ser = serial.Serial("COM6", 115200, timeout=1)
        if not self.ser.isOpen():
            self.ser.open()

        self.txt_log.config(state = tk.NORMAL)
        with open("hsc.log", "r") as f:
            for line in f:
                self.txt_log.insert(tk.END, line)
                self.txt_log.see(tk.END)
        self.txt_log.config(state = tk.DISABLED)

        self.write_reading(self.get_reading())
        self.report_push(self.get_reading())
        self.lastreading = self.get_reading()
        self.after(10, self.do_after)

    def do_after(self):
        reading = self.get_reading()

        self.write_reading(self.get_reading())
        if reading != self.lastreading:
            self.report_push(reading)

        self.lastreading = reading
        self.after(10, self.do_after)

    def get_reading(self):
        try:
            return int(self.ser.readline()[-2:-1].decode())
        except ValueError:
            return self.get_reading()

    def report_push(self, reportInt):
        if reportInt == 0:
            reportText = "Locked"
        elif reportInt == 1:
            reportText = "Unlocked"
        elif reportInt == 2:
            reportText = "Triggered"
        else:
            reportText = "Alarm"

        txtformat = "[%s] - %s\n" % (datetime.datetime.now(), reportText)
        with open("hsc.log", "a") as f:
            f.write(txtformat)

        self.txt_log.config(state = tk.NORMAL)
        self.txt_log.insert(tk.END, txtformat)
        self.txt_log.config(state = tk.DISABLED)
        self.txt_log.see(tk.END)

    def write_reading(self, reportInt):
        if reportInt == 0:
            self.lbl_state.config(text = "Locked")
        elif reportInt == 1:
            self.lbl_state.config(text = "Unlocked")
        elif reportInt == 2:
            self.lbl_state.config(text = "Triggered")
        elif reportInt == 3:
            self.lbl_state.config(text = "Alarm")

if __name__ == "__main__":
    app = SerialReader()
    app.mainloop()

    # ser = serial.Serial("COM6", 115200, timeout=1)
    # if not ser.isOpen():
    #     ser.open()

    # print('COM6 is open: ', ser.isOpen())

    # while True:
        
    #     print(ser.readline())