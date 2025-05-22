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
from PySide6.QtWidgets import (QAbstractScrollArea, QApplication, QComboBox, QFrame,
    QGridLayout, QHBoxLayout, QLabel, QLayout,
    QMainWindow, QMenu, QMenuBar, QPushButton,
    QSizePolicy, QSpacerItem, QStatusBar, QTextEdit,
    QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1272, 796)
        font = QFont()
        font.setBold(False)
        MainWindow.setFont(font)
        MainWindow.setWindowTitle(u"MS6_R ")
        MainWindow.setToolTipDuration(-1)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.main_container = QFrame(self.centralwidget)
        self.main_container.setObjectName(u"main_container")
        self.main_container.setGeometry(QRect(0, 0, 1221, 741))
        font1 = QFont()
        font1.setPointSize(9)
        font1.setBold(False)
        self.main_container.setFont(font1)
        self.main_container.setFrameShape(QFrame.Shape.Box)
        self.main_container.setFrameShadow(QFrame.Shadow.Raised)
        self.gridLayout = QGridLayout(self.main_container)
        self.gridLayout.setObjectName(u"gridLayout")
        self.prog_frame = QFrame(self.main_container)
        self.prog_frame.setObjectName(u"prog_frame")
        self.prog_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.prog_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.prog_frame)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.verticalLayout_3.setContentsMargins(-1, 9, -1, -1)
        self.prog_header_frame = QFrame(self.prog_frame)
        self.prog_header_frame.setObjectName(u"prog_header_frame")
        self.prog_header_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.prog_header_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_4 = QHBoxLayout(self.prog_header_frame)
        self.horizontalLayout_4.setSpacing(0)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.horizontalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.prog_label = QLabel(self.prog_header_frame)
        self.prog_label.setObjectName(u"prog_label")
        font2 = QFont()
        font2.setPointSize(10)
        font2.setBold(True)
        font2.setItalic(False)
        font2.setUnderline(False)
        self.prog_label.setFont(font2)

        self.horizontalLayout_4.addWidget(self.prog_label)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer)

        self.prog_clear_btn = QPushButton(self.prog_header_frame)
        self.prog_clear_btn.setObjectName(u"prog_clear_btn")
        self.prog_clear_btn.setMaximumSize(QSize(16777211, 16777215))
        font3 = QFont()
        font3.setPointSize(9)
        font3.setBold(True)
        self.prog_clear_btn.setFont(font3)

        self.horizontalLayout_4.addWidget(self.prog_clear_btn)


        self.verticalLayout_3.addWidget(self.prog_header_frame)

        self.prog_textEdit = QTextEdit(self.prog_frame)
        self.prog_textEdit.setObjectName(u"prog_textEdit")
        font4 = QFont()
        font4.setFamilies([u"Arial"])
        font4.setPointSize(10)
        font4.setBold(False)
        font4.setKerning(False)
        self.prog_textEdit.setFont(font4)
        self.prog_textEdit.setStyleSheet(u"background-color: rgb(21, 21, 21);\n"
"color: #fff;\n"
"font-size: 10pt;\n"
"")
        self.prog_textEdit.setFrameShape(QFrame.Shape.NoFrame)
        self.prog_textEdit.setFrameShadow(QFrame.Shadow.Plain)
        self.prog_textEdit.setSizeAdjustPolicy(QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents)
        self.prog_textEdit.setReadOnly(True)

        self.verticalLayout_3.addWidget(self.prog_textEdit)


        self.gridLayout.addWidget(self.prog_frame, 0, 4, 9, 4)

        self.con_frame = QFrame(self.main_container)
        self.con_frame.setObjectName(u"con_frame")
        self.con_frame.setMinimumSize(QSize(300, 95))
        self.con_frame.setMaximumSize(QSize(300, 95))
        font5 = QFont()
        font5.setPointSize(10)
        font5.setBold(True)
        self.con_frame.setFont(font5)
        self.con_frame.setFrameShape(QFrame.Shape.Box)
        self.con_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.verticalLayout = QVBoxLayout(self.con_frame)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(-1, 0, -1, 0)
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

        self.verticalSpacer = QSpacerItem(17, 10, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)


        self.gridLayout.addWidget(self.con_frame, 0, 0, 1, 1)

        self.btn_frame = QFrame(self.main_container)
        self.btn_frame.setObjectName(u"btn_frame")
        self.btn_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.btn_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_3 = QHBoxLayout(self.btn_frame)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.btn_pushButton = QPushButton(self.btn_frame)
        self.btn_pushButton.setObjectName(u"btn_pushButton")
        self.btn_pushButton.setFont(font3)

        self.horizontalLayout_3.addWidget(self.btn_pushButton)

        self.btn_pushButton_2 = QPushButton(self.btn_frame)
        self.btn_pushButton_2.setObjectName(u"btn_pushButton_2")
        self.btn_pushButton_2.setFont(font3)

        self.horizontalLayout_3.addWidget(self.btn_pushButton_2)

        self.btn_load_prog_btn = QPushButton(self.btn_frame)
        self.btn_load_prog_btn.setObjectName(u"btn_load_prog_btn")
        self.btn_load_prog_btn.setEnabled(False)
        self.btn_load_prog_btn.setFont(font3)

        self.horizontalLayout_3.addWidget(self.btn_load_prog_btn)

        self.btn_horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.btn_horizontalSpacer)


        self.gridLayout.addWidget(self.btn_frame, 9, 4, 1, 4)

        self.log_frame = QFrame(self.main_container)
        self.log_frame.setObjectName(u"log_frame")
        self.log_frame.setMinimumSize(QSize(0, 0))
        self.log_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.log_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_4 = QVBoxLayout(self.log_frame)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.log_label = QLabel(self.log_frame)
        self.log_label.setObjectName(u"log_label")
        self.log_label.setFont(font5)

        self.verticalLayout_4.addWidget(self.log_label)

        self.log_textEdit = QTextEdit(self.log_frame)
        self.log_textEdit.setObjectName(u"log_textEdit")
        font6 = QFont()
        font6.setPointSize(10)
        font6.setBold(False)
        self.log_textEdit.setFont(font6)
        self.log_textEdit.setStyleSheet(u"background-color: rgb(21, 21, 21);\n"
"color: #fff;\n"
"font-size: 10pt;\n"
"")
        self.log_textEdit.setFrameShape(QFrame.Shape.NoFrame)
        self.log_textEdit.setFrameShadow(QFrame.Shadow.Plain)
        self.log_textEdit.setReadOnly(True)

        self.verticalLayout_4.addWidget(self.log_textEdit)


        self.gridLayout.addWidget(self.log_frame, 10, 4, 4, 4)

        self.frame_6 = QFrame(self.main_container)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Shadow.Raised)

        self.gridLayout.addWidget(self.frame_6, 10, 0, 4, 3)

        self.frame_8 = QFrame(self.main_container)
        self.frame_8.setObjectName(u"frame_8")
        self.frame_8.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_8.setFrameShadow(QFrame.Shadow.Raised)

        self.gridLayout.addWidget(self.frame_8, 1, 0, 9, 3)

        self.frame_2 = QFrame(self.main_container)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)

        self.gridLayout.addWidget(self.frame_2, 0, 1, 1, 2)

        self.cmd_frame = QFrame(self.main_container)
        self.cmd_frame.setObjectName(u"cmd_frame")
        self.cmd_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.cmd_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_5 = QVBoxLayout(self.cmd_frame)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.cmd_prog_menu_pushButton = QPushButton(self.cmd_frame)
        self.cmd_prog_menu_pushButton.setObjectName(u"cmd_prog_menu_pushButton")
        self.cmd_prog_menu_pushButton.setCheckable(True)
        self.cmd_prog_menu_pushButton.setChecked(True)

        self.verticalLayout_5.addWidget(self.cmd_prog_menu_pushButton)

        self.cmd_prog_frame = QFrame(self.cmd_frame)
        self.cmd_prog_frame.setObjectName(u"cmd_prog_frame")
        self.cmd_prog_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.cmd_prog_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.verticalLayout_6 = QVBoxLayout(self.cmd_prog_frame)
        self.verticalLayout_6.setSpacing(0)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.verticalLayout_6.setSizeConstraint(QLayout.SizeConstraint.SetDefaultConstraint)
        self.verticalLayout_6.setContentsMargins(0, 0, 0, 0)
        self.cmd_ping_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_ping_btn.setObjectName(u"cmd_ping_btn")

        self.verticalLayout_6.addWidget(self.cmd_ping_btn)

        self.cmd_pong_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_pong_btn.setObjectName(u"cmd_pong_btn")
        font7 = QFont()
        font7.setPointSize(9)
        font7.setBold(False)
        font7.setStrikeOut(False)
        font7.setKerning(True)
        font7.setStyleStrategy(QFont.PreferDefault)
        self.cmd_pong_btn.setFont(font7)
        self.cmd_pong_btn.setAutoFillBackground(False)

        self.verticalLayout_6.addWidget(self.cmd_pong_btn)

        self.cmd_switches_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_switches_btn.setObjectName(u"cmd_switches_btn")
        self.cmd_switches_btn.setFont(font7)
        self.cmd_switches_btn.setAutoFillBackground(False)

        self.verticalLayout_6.addWidget(self.cmd_switches_btn)


        self.verticalLayout_5.addWidget(self.cmd_prog_frame)

        self.cmd_condi_menu_pushButton = QPushButton(self.cmd_frame)
        self.cmd_condi_menu_pushButton.setObjectName(u"cmd_condi_menu_pushButton")
        self.cmd_condi_menu_pushButton.setCheckable(True)
        self.cmd_condi_menu_pushButton.setChecked(True)

        self.verticalLayout_5.addWidget(self.cmd_condi_menu_pushButton)

        self.cmd_condi_frame = QFrame(self.cmd_frame)
        self.cmd_condi_frame.setObjectName(u"cmd_condi_frame")
        self.cmd_condi_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.cmd_condi_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.verticalLayout_7 = QVBoxLayout(self.cmd_condi_frame)
        self.verticalLayout_7.setSpacing(0)
        self.verticalLayout_7.setObjectName(u"verticalLayout_7")
        self.verticalLayout_7.setContentsMargins(0, 0, 0, 0)
        self.cmd_begin_pushButton = QPushButton(self.cmd_condi_frame)
        self.cmd_begin_pushButton.setObjectName(u"cmd_begin_pushButton")

        self.verticalLayout_7.addWidget(self.cmd_begin_pushButton)

        self.cmd_end_pushButton = QPushButton(self.cmd_condi_frame)
        self.cmd_end_pushButton.setObjectName(u"cmd_end_pushButton")

        self.verticalLayout_7.addWidget(self.cmd_end_pushButton)


        self.verticalLayout_5.addWidget(self.cmd_condi_frame)

        self.cmd_verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_5.addItem(self.cmd_verticalSpacer)


        self.gridLayout.addWidget(self.cmd_frame, 0, 8, 14, 2)

        self.frame_8.raise_()
        self.frame_6.raise_()
        self.con_frame.raise_()
        self.frame_2.raise_()
        self.prog_frame.raise_()
        self.log_frame.raise_()
        self.cmd_frame.raise_()
        self.btn_frame.raise_()
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1272, 33))
        self.menuControls = QMenu(self.menubar)
        self.menuControls.setObjectName(u"menuControls")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuControls.menuAction())

        self.retranslateUi(MainWindow)
        self.cmd_prog_menu_pushButton.toggled.connect(self.cmd_prog_frame.setVisible)
        self.cmd_condi_menu_pushButton.toggled.connect(self.cmd_condi_frame.setVisible)
        self.prog_clear_btn.clicked.connect(self.prog_textEdit.clear)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        self.prog_label.setText(QCoreApplication.translate("MainWindow", u"Program:", None))
        self.prog_clear_btn.setText(QCoreApplication.translate("MainWindow", u"Clear", None))
        self.prog_textEdit.setPlaceholderText("")
        self.select_device_label.setText(QCoreApplication.translate("MainWindow", u"Select your Device:", None))
        self.con_connect_btn.setText(QCoreApplication.translate("MainWindow", u"Connect", None))
        self.con_status_label1.setText(QCoreApplication.translate("MainWindow", u"Status:", None))
        self.con_status_label2.setText(QCoreApplication.translate("MainWindow", u"Disconnected", None))
        self.btn_pushButton.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.btn_pushButton_2.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.btn_load_prog_btn.setText(QCoreApplication.translate("MainWindow", u"Load Program", None))
        self.log_label.setText(QCoreApplication.translate("MainWindow", u"Response log:", None))
        self.cmd_prog_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Programs", None))
        self.cmd_ping_btn.setText(QCoreApplication.translate("MainWindow", u"Ping", None))
        self.cmd_pong_btn.setText(QCoreApplication.translate("MainWindow", u"Pong", None))
        self.cmd_switches_btn.setText(QCoreApplication.translate("MainWindow", u"Test_Switches", None))
        self.cmd_condi_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Conditional", None))
        self.cmd_begin_pushButton.setText(QCoreApplication.translate("MainWindow", u"Begin", None))
        self.cmd_end_pushButton.setText(QCoreApplication.translate("MainWindow", u"End", None))
        self.menuControls.setTitle(QCoreApplication.translate("MainWindow", u"Controls", None))
        pass
    # retranslateUi

