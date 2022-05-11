from typing import Tuple
from matplotlib.animation import FuncAnimation
import serial
import matplotlib.pyplot as plt
from time import time
import numpy as np

PORT = "COM4"
BAUD_RATE = 115200
WINDOW_SECONDS = 20


class PlotterState:
    """Holds the state of the plotter"""
    curr_ylim_max = float("-inf")
    curr_ylim_min = float("inf")

    # Dictionary containing keys of value type, values of a (data, line) tuple
    data_line_dict = {}
    
    def __init__(self, ser: serial.Serial) -> None:
        self.ser = ser
        self.fig, self.ax = plt.subplots()
        self.start_time = time()


def update_plot(_, *args):
    state: PlotterState = args[0]
    
    recv_time = time()
    
    line = state.ser.readline()
    line_str = line.decode()

    try:
        line_key, line_val = line_str.split(": ")
    except ValueError:
        pass
    else:
        val: Tuple[np.ndarray, plt.Line2D] | None = state.data_line_dict.get(line_key)
        elapsed_time = recv_time - state.start_time
        line_float = float(line_val)
        new_row = np.array([[elapsed_time, line_float]])

        state.curr_ylim_max = max(state.curr_ylim_max, line_float)
        state.curr_ylim_min = min(state.curr_ylim_min, line_float)
        state.ax.set_ylim(state.curr_ylim_min, state.curr_ylim_max)

        if val:
            data = np.append(val[0], new_row, axis=0)
            val[1].set_data(data[:, 0], data[:, 1])
            line = val[1]
        else:
            data = new_row
            line, = state.ax.plot(data[:, 0], data[:, 1], label=line_key)
        

        state.data_line_dict[line_key] = (data, line)
        state.ax.legend(loc="upper left")
        state.ax.set_xlim(max(0, elapsed_time - WINDOW_SECONDS), elapsed_time)

def main():

    ser = serial.Serial(port=PORT, baudrate=BAUD_RATE)

    state = PlotterState(ser)
    

    ani = FuncAnimation(state.fig, update_plot, fargs=(state,), interval=1)
    
    
    plt.show()

if __name__ == "__main__":
    main()
