import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout
from PyQt6.QtCore import QTimer
import pyqtgraph as pg
import serial

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # Configuración de la ventana principal
        self.setWindowTitle('Gráficos en Tiempo Real desde Arduino')
        self.setGeometry(100, 100, 800, 600)

        # Crear un widget central y un layout vertical
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout()
        central_widget.setLayout(layout)

        # Crear los widgets de gráfico
        self.graphWidget1 = pg.PlotWidget(title='Señal RB')
        self.graphWidget2 = pg.PlotWidget(title='Señal RT')
        self.graphWidget3 = pg.PlotWidget(title='Señal LB')
        self.graphWidget4 = pg.PlotWidget(title='Señal LT')

        # Agregar los widgets de gráfico al layout
        layout.addWidget(self.graphWidget1)
        layout.addWidget(self.graphWidget2)
        layout.addWidget(self.graphWidget3)
        layout.addWidget(self.graphWidget4)

        # Inicializar las listas de datos
        self.x = []  # Eje X (número de muestra)
        self.y1 = []  # Datos del RB
        self.y2 = []  # Datos del RT
        self.y3 = []  # Datos del LB
        self.y4 = []  # Datos del LT
        self.ptr = 0  # Contador de muestras

        # Configurar las curvas de datos para cada gráfico
        self.data_line1 = self.graphWidget1.plot(self.x, self.y1, pen='r')
        self.data_line2 = self.graphWidget2.plot(self.x, self.y2, pen='g')
        self.data_line3 = self.graphWidget3.plot(self.x, self.y3, pen='b')
        self.data_line4 = self.graphWidget4.plot(self.x, self.y4, pen='y')

        # Configurar el temporizador para actualizar los datos
        self.timer = QTimer()
        self.timer.setInterval(10)  # Intervalo en milisegundos
        self.timer.timeout.connect(self.update_plot_data)
        self.timer.start()

        # Configurar la comunicación serial
        try:
            self.ser = serial.Serial('COM10', 115200)  # Reemplaza 'COM3' por el puerto correcto
        except serial.SerialException:
            print("Error: No se pudo abrir el puerto serial.")
            sys.exit(1)

    def update_plot_data(self):
        if self.ser.in_waiting:
            try:
                line = self.ser.readline().decode('utf-8').rstrip()
                data = line.split(';')
                if len(data) == 4:
                    pot1 = int(data[0])
                    pot2 = int(data[1])
                    pot3 = int(data[2])
                    pot4 = int(data[3])

                    self.x.append(self.ptr)
                    self.y1.append(pot1)
                    self.y2.append(pot2)
                    self.y3.append(pot3)
                    self.y4.append(pot4)
                    self.ptr += 1

                    # Actualizar los datos de las curvas
                    self.data_line1.setData(self.x, self.y1)
                    self.data_line2.setData(self.x, self.y2)
                    self.data_line3.setData(self.x, self.y3)
                    self.data_line4.setData(self.x, self.y4)

                    # Limitar el número de puntos para mejorar el rendimiento
                    if len(self.x) > 200:
                        self.x = self.x[-200:]
                        self.y1 = self.y1[-200:]
                        self.y2 = self.y2[-200:]
                        self.y3 = self.y3[-200:]
                        self.y4 = self.y4[-200:]
                else:
                    print("Datos incompletos recibidos:", data)
            except Exception as e:
                print("Error al leer datos:", e)

def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()
