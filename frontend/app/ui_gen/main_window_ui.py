# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'main_window.ui'
##
## Created by: Qt User Interface Compiler version 6.9.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QFrame, QHBoxLayout,
    QLabel, QMainWindow, QMenu, QMenuBar,
    QPushButton, QSizePolicy, QSpacerItem, QStatusBar,
    QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(814, 609)
        font = QFont()
        font.setBold(False)
        MainWindow.setFont(font)
        MainWindow.setWindowTitle(u"MS6_R ")
        MainWindow.setToolTipDuration(-1)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.main_container = QFrame(self.centralwidget)
        self.main_container.setObjectName(u"main_container")
        self.main_container.setGeometry(QRect(0, 0, 811, 551))
        font1 = QFont()
        font1.setPointSize(9)
        font1.setBold(False)
        self.main_container.setFont(font1)
        self.main_container.setFrameShape(QFrame.Shape.Box)
        self.main_container.setFrameShadow(QFrame.Shadow.Raised)
        self.con_frame = QFrame(self.main_container)
        self.con_frame.setObjectName(u"con_frame")
        self.con_frame.setGeometry(QRect(0, 0, 300, 150))
        self.con_frame.setMinimumSize(QSize(300, 150))
        self.con_frame.setMaximumSize(QSize(300, 150))
        font2 = QFont()
        font2.setPointSize(10)
        font2.setBold(True)
        self.con_frame.setFont(font2)
        self.con_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.con_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.verticalLayout = QVBoxLayout(self.con_frame)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, 9, -1, -1)
        self.con_h1_lable_frame = QFrame(self.con_frame)
        self.con_h1_lable_frame.setObjectName(u"con_h1_lable_frame")
        self.con_h1_lable_frame.setMaximumSize(QSize(300, 50))
        self.con_h1_lable_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.con_h1_lable_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.verticalLayout_2 = QVBoxLayout(self.con_h1_lable_frame)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(-1, 0, -1, 0)
        self.select_device_label = QLabel(self.con_h1_lable_frame)
        self.select_device_label.setObjectName(u"select_device_label")
        self.select_device_label.setFrameShape(QFrame.Shape.NoFrame)
        self.select_device_label.setFrameShadow(QFrame.Shadow.Plain)

        self.verticalLayout_2.addWidget(self.select_device_label)


        self.verticalLayout.addWidget(self.con_h1_lable_frame)

        self.con_select_device_frame = QFrame(self.con_frame)
        self.con_select_device_frame.setObjectName(u"con_select_device_frame")
        self.con_select_device_frame.setMaximumSize(QSize(300, 50))
        self.con_select_device_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.con_select_device_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout = QHBoxLayout(self.con_select_device_frame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(9, 0, -1, 0)
        self.con_device_comboBox = QComboBox(self.con_select_device_frame)
        self.con_device_comboBox.setObjectName(u"con_device_comboBox")
        self.con_device_comboBox.setMinimumSize(QSize(170, 0))

        self.horizontalLayout.addWidget(self.con_device_comboBox)

        self.con_device_horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.con_device_horizontalSpacer)

        self.con_connect_btn = QPushButton(self.con_select_device_frame)
        self.con_connect_btn.setObjectName(u"con_connect_btn")

        self.horizontalLayout.addWidget(self.con_connect_btn)


        self.verticalLayout.addWidget(self.con_select_device_frame)

        self.con_status_frame = QFrame(self.con_frame)
        self.con_status_frame.setObjectName(u"con_status_frame")
        self.con_status_frame.setMaximumSize(QSize(300, 40))
        self.con_status_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.con_status_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_2 = QHBoxLayout(self.con_status_frame)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(-1, 0, -1, 0)
        self.con_status_label1 = QLabel(self.con_status_frame)
        self.con_status_label1.setObjectName(u"con_status_label1")

        self.horizontalLayout_2.addWidget(self.con_status_label1)

        self.con_status_spacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.con_status_spacer)

        self.con_status_label2 = QLabel(self.con_status_frame)
        self.con_status_label2.setObjectName(u"con_status_label2")

        self.horizontalLayout_2.addWidget(self.con_status_label2)


        self.verticalLayout.addWidget(self.con_status_frame)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 814, 33))
        self.menuControls = QMenu(self.menubar)
        self.menuControls.setObjectName(u"menuControls")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuControls.menuAction())

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        self.select_device_label.setText(QCoreApplication.translate("MainWindow", u"Select your Device:", None))
        self.con_connect_btn.setText(QCoreApplication.translate("MainWindow", u"Connect", None))
        self.con_status_label1.setText(QCoreApplication.translate("MainWindow", u"Status:", None))
        self.con_status_label2.setText(QCoreApplication.translate("MainWindow", u"Disconnected", None))
        self.menuControls.setTitle(QCoreApplication.translate("MainWindow", u"Controls", None))
        pass
    # retranslateUi

