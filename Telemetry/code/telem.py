import sys
import csv
import matplotlib as plt
from shapely.geometry import Point
from shapely.geometry import box
import matplotlib.pyplot as pyplt
import os
plt.use('Qt5Agg')

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg, NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QComboBox, QVBoxLayout, QWidget, QLabel

dataDirectory = "./SampleDataUnpacked"
gpsDataLat = []
gpsDataLon = []
currentData = []
timestamps = []

trackIndex = 1
#track bounds
bounds = []
#psczolki
bounds.append([54.179763, 18.708786, 54.175482, 18.714701])
#torun
bounds.append([53.024130, 18.547323, 53.019774, 18.553385])

class CSVReader():
    dirtectoryList = []
    gpsPath = ""
    def __init__(self, path):
        self.directoryList = os.listdir(path)
        for fileName in self.directoryList:
            if fileName.find("_sensor_gps_0.csv") != -1:
                self.gpsPath = fileName
                print(self.gpsPath)
                break
    def printDirectory(self):
        print(self.directoryList)

class MplCanvas(FigureCanvasQTAgg):
    axs = []
    fig = []
    def __init__(self, parent=None, width=15, height=4, dpi=100):
        self.fig,self.axs = pyplt.subplots(1,2, figsize=(30,15))
        super(MplCanvas, self).__init__(self.fig)


class MainWindow(QMainWindow):
    sc = ""
    currentFile = ""
    def __init__(self):
        super().__init__()
        self.setFixedSize(QSize(1300, 700))
        self.setWindowTitle("Telem App")
        layout = QVBoxLayout()
        self.sc = MplCanvas(self, width=5, height=4, dpi=100)

        self.pickFileCombo = QComboBox()
        self.pickValuesCombo = QComboBox()
        
        csvReader = CSVReader(dataDirectory)

        with open(dataDirectory + "/" + csvReader.gpsPath,'r') as csvfile:
            plots = csv.DictReader(csvfile, delimiter=',')
            for row in plots:
               if float(row['lat']) > 1:
                    #convert lat lon
                    lon = float(row['lat'])/10000000
                    lat = float(row['lon'])/10000000
                    point = Point(lon, lat)
                    #error correction
                    newbox = box(bounds[trackIndex][0],bounds[trackIndex][1], bounds[trackIndex][2], bounds[trackIndex][3])
                    if newbox.contains(point):
                        gpsDataLat.append(lon)
                        gpsDataLon.append(lat)
               
               
        self.sc.axs[1].plot(gpsDataLat, gpsDataLon, label = "GPS", color="orange", zorder = 0)
        self.sc.axs[1].invert_yaxis()
        self.sc.draw()
        self.sc.show()
        
        for fileName in csvReader.directoryList:
            self.pickFileCombo.addItem(fileName)


        self.pickValuesCombo.activated[str].connect(self.selected)
        self.pickFileCombo.activated[str].connect(self.selectedFile)
        
        self.toolbar = NavigationToolbar(self.sc, self)

        self.stateLabel = QLabel("State: OK")
       
        
        layout.addWidget(self.pickFileCombo)
        layout.addWidget(self.pickValuesCombo)
        
        layout.addWidget(self.stateLabel)
        layout.addWidget(self.sc)
        layout.addWidget(self.toolbar)

        w = QWidget()
        w.setLayout(layout)
        self.setCentralWidget(w)

    def selected(self,value):
        print("Loading")
        self.stateLabel.setText("State: Loading values")
        self.stateLabel.update()
        timestamps.clear()
        currentData.clear()
        with open(dataDirectory + "/" + self.currentFile,'r') as csvfile:
            plots = csv.DictReader(csvfile, delimiter=',')
            for row in plots:
                timestamps.append(int(row['timestamp']))
                currentData.append(float(row[value]))
        self.sc.axs[0].cla()
        self.sc.axs[0].plot(timestamps,currentData, label=value,linewidth=0.5)
        self.sc.draw()
        self.sc.show()
        self.stateLabel.setText("State: OK")

    def selectedFile(self, value):
        self.stateLabel.setText("State: Loading file")
        self.pickValuesCombo.clear()
        self.currentFile = value
        with open(dataDirectory +  "/" + value,'r') as csvfile:
            plots = csv.DictReader(csvfile, delimiter=',')
            self.pickValuesCombo.addItems(plots.fieldnames)
        self.stateLabel.setText("State: OK")

app = QApplication(sys.argv)

window = MainWindow()
window.show()

app.exec()