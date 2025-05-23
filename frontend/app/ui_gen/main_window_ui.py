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
        MainWindow.resize(1268, 800)
        MainWindow.setMinimumSize(QSize(1268, 800))
        font = QFont()
        font.setBold(False)
        MainWindow.setFont(font)
        MainWindow.setWindowTitle(u"MS6_R ")
        MainWindow.setToolTipDuration(-1)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.verticalLayout_10 = QVBoxLayout(self.centralwidget)
        self.verticalLayout_10.setSpacing(0)
        self.verticalLayout_10.setObjectName(u"verticalLayout_10")
        self.verticalLayout_10.setContentsMargins(0, 0, 0, 0)
        self.main_container = QFrame(self.centralwidget)
        self.main_container.setObjectName(u"main_container")
        self.main_container.setMinimumSize(QSize(0, 0))
        self.main_container.setMaximumSize(QSize(10000, 100000))
        font1 = QFont()
        font1.setPointSize(9)
        font1.setBold(False)
        self.main_container.setFont(font1)
        self.main_container.setFrameShape(QFrame.Shape.NoFrame)
        self.main_container.setFrameShadow(QFrame.Shadow.Plain)
        self.gridLayout = QGridLayout(self.main_container)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(6, 6, 6, 6)
        self.jog_frame = QFrame(self.main_container)
        self.jog_frame.setObjectName(u"jog_frame")
        self.jog_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.jog_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout = QVBoxLayout(self.jog_frame)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.jog_j1_frame = QFrame(self.jog_frame)
        self.jog_j1_frame.setObjectName(u"jog_j1_frame")
        self.jog_j1_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j1_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_2 = QHBoxLayout(self.jog_j1_frame)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalSpacer_28 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_28)

        self.label = QLabel(self.jog_j1_frame)
        self.label.setObjectName(u"label")
        font2 = QFont()
        font2.setPointSize(20)
        font2.setBold(True)
        self.label.setFont(font2)

        self.horizontalLayout_2.addWidget(self.label)

        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_3)

        self.jog_j1_neg_btn = QPushButton(self.jog_j1_frame)
        self.jog_j1_neg_btn.setObjectName(u"jog_j1_neg_btn")
        self.jog_j1_neg_btn.setMaximumSize(QSize(50, 40))
        font3 = QFont()
        font3.setPointSize(12)
        font3.setBold(True)
        self.jog_j1_neg_btn.setFont(font3)

        self.horizontalLayout_2.addWidget(self.jog_j1_neg_btn)

        self.horizontalSpacer_6 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_6)

        self.jog_j1_label = QLabel(self.jog_j1_frame)
        self.jog_j1_label.setObjectName(u"jog_j1_label")
        self.jog_j1_label.setMinimumSize(QSize(100, 0))
        self.jog_j1_label.setMaximumSize(QSize(16777215, 30))
        font4 = QFont()
        font4.setPointSize(10)
        font4.setBold(False)
        self.jog_j1_label.setFont(font4)
        self.jog_j1_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j1_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_2.addWidget(self.jog_j1_label)

        self.horizontalSpacer_7 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_7)

        self.jog_j1_pos_btn = QPushButton(self.jog_j1_frame)
        self.jog_j1_pos_btn.setObjectName(u"jog_j1_pos_btn")
        self.jog_j1_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j1_pos_btn.setFont(font3)

        self.horizontalLayout_2.addWidget(self.jog_j1_pos_btn)

        self.horizontalSpacer_5 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_5)


        self.verticalLayout.addWidget(self.jog_j1_frame)

        self.jog_j2_frame = QFrame(self.jog_frame)
        self.jog_j2_frame.setObjectName(u"jog_j2_frame")
        self.jog_j2_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j2_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_6 = QHBoxLayout(self.jog_j2_frame)
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.horizontalSpacer_29 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_29)

        self.label_4 = QLabel(self.jog_j2_frame)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setFont(font2)

        self.horizontalLayout_6.addWidget(self.label_4)

        self.horizontalSpacer_8 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_8)

        self.jog_j2_neg_btn = QPushButton(self.jog_j2_frame)
        self.jog_j2_neg_btn.setObjectName(u"jog_j2_neg_btn")
        self.jog_j2_neg_btn.setMaximumSize(QSize(50, 40))
        self.jog_j2_neg_btn.setFont(font3)

        self.horizontalLayout_6.addWidget(self.jog_j2_neg_btn)

        self.horizontalSpacer_9 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_9)

        self.jog_j2_label = QLabel(self.jog_j2_frame)
        self.jog_j2_label.setObjectName(u"jog_j2_label")
        self.jog_j2_label.setMinimumSize(QSize(100, 0))
        self.jog_j2_label.setMaximumSize(QSize(16777215, 30))
        self.jog_j2_label.setFont(font4)
        self.jog_j2_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j2_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_6.addWidget(self.jog_j2_label)

        self.horizontalSpacer_10 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_10)

        self.jog_j2_pos_btn = QPushButton(self.jog_j2_frame)
        self.jog_j2_pos_btn.setObjectName(u"jog_j2_pos_btn")
        self.jog_j2_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j2_pos_btn.setFont(font3)

        self.horizontalLayout_6.addWidget(self.jog_j2_pos_btn)

        self.horizontalSpacer_11 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_11)


        self.verticalLayout.addWidget(self.jog_j2_frame)

        self.jog_j3_frame = QFrame(self.jog_frame)
        self.jog_j3_frame.setObjectName(u"jog_j3_frame")
        self.jog_j3_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j3_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_7 = QHBoxLayout(self.jog_j3_frame)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.horizontalSpacer_30 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_30)

        self.label_6 = QLabel(self.jog_j3_frame)
        self.label_6.setObjectName(u"label_6")
        self.label_6.setFont(font2)

        self.horizontalLayout_7.addWidget(self.label_6)

        self.horizontalSpacer_12 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_12)

        self.jog_j3_neg_btn = QPushButton(self.jog_j3_frame)
        self.jog_j3_neg_btn.setObjectName(u"jog_j3_neg_btn")
        self.jog_j3_neg_btn.setMaximumSize(QSize(50, 40))
        self.jog_j3_neg_btn.setFont(font3)

        self.horizontalLayout_7.addWidget(self.jog_j3_neg_btn)

        self.horizontalSpacer_13 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_13)

        self.jog_j3_label = QLabel(self.jog_j3_frame)
        self.jog_j3_label.setObjectName(u"jog_j3_label")
        self.jog_j3_label.setMinimumSize(QSize(100, 0))
        self.jog_j3_label.setMaximumSize(QSize(16777215, 30))
        self.jog_j3_label.setFont(font4)
        self.jog_j3_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j3_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_7.addWidget(self.jog_j3_label)

        self.horizontalSpacer_14 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_14)

        self.jog_j3_pos_btn = QPushButton(self.jog_j3_frame)
        self.jog_j3_pos_btn.setObjectName(u"jog_j3_pos_btn")
        self.jog_j3_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j3_pos_btn.setFont(font3)

        self.horizontalLayout_7.addWidget(self.jog_j3_pos_btn)

        self.horizontalSpacer_15 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_15)


        self.verticalLayout.addWidget(self.jog_j3_frame)

        self.jog_j4_frame = QFrame(self.jog_frame)
        self.jog_j4_frame.setObjectName(u"jog_j4_frame")
        self.jog_j4_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j4_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_8 = QHBoxLayout(self.jog_j4_frame)
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.horizontalSpacer_31 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_31)

        self.label_8 = QLabel(self.jog_j4_frame)
        self.label_8.setObjectName(u"label_8")
        self.label_8.setFont(font2)

        self.horizontalLayout_8.addWidget(self.label_8)

        self.horizontalSpacer_16 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_16)

        self.jog_j4_neg_btn = QPushButton(self.jog_j4_frame)
        self.jog_j4_neg_btn.setObjectName(u"jog_j4_neg_btn")
        self.jog_j4_neg_btn.setMaximumSize(QSize(50, 40))
        self.jog_j4_neg_btn.setFont(font3)

        self.horizontalLayout_8.addWidget(self.jog_j4_neg_btn)

        self.horizontalSpacer_17 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_17)

        self.jog_j4_label = QLabel(self.jog_j4_frame)
        self.jog_j4_label.setObjectName(u"jog_j4_label")
        self.jog_j4_label.setMinimumSize(QSize(100, 0))
        self.jog_j4_label.setMaximumSize(QSize(16777215, 30))
        self.jog_j4_label.setFont(font4)
        self.jog_j4_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j4_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_8.addWidget(self.jog_j4_label)

        self.horizontalSpacer_18 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_18)

        self.jog_j4_pos_btn = QPushButton(self.jog_j4_frame)
        self.jog_j4_pos_btn.setObjectName(u"jog_j4_pos_btn")
        self.jog_j4_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j4_pos_btn.setFont(font3)

        self.horizontalLayout_8.addWidget(self.jog_j4_pos_btn)

        self.horizontalSpacer_19 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_19)


        self.verticalLayout.addWidget(self.jog_j4_frame)

        self.jog_j5_frame = QFrame(self.jog_frame)
        self.jog_j5_frame.setObjectName(u"jog_j5_frame")
        self.jog_j5_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j5_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_9 = QHBoxLayout(self.jog_j5_frame)
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.horizontalSpacer_32 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_32)

        self.label_10 = QLabel(self.jog_j5_frame)
        self.label_10.setObjectName(u"label_10")
        self.label_10.setFont(font2)

        self.horizontalLayout_9.addWidget(self.label_10)

        self.horizontalSpacer_20 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_20)

        self.jog_j5_neg_btn = QPushButton(self.jog_j5_frame)
        self.jog_j5_neg_btn.setObjectName(u"jog_j5_neg_btn")
        self.jog_j5_neg_btn.setMaximumSize(QSize(50, 40))
        self.jog_j5_neg_btn.setFont(font3)

        self.horizontalLayout_9.addWidget(self.jog_j5_neg_btn)

        self.horizontalSpacer_21 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_21)

        self.jog_j5_label = QLabel(self.jog_j5_frame)
        self.jog_j5_label.setObjectName(u"jog_j5_label")
        self.jog_j5_label.setMinimumSize(QSize(100, 0))
        self.jog_j5_label.setMaximumSize(QSize(16777215, 30))
        self.jog_j5_label.setFont(font4)
        self.jog_j5_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j5_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_9.addWidget(self.jog_j5_label)

        self.horizontalSpacer_22 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_22)

        self.jog_j5_pos_btn = QPushButton(self.jog_j5_frame)
        self.jog_j5_pos_btn.setObjectName(u"jog_j5_pos_btn")
        self.jog_j5_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j5_pos_btn.setFont(font3)

        self.horizontalLayout_9.addWidget(self.jog_j5_pos_btn)

        self.horizontalSpacer_23 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_23)


        self.verticalLayout.addWidget(self.jog_j5_frame)

        self.jog_j6_frame = QFrame(self.jog_frame)
        self.jog_j6_frame.setObjectName(u"jog_j6_frame")
        self.jog_j6_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_j6_frame.setFrameShadow(QFrame.Shadow.Plain)
        self.horizontalLayout_10 = QHBoxLayout(self.jog_j6_frame)
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.horizontalSpacer_33 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_33)

        self.label_12 = QLabel(self.jog_j6_frame)
        self.label_12.setObjectName(u"label_12")
        self.label_12.setFont(font2)

        self.horizontalLayout_10.addWidget(self.label_12)

        self.horizontalSpacer_24 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_24)

        self.jog_j6_neg_btn = QPushButton(self.jog_j6_frame)
        self.jog_j6_neg_btn.setObjectName(u"jog_j6_neg_btn")
        self.jog_j6_neg_btn.setMaximumSize(QSize(50, 40))
        self.jog_j6_neg_btn.setFont(font3)

        self.horizontalLayout_10.addWidget(self.jog_j6_neg_btn)

        self.horizontalSpacer_25 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_25)

        self.jog_j6_label = QLabel(self.jog_j6_frame)
        self.jog_j6_label.setObjectName(u"jog_j6_label")
        self.jog_j6_label.setMinimumSize(QSize(100, 0))
        self.jog_j6_label.setMaximumSize(QSize(16777215, 30))
        self.jog_j6_label.setFont(font4)
        self.jog_j6_label.setFrameShape(QFrame.Shape.WinPanel)
        self.jog_j6_label.setFrameShadow(QFrame.Shadow.Sunken)

        self.horizontalLayout_10.addWidget(self.jog_j6_label)

        self.horizontalSpacer_26 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_26)

        self.jog_j6_pos_btn = QPushButton(self.jog_j6_frame)
        self.jog_j6_pos_btn.setObjectName(u"jog_j6_pos_btn")
        self.jog_j6_pos_btn.setMaximumSize(QSize(50, 40))
        self.jog_j6_pos_btn.setFont(font3)

        self.horizontalLayout_10.addWidget(self.jog_j6_pos_btn)

        self.horizontalSpacer_27 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_27)


        self.verticalLayout.addWidget(self.jog_j6_frame)


        self.gridLayout.addWidget(self.jog_frame, 1, 0, 10, 4)

        self.prog_frame = QFrame(self.main_container)
        self.prog_frame.setObjectName(u"prog_frame")
        self.prog_frame.setMaximumSize(QSize(100000, 1000))
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
        font5 = QFont()
        font5.setPointSize(10)
        font5.setBold(True)
        font5.setItalic(False)
        font5.setUnderline(False)
        self.prog_label.setFont(font5)

        self.horizontalLayout_4.addWidget(self.prog_label)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer)

        self.prog_clear_btn = QPushButton(self.prog_header_frame)
        self.prog_clear_btn.setObjectName(u"prog_clear_btn")
        self.prog_clear_btn.setMaximumSize(QSize(16777211, 16777215))
        font6 = QFont()
        font6.setPointSize(9)
        font6.setBold(True)
        self.prog_clear_btn.setFont(font6)

        self.horizontalLayout_4.addWidget(self.prog_clear_btn)


        self.verticalLayout_3.addWidget(self.prog_header_frame)

        self.prog_textEdit = QTextEdit(self.prog_frame)
        self.prog_textEdit.setObjectName(u"prog_textEdit")
        font7 = QFont()
        font7.setFamilies([u"Arial"])
        font7.setPointSize(10)
        font7.setBold(False)
        font7.setKerning(False)
        self.prog_textEdit.setFont(font7)
        self.prog_textEdit.setStyleSheet(u"background-color: rgb(21, 21, 21);\n"
"color: #fff;\n"
"font-size: 10pt;\n"
"")
        self.prog_textEdit.setFrameShape(QFrame.Shape.NoFrame)
        self.prog_textEdit.setFrameShadow(QFrame.Shadow.Plain)
        self.prog_textEdit.setSizeAdjustPolicy(QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents)
        self.prog_textEdit.setReadOnly(True)

        self.verticalLayout_3.addWidget(self.prog_textEdit)


        self.gridLayout.addWidget(self.prog_frame, 0, 4, 10, 5)

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
        font8 = QFont()
        font8.setPointSize(9)
        font8.setBold(False)
        font8.setStrikeOut(False)
        font8.setKerning(True)
        font8.setStyleStrategy(QFont.PreferDefault)
        self.cmd_pong_btn.setFont(font8)
        self.cmd_pong_btn.setAutoFillBackground(False)

        self.verticalLayout_6.addWidget(self.cmd_pong_btn)

        self.cmd_switches_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_switches_btn.setObjectName(u"cmd_switches_btn")
        self.cmd_switches_btn.setFont(font8)
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


        self.gridLayout.addWidget(self.cmd_frame, 0, 9, 14, 2)

        self.frame_6 = QFrame(self.main_container)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_6.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_35 = QHBoxLayout(self.frame_6)
        self.horizontalLayout_35.setObjectName(u"horizontalLayout_35")
        self.tool_pos_frame = QFrame(self.frame_6)
        self.tool_pos_frame.setObjectName(u"tool_pos_frame")
        self.tool_pos_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_pos_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.tool_pos_frame)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(-1, 0, -1, 9)
        self.tool_h1_label = QLabel(self.tool_pos_frame)
        self.tool_h1_label.setObjectName(u"tool_h1_label")

        self.verticalLayout_2.addWidget(self.tool_h1_label)

        self.tool_x_frame = QFrame(self.tool_pos_frame)
        self.tool_x_frame.setObjectName(u"tool_x_frame")
        self.tool_x_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_x_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_x_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_11 = QHBoxLayout(self.tool_x_frame)
        self.horizontalLayout_11.setObjectName(u"horizontalLayout_11")
        self.horizontalLayout_11.setContentsMargins(-1, 9, -1, -1)
        self.label_3 = QLabel(self.tool_x_frame)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_11.addWidget(self.label_3)

        self.tool_x_label = QLabel(self.tool_x_frame)
        self.tool_x_label.setObjectName(u"tool_x_label")
        self.tool_x_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_x_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_11.addWidget(self.tool_x_label)


        self.verticalLayout_2.addWidget(self.tool_x_frame)

        self.tool_y_frame = QFrame(self.tool_pos_frame)
        self.tool_y_frame.setObjectName(u"tool_y_frame")
        self.tool_y_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_y_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_y_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_22 = QHBoxLayout(self.tool_y_frame)
        self.horizontalLayout_22.setObjectName(u"horizontalLayout_22")
        self.horizontalLayout_22.setContentsMargins(-1, 9, -1, -1)
        self.label_25 = QLabel(self.tool_y_frame)
        self.label_25.setObjectName(u"label_25")
        self.label_25.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_22.addWidget(self.label_25)

        self.tool_y_label = QLabel(self.tool_y_frame)
        self.tool_y_label.setObjectName(u"tool_y_label")
        self.tool_y_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_y_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_22.addWidget(self.tool_y_label)


        self.verticalLayout_2.addWidget(self.tool_y_frame)

        self.tool_z_frame = QFrame(self.tool_pos_frame)
        self.tool_z_frame.setObjectName(u"tool_z_frame")
        self.tool_z_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_z_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_z_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_23 = QHBoxLayout(self.tool_z_frame)
        self.horizontalLayout_23.setObjectName(u"horizontalLayout_23")
        self.horizontalLayout_23.setContentsMargins(-1, 9, -1, -1)
        self.label_26 = QLabel(self.tool_z_frame)
        self.label_26.setObjectName(u"label_26")
        self.label_26.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_23.addWidget(self.label_26)

        self.tool_z_label = QLabel(self.tool_z_frame)
        self.tool_z_label.setObjectName(u"tool_z_label")
        self.tool_z_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_z_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_23.addWidget(self.tool_z_label)


        self.verticalLayout_2.addWidget(self.tool_z_frame)

        self.tool_rx_frame = QFrame(self.tool_pos_frame)
        self.tool_rx_frame.setObjectName(u"tool_rx_frame")
        self.tool_rx_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_rx_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_rx_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_26 = QHBoxLayout(self.tool_rx_frame)
        self.horizontalLayout_26.setObjectName(u"horizontalLayout_26")
        self.horizontalLayout_26.setContentsMargins(-1, 9, -1, -1)
        self.label_29 = QLabel(self.tool_rx_frame)
        self.label_29.setObjectName(u"label_29")
        self.label_29.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_26.addWidget(self.label_29)

        self.tool_rx_label = QLabel(self.tool_rx_frame)
        self.tool_rx_label.setObjectName(u"tool_rx_label")
        self.tool_rx_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_rx_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_26.addWidget(self.tool_rx_label)


        self.verticalLayout_2.addWidget(self.tool_rx_frame)

        self.tool_ry_frame = QFrame(self.tool_pos_frame)
        self.tool_ry_frame.setObjectName(u"tool_ry_frame")
        self.tool_ry_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_ry_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_ry_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_27 = QHBoxLayout(self.tool_ry_frame)
        self.horizontalLayout_27.setObjectName(u"horizontalLayout_27")
        self.horizontalLayout_27.setContentsMargins(-1, 9, -1, -1)
        self.label_30 = QLabel(self.tool_ry_frame)
        self.label_30.setObjectName(u"label_30")
        self.label_30.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_27.addWidget(self.label_30)

        self.tool_ry_label = QLabel(self.tool_ry_frame)
        self.tool_ry_label.setObjectName(u"tool_ry_label")
        self.tool_ry_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_ry_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_27.addWidget(self.tool_ry_label)


        self.verticalLayout_2.addWidget(self.tool_ry_frame)

        self.tool_rz_frame = QFrame(self.tool_pos_frame)
        self.tool_rz_frame.setObjectName(u"tool_rz_frame")
        self.tool_rz_frame.setMaximumSize(QSize(16777215, 30))
        self.tool_rz_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_rz_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_28 = QHBoxLayout(self.tool_rz_frame)
        self.horizontalLayout_28.setObjectName(u"horizontalLayout_28")
        self.horizontalLayout_28.setContentsMargins(-1, 9, -1, -1)
        self.label_31 = QLabel(self.tool_rz_frame)
        self.label_31.setObjectName(u"label_31")
        self.label_31.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_28.addWidget(self.label_31)

        self.tool_rz_label = QLabel(self.tool_rz_frame)
        self.tool_rz_label.setObjectName(u"tool_rz_label")
        self.tool_rz_label.setFrameShape(QFrame.Shape.NoFrame)
        self.tool_rz_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_28.addWidget(self.tool_rz_label)


        self.verticalLayout_2.addWidget(self.tool_rz_frame)


        self.horizontalLayout_35.addWidget(self.tool_pos_frame)

        self.joint_pos_frame = QFrame(self.frame_6)
        self.joint_pos_frame.setObjectName(u"joint_pos_frame")
        self.joint_pos_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_9 = QVBoxLayout(self.joint_pos_frame)
        self.verticalLayout_9.setObjectName(u"verticalLayout_9")
        self.verticalLayout_9.setContentsMargins(-1, 0, -1, 9)
        self.joint_pos__h1_label = QLabel(self.joint_pos_frame)
        self.joint_pos__h1_label.setObjectName(u"joint_pos__h1_label")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.joint_pos__h1_label.sizePolicy().hasHeightForWidth())
        self.joint_pos__h1_label.setSizePolicy(sizePolicy)

        self.verticalLayout_9.addWidget(self.joint_pos__h1_label)

        self.joint_pos_j1_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j1_frame.setObjectName(u"joint_pos_j1_frame")
        self.joint_pos_j1_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j1_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j1_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_29 = QHBoxLayout(self.joint_pos_j1_frame)
        self.horizontalLayout_29.setObjectName(u"horizontalLayout_29")
        self.horizontalLayout_29.setContentsMargins(-1, 9, -1, -1)
        self.label_32 = QLabel(self.joint_pos_j1_frame)
        self.label_32.setObjectName(u"label_32")
        self.label_32.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_29.addWidget(self.label_32)

        self.joint_pos_j1_label = QLabel(self.joint_pos_j1_frame)
        self.joint_pos_j1_label.setObjectName(u"joint_pos_j1_label")
        self.joint_pos_j1_label.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j1_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_29.addWidget(self.joint_pos_j1_label)


        self.verticalLayout_9.addWidget(self.joint_pos_j1_frame)

        self.joint_pos_j2_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j2_frame.setObjectName(u"joint_pos_j2_frame")
        self.joint_pos_j2_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j2_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j2_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_30 = QHBoxLayout(self.joint_pos_j2_frame)
        self.horizontalLayout_30.setObjectName(u"horizontalLayout_30")
        self.horizontalLayout_30.setContentsMargins(-1, 9, -1, -1)
        self.label_33 = QLabel(self.joint_pos_j2_frame)
        self.label_33.setObjectName(u"label_33")
        self.label_33.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_30.addWidget(self.label_33)

        self.joint_pos_j2_label = QLabel(self.joint_pos_j2_frame)
        self.joint_pos_j2_label.setObjectName(u"joint_pos_j2_label")
        self.joint_pos_j2_label.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j2_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_30.addWidget(self.joint_pos_j2_label)


        self.verticalLayout_9.addWidget(self.joint_pos_j2_frame)

        self.joint_pos_j3_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j3_frame.setObjectName(u"joint_pos_j3_frame")
        self.joint_pos_j3_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j3_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j3_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_31 = QHBoxLayout(self.joint_pos_j3_frame)
        self.horizontalLayout_31.setObjectName(u"horizontalLayout_31")
        self.horizontalLayout_31.setContentsMargins(-1, 9, -1, -1)
        self.label_34 = QLabel(self.joint_pos_j3_frame)
        self.label_34.setObjectName(u"label_34")
        self.label_34.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_31.addWidget(self.label_34)

        self.joint_pos_j3_label = QLabel(self.joint_pos_j3_frame)
        self.joint_pos_j3_label.setObjectName(u"joint_pos_j3_label")
        self.joint_pos_j3_label.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j3_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_31.addWidget(self.joint_pos_j3_label)


        self.verticalLayout_9.addWidget(self.joint_pos_j3_frame)

        self.joint_pos_j4_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j4_frame.setObjectName(u"joint_pos_j4_frame")
        self.joint_pos_j4_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j4_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j4_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_32 = QHBoxLayout(self.joint_pos_j4_frame)
        self.horizontalLayout_32.setObjectName(u"horizontalLayout_32")
        self.horizontalLayout_32.setContentsMargins(-1, 9, -1, -1)
        self.label_35 = QLabel(self.joint_pos_j4_frame)
        self.label_35.setObjectName(u"label_35")
        self.label_35.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_32.addWidget(self.label_35)

        self.joint_pos_j4_label = QLabel(self.joint_pos_j4_frame)
        self.joint_pos_j4_label.setObjectName(u"joint_pos_j4_label")
        self.joint_pos_j4_label.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j4_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_32.addWidget(self.joint_pos_j4_label)


        self.verticalLayout_9.addWidget(self.joint_pos_j4_frame)

        self.joint_pos_j5_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j5_frame.setObjectName(u"joint_pos_j5_frame")
        self.joint_pos_j5_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j5_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j5_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_33 = QHBoxLayout(self.joint_pos_j5_frame)
        self.horizontalLayout_33.setObjectName(u"horizontalLayout_33")
        self.horizontalLayout_33.setContentsMargins(-1, 9, -1, -1)
        self.label_36 = QLabel(self.joint_pos_j5_frame)
        self.label_36.setObjectName(u"label_36")
        self.label_36.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_33.addWidget(self.label_36)

        self.joint_pos_j5_label_2 = QLabel(self.joint_pos_j5_frame)
        self.joint_pos_j5_label_2.setObjectName(u"joint_pos_j5_label_2")
        self.joint_pos_j5_label_2.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j5_label_2.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_33.addWidget(self.joint_pos_j5_label_2)


        self.verticalLayout_9.addWidget(self.joint_pos_j5_frame)

        self.joint_pos_j6_frame = QFrame(self.joint_pos_frame)
        self.joint_pos_j6_frame.setObjectName(u"joint_pos_j6_frame")
        self.joint_pos_j6_frame.setMaximumSize(QSize(16777215, 30))
        self.joint_pos_j6_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j6_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_34 = QHBoxLayout(self.joint_pos_j6_frame)
        self.horizontalLayout_34.setObjectName(u"horizontalLayout_34")
        self.horizontalLayout_34.setContentsMargins(-1, 9, -1, -1)
        self.label_37 = QLabel(self.joint_pos_j6_frame)
        self.label_37.setObjectName(u"label_37")
        self.label_37.setMaximumSize(QSize(20, 16777215))

        self.horizontalLayout_34.addWidget(self.label_37)

        self.joint_pos_j6_label = QLabel(self.joint_pos_j6_frame)
        self.joint_pos_j6_label.setObjectName(u"joint_pos_j6_label")
        self.joint_pos_j6_label.setFrameShape(QFrame.Shape.NoFrame)
        self.joint_pos_j6_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout_34.addWidget(self.joint_pos_j6_label)


        self.verticalLayout_9.addWidget(self.joint_pos_j6_frame)


        self.horizontalLayout_35.addWidget(self.joint_pos_frame)

        self.frame_3 = QFrame(self.frame_6)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Shadow.Raised)

        self.horizontalLayout_35.addWidget(self.frame_3)


        self.gridLayout.addWidget(self.frame_6, 11, 0, 3, 4)

        self.log_frame = QFrame(self.main_container)
        self.log_frame.setObjectName(u"log_frame")
        self.log_frame.setMinimumSize(QSize(0, 0))
        self.log_frame.setMaximumSize(QSize(10000, 300))
        self.log_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.log_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_4 = QVBoxLayout(self.log_frame)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.log_label = QLabel(self.log_frame)
        self.log_label.setObjectName(u"log_label")
        font9 = QFont()
        font9.setPointSize(10)
        font9.setBold(True)
        self.log_label.setFont(font9)

        self.verticalLayout_4.addWidget(self.log_label)

        self.log_textEdit = QTextEdit(self.log_frame)
        self.log_textEdit.setObjectName(u"log_textEdit")
        self.log_textEdit.setMinimumSize(QSize(0, 0))
        self.log_textEdit.setFont(font4)
        self.log_textEdit.setStyleSheet(u"background-color: rgb(21, 21, 21);\n"
"color: #fff;\n"
"font-size: 10pt;\n"
"")
        self.log_textEdit.setFrameShape(QFrame.Shape.NoFrame)
        self.log_textEdit.setFrameShadow(QFrame.Shadow.Plain)
        self.log_textEdit.setReadOnly(True)

        self.verticalLayout_4.addWidget(self.log_textEdit)


        self.gridLayout.addWidget(self.log_frame, 11, 4, 3, 5)

        self.btn_frame = QFrame(self.main_container)
        self.btn_frame.setObjectName(u"btn_frame")
        self.btn_frame.setMaximumSize(QSize(10000, 50))
        self.btn_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.btn_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_3 = QHBoxLayout(self.btn_frame)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.btn_pushButton = QPushButton(self.btn_frame)
        self.btn_pushButton.setObjectName(u"btn_pushButton")
        self.btn_pushButton.setFont(font6)

        self.horizontalLayout_3.addWidget(self.btn_pushButton)

        self.btn_pushButton_2 = QPushButton(self.btn_frame)
        self.btn_pushButton_2.setObjectName(u"btn_pushButton_2")
        self.btn_pushButton_2.setFont(font6)

        self.horizontalLayout_3.addWidget(self.btn_pushButton_2)

        self.btn_load_prog_btn = QPushButton(self.btn_frame)
        self.btn_load_prog_btn.setObjectName(u"btn_load_prog_btn")
        self.btn_load_prog_btn.setEnabled(False)
        self.btn_load_prog_btn.setFont(font6)

        self.horizontalLayout_3.addWidget(self.btn_load_prog_btn)

        self.btn_horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.btn_horizontalSpacer)


        self.gridLayout.addWidget(self.btn_frame, 10, 4, 1, 5)

        self.con_frame = QFrame(self.main_container)
        self.con_frame.setObjectName(u"con_frame")
        self.con_frame.setMinimumSize(QSize(500, 50))
        self.con_frame.setMaximumSize(QSize(10000, 50))
        self.con_frame.setFont(font6)
        self.con_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.con_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout = QHBoxLayout(self.con_frame)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(9, -1, 50, -1)
        self.select_device_label = QLabel(self.con_frame)
        self.select_device_label.setObjectName(u"select_device_label")
        self.select_device_label.setFont(font9)
        self.select_device_label.setFrameShape(QFrame.Shape.NoFrame)
        self.select_device_label.setFrameShadow(QFrame.Shadow.Plain)

        self.horizontalLayout.addWidget(self.select_device_label)

        self.con_device_comboBox = QComboBox(self.con_frame)
        self.con_device_comboBox.setObjectName(u"con_device_comboBox")
        self.con_device_comboBox.setMinimumSize(QSize(100, 0))

        self.horizontalLayout.addWidget(self.con_device_comboBox)

        self.con_connect_btn = QPushButton(self.con_frame)
        self.con_connect_btn.setObjectName(u"con_connect_btn")

        self.horizontalLayout.addWidget(self.con_connect_btn)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer_2)

        self.con_status_label1 = QLabel(self.con_frame)
        self.con_status_label1.setObjectName(u"con_status_label1")
        self.con_status_label1.setFont(font9)

        self.horizontalLayout.addWidget(self.con_status_label1)

        self.con_status_label2 = QLabel(self.con_frame)
        self.con_status_label2.setObjectName(u"con_status_label2")
        self.con_status_label2.setMinimumSize(QSize(140, 0))
        self.con_status_label2.setFont(font9)

        self.horizontalLayout.addWidget(self.con_status_label2)


        self.gridLayout.addWidget(self.con_frame, 0, 0, 1, 4)

        self.jog_frame.raise_()
        self.frame_6.raise_()
        self.prog_frame.raise_()
        self.log_frame.raise_()
        self.cmd_frame.raise_()
        self.btn_frame.raise_()
        self.con_frame.raise_()

        self.verticalLayout_10.addWidget(self.main_container)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1268, 33))
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
        self.label.setText(QCoreApplication.translate("MainWindow", u"Joint 1", None))
        self.jog_j1_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j1_label.setText("")
        self.jog_j1_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Joint 2", None))
        self.jog_j2_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j2_label.setText("")
        self.jog_j2_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Joint 3", None))
        self.jog_j3_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j3_label.setText("")
        self.jog_j3_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Joint 4", None))
        self.jog_j4_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j4_label.setText("")
        self.jog_j4_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Joint 5", None))
        self.jog_j5_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j5_label.setText("")
        self.jog_j5_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_12.setText(QCoreApplication.translate("MainWindow", u"Joint 6", None))
        self.jog_j6_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j6_label.setText("")
        self.jog_j6_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.prog_label.setText(QCoreApplication.translate("MainWindow", u"Program:", None))
        self.prog_clear_btn.setText(QCoreApplication.translate("MainWindow", u"Clear", None))
        self.prog_textEdit.setPlaceholderText("")
        self.cmd_prog_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Programs", None))
        self.cmd_ping_btn.setText(QCoreApplication.translate("MainWindow", u"Ping", None))
        self.cmd_pong_btn.setText(QCoreApplication.translate("MainWindow", u"Pong", None))
        self.cmd_switches_btn.setText(QCoreApplication.translate("MainWindow", u"Test_Switches", None))
        self.cmd_condi_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Conditional", None))
        self.cmd_begin_pushButton.setText(QCoreApplication.translate("MainWindow", u"Begin", None))
        self.cmd_end_pushButton.setText(QCoreApplication.translate("MainWindow", u"End", None))
        self.tool_h1_label.setText(QCoreApplication.translate("MainWindow", u"Tool positions:", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"X:", None))
        self.tool_x_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_25.setText(QCoreApplication.translate("MainWindow", u"Y:", None))
        self.tool_y_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_26.setText(QCoreApplication.translate("MainWindow", u"Z:", None))
        self.tool_z_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_29.setText(QCoreApplication.translate("MainWindow", u"Rx:", None))
        self.tool_rx_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_30.setText(QCoreApplication.translate("MainWindow", u"Ry:", None))
        self.tool_ry_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_31.setText(QCoreApplication.translate("MainWindow", u"Rz:", None))
        self.tool_rz_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.joint_pos__h1_label.setText(QCoreApplication.translate("MainWindow", u"Joint positions:", None))
        self.label_32.setText(QCoreApplication.translate("MainWindow", u"J1:", None))
        self.joint_pos_j1_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_33.setText(QCoreApplication.translate("MainWindow", u"J2:", None))
        self.joint_pos_j2_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_34.setText(QCoreApplication.translate("MainWindow", u"J3:", None))
        self.joint_pos_j3_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_35.setText(QCoreApplication.translate("MainWindow", u"J4:", None))
        self.joint_pos_j4_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_36.setText(QCoreApplication.translate("MainWindow", u"J5:", None))
        self.joint_pos_j5_label_2.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.label_37.setText(QCoreApplication.translate("MainWindow", u"J6:", None))
        self.joint_pos_j6_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.log_label.setText(QCoreApplication.translate("MainWindow", u"Response log:", None))
        self.btn_pushButton.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.btn_pushButton_2.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.btn_load_prog_btn.setText(QCoreApplication.translate("MainWindow", u"Load Program", None))
        self.select_device_label.setText(QCoreApplication.translate("MainWindow", u"Select Device:", None))
        self.con_connect_btn.setText(QCoreApplication.translate("MainWindow", u"Connect", None))
        self.con_status_label1.setText(QCoreApplication.translate("MainWindow", u"Status:", None))
        self.con_status_label2.setText(QCoreApplication.translate("MainWindow", u"Disconnected", None))
        self.menuControls.setTitle(QCoreApplication.translate("MainWindow", u"Controls", None))
        pass
    # retranslateUi

