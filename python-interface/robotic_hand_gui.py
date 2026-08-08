import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import threading
import time
import os


BAUDRATE = 9600

STUDENTS = "LORENZO MAZZANTE - MARTIN SCORZA"

PROJECT_TITLE = "Projet NF15 - Pilotage UART de la main robotique"

UTC_LOGO_FILE = "utc_logo.png"


class HandControlGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("NF15 - Robotic Hand UART Control")
        self.root.geometry("900x600")

        self.ser = None
        self.busy = False
        self.utc_logo = None

        self.build_ui()
        self.refresh_ports()

    def build_ui(self):
        # ---------- Header frame ----------
        header_frame = ttk.Frame(self.root)
        header_frame.pack(fill="x", padx=10, pady=(10, 5))

        title_frame = ttk.Frame(header_frame)
        title_frame.pack(side="left", fill="x", expand=True)

        title_label = ttk.Label(
            title_frame,
            text=PROJECT_TITLE,
            font=("Arial", 16, "bold")
        )
        title_label.pack(anchor="w")

        students_label = ttk.Label(
            title_frame,
            text=f"Étudiants : {STUDENTS}",
            font=("Arial", 11)
        )
        students_label.pack(anchor="w", pady=(4, 0))

        logo_path = os.path.join(os.path.dirname(__file__), UTC_LOGO_FILE)

        if os.path.exists(logo_path):
            try:
                self.utc_logo = tk.PhotoImage(file=logo_path)

                
                if self.utc_logo.width() > 180:
                    factor = max(1, self.utc_logo.width() // 180)
                    self.utc_logo = self.utc_logo.subsample(factor, factor)

                logo_label = ttk.Label(header_frame, image=self.utc_logo)
                logo_label.pack(side="right", padx=10)

            except tk.TclError:
                logo_label = ttk.Label(
                    header_frame,
                    text="UTC",
                    font=("Arial", 18, "bold")
                )
                logo_label.pack(side="right", padx=10)
        else:
            logo_label = ttk.Label(
                header_frame,
                text="UTC",
                font=("Arial", 18, "bold")
            )
            logo_label.pack(side="right", padx=10)

        # ---------- Connection frame ----------
        conn_frame = ttk.LabelFrame(self.root, text="UART connection")
        conn_frame.pack(fill="x", padx=10, pady=10)

        ttk.Label(conn_frame, text="Port:").pack(side="left", padx=5)

        self.port_var = tk.StringVar()
        self.port_box = ttk.Combobox(
            conn_frame,
            textvariable=self.port_var,
            width=15,
            state="readonly"
        )
        self.port_box.pack(side="left", padx=5)

        self.refresh_button = ttk.Button(
            conn_frame,
            text="Refresh",
            command=self.refresh_ports
        )
        self.refresh_button.pack(side="left", padx=5)

        self.connect_button = ttk.Button(
            conn_frame,
            text="Connect",
            command=self.connect_uart
        )
        self.connect_button.pack(side="left", padx=5)

        self.disconnect_button = ttk.Button(
            conn_frame,
            text="Disconnect",
            command=self.disconnect_uart,
            state="disabled"
        )
        self.disconnect_button.pack(side="left", padx=5)

        self.status_label = ttk.Label(conn_frame, text="Disconnected")
        self.status_label.pack(side="left", padx=15)

        # ---------- Command frame ----------
        cmd_frame = ttk.LabelFrame(self.root, text="Hand commands")
        cmd_frame.pack(fill="x", padx=10, pady=10)

        self.btn_whoami = ttk.Button(
            cmd_frame,
            text="WHOAMI",
            command=lambda: self.send_command_thread("WHOAMI")
        )

        self.btn_pos = ttk.Button(
            cmd_frame,
            text="POS",
            command=lambda: self.send_command_thread("POS")
        )

        self.btn_status = ttk.Button(
            cmd_frame,
            text="STATUS",
            command=lambda: self.send_command_thread("STATUS")
        )

        self.btn_open = ttk.Button(
            cmd_frame,
            text="OPEN",
            command=lambda: self.send_command_thread("OPEN")
        )

        self.btn_mid = ttk.Button(
            cmd_frame,
            text="MID",
            command=lambda: self.send_command_thread("MID")
        )

        self.btn_close = ttk.Button(
            cmd_frame,
            text="CLOSE",
            command=lambda: self.send_command_thread("CLOSE")
        )

        self.btn_demo = ttk.Button(
            cmd_frame,
            text="PLAY DEMO",
            command=lambda: self.send_command_thread("DEMO")
        )

        self.btn_stop = ttk.Button(
            cmd_frame,
            text="STOP",
            command=lambda: self.send_command_thread("STOP")
        )

        self.btn_whoami.grid(row=0, column=0, padx=8, pady=8, sticky="ew")
        self.btn_pos.grid(row=0, column=1, padx=8, pady=8, sticky="ew")
        self.btn_status.grid(row=0, column=2, padx=8, pady=8, sticky="ew")

        self.btn_open.grid(row=1, column=0, padx=8, pady=8, sticky="ew")
        self.btn_mid.grid(row=1, column=1, padx=8, pady=8, sticky="ew")
        self.btn_close.grid(row=1, column=2, padx=8, pady=8, sticky="ew")

        self.btn_demo.grid(row=2, column=0, columnspan=2, padx=8, pady=8, sticky="ew")
        self.btn_stop.grid(row=2, column=2, padx=8, pady=8, sticky="ew")

        for col in range(3):
            cmd_frame.columnconfigure(col, weight=1)

        # ---------- Log frame ----------
        log_frame = ttk.LabelFrame(self.root, text="UART log")
        log_frame.pack(fill="both", expand=True, padx=10, pady=10)

        self.log_text = tk.Text(log_frame, height=15, wrap="word")
        self.log_text.pack(side="left", fill="both", expand=True)

        scrollbar = ttk.Scrollbar(
            log_frame,
            orient="vertical",
            command=self.log_text.yview
        )
        scrollbar.pack(side="right", fill="y")

        self.log_text.configure(yscrollcommand=scrollbar.set)

        # ---------- Initial button state ----------
        self.set_command_buttons_state("disabled")

    def refresh_ports(self):
        ports = list(serial.tools.list_ports.comports())
        port_names = [p.device for p in ports]

        self.port_box["values"] = port_names

        if "COM3" in port_names:
            self.port_var.set("COM3")
        elif port_names:
            self.port_var.set(port_names[0])
        else:
            self.port_var.set("")

    def connect_uart(self):
        port = self.port_var.get()

        if not port:
            messagebox.showerror("Error", "No COM port selected.")
            return

        try:
            self.ser = serial.Serial(
                port=port,
                baudrate=BAUDRATE,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=0.3
            )

            time.sleep(1)

            self.status_label.config(text=f"Connected to {port}")
            self.connect_button.config(state="disabled")
            self.disconnect_button.config(state="normal")
            self.set_command_buttons_state("normal")

            self.log(f"[PC] Connected to {port} at {BAUDRATE} baud")

            # Try reading startup message if available.
            startup = self.read_available_lines()
            for line in startup:
                self.log(f"[MSP432] {line}")

        except serial.SerialException as e:
            messagebox.showerror("UART error", str(e))

    def disconnect_uart(self):
        if self.ser is not None and self.ser.is_open:
            self.ser.close()

        self.ser = None
        self.status_label.config(text="Disconnected")
        self.connect_button.config(state="normal")
        self.disconnect_button.config(state="disabled")
        self.set_command_buttons_state("disabled")
        self.log("[PC] Disconnected")

    def set_command_buttons_state(self, state):
        buttons = [
            self.btn_whoami,
            self.btn_pos,
            self.btn_status,
            self.btn_open,
            self.btn_mid,
            self.btn_close,
            self.btn_demo,
            self.btn_stop,
        ]

        for btn in buttons:
            btn.config(state=state)

    def log(self, message):
        self.log_text.insert("end", message + "\n")
        self.log_text.see("end")

    def read_available_lines(self):
        lines = []

        if self.ser is None:
            return lines

        while True:
            line = self.ser.readline().decode(errors="ignore").strip()

            if not line:
                break

            lines.append(line)

        return lines

    def send_command_thread(self, cmd):
        if self.busy:
            self.log("[PC] Busy, wait for current command to finish.")
            return

        thread = threading.Thread(
            target=self.send_command,
            args=(cmd,),
            daemon=True
        )
        thread.start()

    def send_command(self, cmd):
        if self.ser is None or not self.ser.is_open:
            self.log("[PC] UART not connected.")
            return

        self.busy = True
        self.root.after(0, lambda: self.set_command_buttons_state("disabled"))

        try:
            self.root.after(0, lambda: self.log(f"[PC] > {cmd}"))
            self.ser.write((cmd + "\n").encode())

            responses = []
            start_time = time.time()

            while time.time() - start_time < 15:
                line = self.ser.readline().decode(errors="ignore").strip()

                if line:
                    responses.append(line)
                    self.root.after(0, lambda l=line: self.log(f"[MSP432] {l}"))

                    # Final response received.
                    if (
                        line.startswith("OK")
                        or line.startswith("ERR")
                        or line.startswith("WHOAMI")
                        or line.startswith("POS")
                        or line.startswith("STATUS")
                        or line.startswith("COMMANDS")
                        or line.startswith("SIM")
                    ):
                        break

            if not responses:
                self.root.after(0, lambda: self.log("[MSP432] <no response>"))

        except serial.SerialException as e:
            self.root.after(0, lambda: self.log(f"[PC] UART error: {e}"))

        finally:
            self.busy = False
            self.root.after(0, lambda: self.set_command_buttons_state("normal"))

    def on_close(self):
        self.disconnect_uart()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = HandControlGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_close)
    root.mainloop()
