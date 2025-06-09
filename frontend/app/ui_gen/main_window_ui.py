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
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QAbstractScrollArea, QApplication, QComboBox, QFrame,
    QGridLayout, QHBoxLayout, QLabel, QLayout,
    QMainWindow, QPushButton, QSizePolicy, QSlider,
    QSpacerItem, QStatusBar, QTabWidget, QTextEdit,
    QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1278, 800)
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
        self.tabWidget = QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tabWidget.setStyleSheet(u"QTabWidget::pane {\n"
"    border: none;\n"
"    background: transparent;\n"
"}\n"
"")
        self.tabWidget.setTabShape(QTabWidget.TabShape.Rounded)
        self.tab_control = QWidget()
        self.tab_control.setObjectName(u"tab_control")
        self.tab_control.setAutoFillBackground(True)
        self.tab_control.setStyleSheet(u"QTabWidget::pane {\n"
"    border: none;\n"
"}")
        self.verticalLayout_11 = QVBoxLayout(self.tab_control)
        self.verticalLayout_11.setSpacing(0)
        self.verticalLayout_11.setObjectName(u"verticalLayout_11")
        self.verticalLayout_11.setContentsMargins(0, 0, 0, 0)
        self.control_container = QFrame(self.tab_control)
        self.control_container.setObjectName(u"control_container")
        self.control_container.setMinimumSize(QSize(0, 0))
        self.control_container.setMaximumSize(QSize(10000, 10000))
        font1 = QFont()
        font1.setPointSize(9)
        font1.setBold(False)
        self.control_container.setFont(font1)
        self.control_container.setFrameShape(QFrame.Shape.NoFrame)
        self.control_container.setFrameShadow(QFrame.Shadow.Plain)
        self.gridLayout = QGridLayout(self.control_container)
        self.gridLayout.setSpacing(4)
        self.gridLayout.setObjectName(u"gridLayout")
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.frame_6 = QFrame(self.control_container)
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

        self.util_frame = QFrame(self.frame_6)
        self.util_frame.setObjectName(u"util_frame")
        self.util_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.util_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_4 = QVBoxLayout(self.util_frame)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.verticalLayout_4.setContentsMargins(9, 0, 9, -1)
        self.label = QLabel(self.util_frame)
        self.label.setObjectName(u"label")

        self.verticalLayout_4.addWidget(self.label)

        self.jog_slider_frame = QFrame(self.util_frame)
        self.jog_slider_frame.setObjectName(u"jog_slider_frame")
        self.jog_slider_frame.setFrameShape(QFrame.Shape.NoFrame)
        self.jog_slider_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_12 = QHBoxLayout(self.jog_slider_frame)
        self.horizontalLayout_12.setObjectName(u"horizontalLayout_12")
        self.horizontalLayout_12.setContentsMargins(0, 0, 0, -1)
        self.jog_slider = QSlider(self.jog_slider_frame)
        self.jog_slider.setObjectName(u"jog_slider")
        self.jog_slider.setMinimum(0)
        self.jog_slider.setMaximum(100)
        self.jog_slider.setSingleStep(10)
        self.jog_slider.setValue(50)
        self.jog_slider.setSliderPosition(50)
        self.jog_slider.setOrientation(Qt.Orientation.Horizontal)
        self.jog_slider.setTickPosition(QSlider.TickPosition.NoTicks)
        self.jog_slider.setTickInterval(11)

        self.horizontalLayout_12.addWidget(self.jog_slider)


        self.verticalLayout_4.addWidget(self.jog_slider_frame)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_4.addItem(self.verticalSpacer)


        self.horizontalLayout_35.addWidget(self.util_frame)

        self.horizontalLayout_35.setStretch(0, 2)
        self.horizontalLayout_35.setStretch(1, 2)
        self.horizontalLayout_35.setStretch(2, 3)

        self.gridLayout.addWidget(self.frame_6, 11, 0, 3, 4)

        self.jog_frame = QFrame(self.control_container)
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

        self.label_1 = QLabel(self.jog_j1_frame)
        self.label_1.setObjectName(u"label_1")
        font2 = QFont()
        font2.setPointSize(20)
        font2.setBold(True)
        self.label_1.setFont(font2)

        self.horizontalLayout_2.addWidget(self.label_1)

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

        self.cmd_frame = QFrame(self.control_container)
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
        font5 = QFont()
        font5.setPointSize(9)
        font5.setBold(False)
        font5.setStrikeOut(False)
        font5.setKerning(True)
        font5.setStyleStrategy(QFont.PreferDefault)
        self.cmd_pong_btn.setFont(font5)
        self.cmd_pong_btn.setAutoFillBackground(False)

        self.verticalLayout_6.addWidget(self.cmd_pong_btn)

        self.cmd_switches_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_switches_btn.setObjectName(u"cmd_switches_btn")
        self.cmd_switches_btn.setFont(font5)
        self.cmd_switches_btn.setAutoFillBackground(False)

        self.verticalLayout_6.addWidget(self.cmd_switches_btn)

        self.cmd_home_axis_btn = QPushButton(self.cmd_prog_frame)
        self.cmd_home_axis_btn.setObjectName(u"cmd_home_axis_btn")

        self.verticalLayout_6.addWidget(self.cmd_home_axis_btn)


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

        self.prog_frame = QFrame(self.control_container)
        self.prog_frame.setObjectName(u"prog_frame")
        self.prog_frame.setMaximumSize(QSize(100000, 1000))
        self.prog_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.prog_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.prog_frame)
        self.verticalLayout_3.setSpacing(6)
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
        font6 = QFont()
        font6.setPointSize(10)
        font6.setBold(True)
        font6.setItalic(False)
        font6.setUnderline(False)
        self.prog_label.setFont(font6)

        self.horizontalLayout_4.addWidget(self.prog_label)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer)

        self.prog_clear_btn = QPushButton(self.prog_header_frame)
        self.prog_clear_btn.setObjectName(u"prog_clear_btn")
        self.prog_clear_btn.setMaximumSize(QSize(16777211, 16777215))
        font7 = QFont()
        font7.setPointSize(9)
        font7.setBold(True)
        self.prog_clear_btn.setFont(font7)

        self.horizontalLayout_4.addWidget(self.prog_clear_btn)


        self.verticalLayout_3.addWidget(self.prog_header_frame)

        self.prog_textEdit = QTextEdit(self.prog_frame)
        self.prog_textEdit.setObjectName(u"prog_textEdit")
        font8 = QFont()
        font8.setFamilies([u"Arial"])
        font8.setPointSize(10)
        font8.setBold(False)
        font8.setKerning(False)
        self.prog_textEdit.setFont(font8)
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

        self.log_frame = QFrame(self.control_container)
        self.log_frame.setObjectName(u"log_frame")
        self.log_frame.setMinimumSize(QSize(0, 0))
        self.log_frame.setMaximumSize(QSize(10000, 300))
        self.log_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.log_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_8 = QVBoxLayout(self.log_frame)
        self.verticalLayout_8.setSpacing(6)
        self.verticalLayout_8.setObjectName(u"verticalLayout_8")
        self.verticalLayout_8.setContentsMargins(-1, 9, -1, 9)
        self.frame = QFrame(self.log_frame)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.Shape.NoFrame)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_5 = QHBoxLayout(self.frame)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.horizontalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.log_label = QLabel(self.frame)
        self.log_label.setObjectName(u"log_label")
        font9 = QFont()
        font9.setPointSize(10)
        font9.setBold(True)
        self.log_label.setFont(font9)

        self.horizontalLayout_5.addWidget(self.log_label)

        self.horizontalSpacer_4 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_4)

        self.pushButton = QPushButton(self.frame)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setFont(font7)

        self.horizontalLayout_5.addWidget(self.pushButton)


        self.verticalLayout_8.addWidget(self.frame)

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

        self.verticalLayout_8.addWidget(self.log_textEdit)


        self.gridLayout.addWidget(self.log_frame, 11, 4, 3, 5)

        self.btn_frame = QFrame(self.control_container)
        self.btn_frame.setObjectName(u"btn_frame")
        self.btn_frame.setMaximumSize(QSize(10000, 50))
        self.btn_frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.btn_frame.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_3 = QHBoxLayout(self.btn_frame)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.btn_start = QPushButton(self.btn_frame)
        self.btn_start.setObjectName(u"btn_start")
        self.btn_start.setEnabled(False)
        self.btn_start.setFont(font7)

        self.horizontalLayout_3.addWidget(self.btn_start)

        self.btn_stop = QPushButton(self.btn_frame)
        self.btn_stop.setObjectName(u"btn_stop")
        self.btn_stop.setEnabled(False)
        self.btn_stop.setFont(font7)

        self.horizontalLayout_3.addWidget(self.btn_stop)

        self.btn_load_prog_btn = QPushButton(self.btn_frame)
        self.btn_load_prog_btn.setObjectName(u"btn_load_prog_btn")
        self.btn_load_prog_btn.setEnabled(False)
        self.btn_load_prog_btn.setFont(font7)

        self.horizontalLayout_3.addWidget(self.btn_load_prog_btn)

        self.btn_horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.btn_horizontalSpacer)


        self.gridLayout.addWidget(self.btn_frame, 10, 4, 1, 5)

        self.con_frame = QFrame(self.control_container)
        self.con_frame.setObjectName(u"con_frame")
        self.con_frame.setMinimumSize(QSize(500, 50))
        self.con_frame.setMaximumSize(QSize(10000, 50))
        self.con_frame.setFont(font7)
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

        self.log_frame.raise_()
        self.jog_frame.raise_()
        self.frame_6.raise_()
        self.prog_frame.raise_()
        self.cmd_frame.raise_()
        self.btn_frame.raise_()
        self.con_frame.raise_()

        self.verticalLayout_11.addWidget(self.control_container)

        self.tabWidget.addTab(self.tab_control, "")
        self.tab_setup = QWidget()
        self.tab_setup.setObjectName(u"tab_setup")
        self.tab_setup.setStyleSheet(u"QTabWidget::pane {\n"
"    border: none;\n"
"}")
        self.setup_container = QFrame(self.tab_setup)
        self.setup_container.setObjectName(u"setup_container")
        self.setup_container.setGeometry(QRect(0, 0, 1181, 681))
        self.setup_container.setFrameShape(QFrame.Shape.StyledPanel)
        self.setup_container.setFrameShadow(QFrame.Shadow.Raised)
        self.frame_2 = QFrame(self.setup_container)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setGeometry(QRect(10, 10, 451, 301))
        self.frame_2.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_12 = QVBoxLayout(self.frame_2)
        self.verticalLayout_12.setObjectName(u"verticalLayout_12")
        self.label_2 = QLabel(self.frame_2)
        self.label_2.setObjectName(u"label_2")

        self.verticalLayout_12.addWidget(self.label_2)

        self.gridLayout_2 = QGridLayout()
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.gridLayout_2.setContentsMargins(0, -1, -1, -1)
        self.dh_j2_a_v = QTextEdit(self.frame_2)
        self.dh_j2_a_v.setObjectName(u"dh_j2_a_v")
        self.dh_j2_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j2_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j2_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j2_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j2_a_v, 2, 4, 1, 1)

        self.label_15 = QLabel(self.frame_2)
        self.label_15.setObjectName(u"label_15")
        self.label_15.setFont(font9)

        self.gridLayout_2.addWidget(self.label_15, 3, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_18 = QLabel(self.frame_2)
        self.label_18.setObjectName(u"label_18")
        self.label_18.setFont(font9)

        self.gridLayout_2.addWidget(self.label_18, 5, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j2_d_v = QTextEdit(self.frame_2)
        self.dh_j2_d_v.setObjectName(u"dh_j2_d_v")
        self.dh_j2_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j2_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j2_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j2_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j2_d_v, 2, 3, 1, 1)

        self.label_11 = QLabel(self.frame_2)
        self.label_11.setObjectName(u"label_11")
        self.label_11.setFont(font9)

        self.gridLayout_2.addWidget(self.label_11, 0, 4, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_17 = QLabel(self.frame_2)
        self.label_17.setObjectName(u"label_17")
        self.label_17.setMaximumSize(QSize(16777215, 20))
        self.label_17.setFont(font9)

        self.gridLayout_2.addWidget(self.label_17, 1, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j1_aph_v = QTextEdit(self.frame_2)
        self.dh_j1_aph_v.setObjectName(u"dh_j1_aph_v")
        self.dh_j1_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j1_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j1_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j1_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j1_aph_v, 1, 2, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_9 = QLabel(self.frame_2)
        self.label_9.setObjectName(u"label_9")
        self.label_9.setFont(font9)

        self.gridLayout_2.addWidget(self.label_9, 0, 3, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_14 = QLabel(self.frame_2)
        self.label_14.setObjectName(u"label_14")
        self.label_14.setFont(font9)

        self.gridLayout_2.addWidget(self.label_14, 2, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j5_aph_v = QTextEdit(self.frame_2)
        self.dh_j5_aph_v.setObjectName(u"dh_j5_aph_v")
        self.dh_j5_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j5_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j5_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j5_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j5_aph_v, 5, 2, 1, 1)

        self.label_7 = QLabel(self.frame_2)
        self.label_7.setObjectName(u"label_7")
        self.label_7.setFont(font9)

        self.gridLayout_2.addWidget(self.label_7, 0, 2, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j3_a_v = QTextEdit(self.frame_2)
        self.dh_j3_a_v.setObjectName(u"dh_j3_a_v")
        self.dh_j3_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j3_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j3_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j3_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j3_a_v, 3, 4, 1, 1)

        self.dh_j5_a_v = QTextEdit(self.frame_2)
        self.dh_j5_a_v.setObjectName(u"dh_j5_a_v")
        self.dh_j5_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j5_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j5_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j5_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j5_a_v, 5, 4, 1, 1)

        self.dh_j5_d_v = QTextEdit(self.frame_2)
        self.dh_j5_d_v.setObjectName(u"dh_j5_d_v")
        self.dh_j5_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j5_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j5_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j5_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j5_d_v, 5, 3, 1, 1)

        self.dh_j3_aph_v = QTextEdit(self.frame_2)
        self.dh_j3_aph_v.setObjectName(u"dh_j3_aph_v")
        self.dh_j3_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j3_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j3_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j3_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j3_aph_v, 3, 2, 1, 1)

        self.label_16 = QLabel(self.frame_2)
        self.label_16.setObjectName(u"label_16")
        self.label_16.setFont(font9)

        self.gridLayout_2.addWidget(self.label_16, 4, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j6_aph_v = QTextEdit(self.frame_2)
        self.dh_j6_aph_v.setObjectName(u"dh_j6_aph_v")
        self.dh_j6_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j6_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j6_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j6_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j6_aph_v, 6, 2, 1, 1)

        self.label_19 = QLabel(self.frame_2)
        self.label_19.setObjectName(u"label_19")
        self.label_19.setFont(font9)

        self.gridLayout_2.addWidget(self.label_19, 6, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j6_a_v = QTextEdit(self.frame_2)
        self.dh_j6_a_v.setObjectName(u"dh_j6_a_v")
        self.dh_j6_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j6_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j6_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j6_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j6_a_v, 6, 4, 1, 1)

        self.dh_j2_aph_v = QTextEdit(self.frame_2)
        self.dh_j2_aph_v.setObjectName(u"dh_j2_aph_v")
        self.dh_j2_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j2_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j2_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j2_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j2_aph_v, 2, 2, 1, 1)

        self.dh_j1_a_v = QTextEdit(self.frame_2)
        self.dh_j1_a_v.setObjectName(u"dh_j1_a_v")
        self.dh_j1_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j1_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j1_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j1_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j1_a_v, 1, 4, 1, 1)

        self.dh_j1_d_v = QTextEdit(self.frame_2)
        self.dh_j1_d_v.setObjectName(u"dh_j1_d_v")
        self.dh_j1_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j1_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j1_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j1_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j1_d_v, 1, 3, 1, 1)

        self.dh_j3_d_v = QTextEdit(self.frame_2)
        self.dh_j3_d_v.setObjectName(u"dh_j3_d_v")
        self.dh_j3_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j3_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j3_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j3_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j3_d_v, 3, 3, 1, 1)

        self.joint_label = QLabel(self.frame_2)
        self.joint_label.setObjectName(u"joint_label")
        self.joint_label.setMaximumSize(QSize(100, 20))
        self.joint_label.setFont(font9)

        self.gridLayout_2.addWidget(self.joint_label, 0, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j6_d_v = QTextEdit(self.frame_2)
        self.dh_j6_d_v.setObjectName(u"dh_j6_d_v")
        self.dh_j6_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j6_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j6_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j6_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j6_d_v, 6, 3, 1, 1)

        self.dh_j4_a_v = QTextEdit(self.frame_2)
        self.dh_j4_a_v.setObjectName(u"dh_j4_a_v")
        self.dh_j4_a_v.setMaximumSize(QSize(100, 30))
        self.dh_j4_a_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j4_a_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j4_a_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j4_a_v, 4, 4, 1, 1)

        self.dh_j4_d_v = QTextEdit(self.frame_2)
        self.dh_j4_d_v.setObjectName(u"dh_j4_d_v")
        self.dh_j4_d_v.setMaximumSize(QSize(100, 30))
        self.dh_j4_d_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j4_d_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j4_d_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j4_d_v, 4, 3, 1, 1)

        self.dh_j4_aph_v = QTextEdit(self.frame_2)
        self.dh_j4_aph_v.setObjectName(u"dh_j4_aph_v")
        self.dh_j4_aph_v.setMaximumSize(QSize(100, 30))
        self.dh_j4_aph_v.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j4_aph_v.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j4_aph_v.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j4_aph_v, 4, 2, 1, 1)

        self.label_53 = QLabel(self.frame_2)
        self.label_53.setObjectName(u"label_53")
        self.label_53.setFont(font9)

        self.gridLayout_2.addWidget(self.label_53, 0, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.dh_j1_theta_off = QTextEdit(self.frame_2)
        self.dh_j1_theta_off.setObjectName(u"dh_j1_theta_off")
        self.dh_j1_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j1_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j1_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j1_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j1_theta_off, 1, 1, 1, 1)

        self.dh_j2_theta_off = QTextEdit(self.frame_2)
        self.dh_j2_theta_off.setObjectName(u"dh_j2_theta_off")
        self.dh_j2_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j2_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j2_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j2_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j2_theta_off, 2, 1, 1, 1)

        self.dh_j3_theta_off = QTextEdit(self.frame_2)
        self.dh_j3_theta_off.setObjectName(u"dh_j3_theta_off")
        self.dh_j3_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j3_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j3_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j3_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j3_theta_off, 3, 1, 1, 1)

        self.dh_j4_theta_off = QTextEdit(self.frame_2)
        self.dh_j4_theta_off.setObjectName(u"dh_j4_theta_off")
        self.dh_j4_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j4_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j4_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j4_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j4_theta_off, 4, 1, 1, 1)

        self.dh_j5_theta_off = QTextEdit(self.frame_2)
        self.dh_j5_theta_off.setObjectName(u"dh_j5_theta_off")
        self.dh_j5_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j5_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j5_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j5_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j5_theta_off, 5, 1, 1, 1)

        self.dh_j6_theta_off = QTextEdit(self.frame_2)
        self.dh_j6_theta_off.setObjectName(u"dh_j6_theta_off")
        self.dh_j6_theta_off.setMaximumSize(QSize(100, 30))
        self.dh_j6_theta_off.setFrameShape(QFrame.Shape.WinPanel)
        self.dh_j6_theta_off.setFrameShadow(QFrame.Shadow.Sunken)
        self.dh_j6_theta_off.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_2.addWidget(self.dh_j6_theta_off, 6, 1, 1, 1)


        self.verticalLayout_12.addLayout(self.gridLayout_2)

        self.frame_4 = QFrame(self.setup_container)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setGeometry(QRect(580, 20, 484, 277))
        self.frame_4.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_13 = QVBoxLayout(self.frame_4)
        self.verticalLayout_13.setObjectName(u"verticalLayout_13")
        self.label_39 = QLabel(self.frame_4)
        self.label_39.setObjectName(u"label_39")

        self.verticalLayout_13.addWidget(self.label_39)

        self.gridLayout_5 = QGridLayout()
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.gridLayout_5.setContentsMargins(0, -1, -1, -1)
        self.label_44 = QLabel(self.frame_4)
        self.label_44.setObjectName(u"label_44")
        self.label_44.setFont(font9)

        self.gridLayout_5.addWidget(self.label_44, 0, 2, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_6_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_6_bck_vel.setObjectName(u"home_mtr_6_bck_vel")
        self.home_mtr_6_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_6_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_6_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_6_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_6_bck_vel, 6, 3, 1, 1)

        self.label_48 = QLabel(self.frame_4)
        self.label_48.setObjectName(u"label_48")
        self.label_48.setFont(font9)

        self.gridLayout_5.addWidget(self.label_48, 6, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_2_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_2_h_pos.setObjectName(u"home_mtr_2_h_pos")
        self.home_mtr_2_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_2_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_2_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_2_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_2_h_pos, 2, 4, 1, 1)

        self.home_mtr_3_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_3_bck_vel.setObjectName(u"home_mtr_3_bck_vel")
        self.home_mtr_3_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_3_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_3_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_3_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_3_bck_vel, 3, 3, 1, 1)

        self.home_mtr_3_vel = QTextEdit(self.frame_4)
        self.home_mtr_3_vel.setObjectName(u"home_mtr_3_vel")
        self.home_mtr_3_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_3_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_3_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_3_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_3_vel, 3, 1, 1, 1)

        self.home_mtr_1_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_1_aw_vel.setObjectName(u"home_mtr_1_aw_vel")
        self.home_mtr_1_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_1_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_1_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_1_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_1_aw_vel, 1, 2, 1, 1)

        self.motor_label = QLabel(self.frame_4)
        self.motor_label.setObjectName(u"motor_label")
        self.motor_label.setMaximumSize(QSize(100, 20))
        self.motor_label.setFont(font9)

        self.gridLayout_5.addWidget(self.motor_label, 0, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_6_vel = QTextEdit(self.frame_4)
        self.home_mtr_6_vel.setObjectName(u"home_mtr_6_vel")
        self.home_mtr_6_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_6_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_6_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_6_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_6_vel, 6, 1, 1, 1)

        self.home_mtr_3_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_3_aw_vel.setObjectName(u"home_mtr_3_aw_vel")
        self.home_mtr_3_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_3_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_3_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_3_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_3_aw_vel, 3, 2, 1, 1)

        self.home_mtr_1_vel = QTextEdit(self.frame_4)
        self.home_mtr_1_vel.setObjectName(u"home_mtr_1_vel")
        self.home_mtr_1_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_1_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_1_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_1_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_1_vel, 1, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_41 = QLabel(self.frame_4)
        self.label_41.setObjectName(u"label_41")
        self.label_41.setFont(font9)

        self.gridLayout_5.addWidget(self.label_41, 5, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_5_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_5_aw_vel.setObjectName(u"home_mtr_5_aw_vel")
        self.home_mtr_5_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_5_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_5_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_5_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_5_aw_vel, 5, 2, 1, 1)

        self.home_mtr_6_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_6_aw_vel.setObjectName(u"home_mtr_6_aw_vel")
        self.home_mtr_6_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_6_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_6_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_6_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_6_aw_vel, 6, 2, 1, 1)

        self.home_mtr_4_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_4_bck_vel.setObjectName(u"home_mtr_4_bck_vel")
        self.home_mtr_4_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_4_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_4_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_4_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_4_bck_vel, 4, 3, 1, 1)

        self.home_mtr_4_vel = QTextEdit(self.frame_4)
        self.home_mtr_4_vel.setObjectName(u"home_mtr_4_vel")
        self.home_mtr_4_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_4_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_4_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_4_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_4_vel, 4, 1, 1, 1)

        self.label_49 = QLabel(self.frame_4)
        self.label_49.setObjectName(u"label_49")
        self.label_49.setFont(font9)

        self.gridLayout_5.addWidget(self.label_49, 0, 4, 1, 1)

        self.label_40 = QLabel(self.frame_4)
        self.label_40.setObjectName(u"label_40")
        self.label_40.setFont(font9)

        self.gridLayout_5.addWidget(self.label_40, 3, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_46 = QLabel(self.frame_4)
        self.label_46.setObjectName(u"label_46")
        self.label_46.setFont(font9)

        self.gridLayout_5.addWidget(self.label_46, 0, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_1_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_1_h_pos.setObjectName(u"home_mtr_1_h_pos")
        self.home_mtr_1_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_1_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_1_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_1_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_1_h_pos, 1, 4, 1, 1)

        self.home_mtr_4_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_4_aw_vel.setObjectName(u"home_mtr_4_aw_vel")
        self.home_mtr_4_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_4_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_4_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_4_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_4_aw_vel, 4, 2, 1, 1)

        self.label_42 = QLabel(self.frame_4)
        self.label_42.setObjectName(u"label_42")
        self.label_42.setFont(font9)

        self.gridLayout_5.addWidget(self.label_42, 0, 3, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_1_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_1_bck_vel.setObjectName(u"home_mtr_1_bck_vel")
        self.home_mtr_1_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_1_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_1_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_1_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_1_bck_vel, 1, 3, 1, 1)

        self.home_mtr_2_aw_vel = QTextEdit(self.frame_4)
        self.home_mtr_2_aw_vel.setObjectName(u"home_mtr_2_aw_vel")
        self.home_mtr_2_aw_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_2_aw_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_2_aw_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_2_aw_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_2_aw_vel, 2, 2, 1, 1)

        self.home_mtr_5_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_5_bck_vel.setObjectName(u"home_mtr_5_bck_vel")
        self.home_mtr_5_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_5_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_5_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_5_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_5_bck_vel, 5, 3, 1, 1)

        self.home_mtr_5_vel = QTextEdit(self.frame_4)
        self.home_mtr_5_vel.setObjectName(u"home_mtr_5_vel")
        self.home_mtr_5_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_5_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_5_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_5_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_5_vel, 5, 1, 1, 1)

        self.label_47 = QLabel(self.frame_4)
        self.label_47.setObjectName(u"label_47")
        self.label_47.setFont(font9)

        self.gridLayout_5.addWidget(self.label_47, 4, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_45 = QLabel(self.frame_4)
        self.label_45.setObjectName(u"label_45")
        self.label_45.setFont(font9)

        self.gridLayout_5.addWidget(self.label_45, 2, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_2_vel = QTextEdit(self.frame_4)
        self.home_mtr_2_vel.setObjectName(u"home_mtr_2_vel")
        self.home_mtr_2_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_2_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_2_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_2_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_2_vel, 2, 1, 1, 1)

        self.home_mtr_2_bck_vel = QTextEdit(self.frame_4)
        self.home_mtr_2_bck_vel.setObjectName(u"home_mtr_2_bck_vel")
        self.home_mtr_2_bck_vel.setMaximumSize(QSize(100, 30))
        self.home_mtr_2_bck_vel.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_2_bck_vel.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_2_bck_vel.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_2_bck_vel, 2, 3, 1, 1)

        self.label_43 = QLabel(self.frame_4)
        self.label_43.setObjectName(u"label_43")
        self.label_43.setMaximumSize(QSize(16777215, 20))
        self.label_43.setFont(font9)

        self.gridLayout_5.addWidget(self.label_43, 1, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.home_mtr_3_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_3_h_pos.setObjectName(u"home_mtr_3_h_pos")
        self.home_mtr_3_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_3_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_3_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_3_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_3_h_pos, 3, 4, 1, 1)

        self.home_mtr_4_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_4_h_pos.setObjectName(u"home_mtr_4_h_pos")
        self.home_mtr_4_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_4_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_4_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_4_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_4_h_pos, 4, 4, 1, 1)

        self.home_mtr_5_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_5_h_pos.setObjectName(u"home_mtr_5_h_pos")
        self.home_mtr_5_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_5_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_5_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_5_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_5_h_pos, 5, 4, 1, 1)

        self.home_mtr_6_h_pos = QTextEdit(self.frame_4)
        self.home_mtr_6_h_pos.setObjectName(u"home_mtr_6_h_pos")
        self.home_mtr_6_h_pos.setMaximumSize(QSize(100, 30))
        self.home_mtr_6_h_pos.setFrameShape(QFrame.Shape.WinPanel)
        self.home_mtr_6_h_pos.setFrameShadow(QFrame.Shadow.Sunken)
        self.home_mtr_6_h_pos.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_5.addWidget(self.home_mtr_6_h_pos, 6, 4, 1, 1)


        self.verticalLayout_13.addLayout(self.gridLayout_5)

        self.frame_5 = QFrame(self.setup_container)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setGeometry(QRect(20, 340, 261, 277))
        self.frame_5.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_14 = QVBoxLayout(self.frame_5)
        self.verticalLayout_14.setObjectName(u"verticalLayout_14")
        self.label_50 = QLabel(self.frame_5)
        self.label_50.setObjectName(u"label_50")

        self.verticalLayout_14.addWidget(self.label_50)

        self.gridLayout_6 = QGridLayout()
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.gridLayout_6.setContentsMargins(0, -1, -1, -1)
        self.label_52 = QLabel(self.frame_5)
        self.label_52.setObjectName(u"label_52")
        self.label_52.setFont(font9)

        self.gridLayout_6.addWidget(self.label_52, 5, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.motor_4_acc = QTextEdit(self.frame_5)
        self.motor_4_acc.setObjectName(u"motor_4_acc")
        self.motor_4_acc.setMaximumSize(QSize(100, 30))
        self.motor_4_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_4_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_4_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_4_acc, 4, 2, 1, 1)

        self.motor_4_max_speed = QTextEdit(self.frame_5)
        self.motor_4_max_speed.setObjectName(u"motor_4_max_speed")
        self.motor_4_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_4_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_4_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_4_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_4_max_speed, 4, 1, 1, 1)

        self.motor_1_max_speed = QTextEdit(self.frame_5)
        self.motor_1_max_speed.setObjectName(u"motor_1_max_speed")
        self.motor_1_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_1_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_1_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_1_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_1_max_speed, 1, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_57 = QLabel(self.frame_5)
        self.label_57.setObjectName(u"label_57")
        self.label_57.setFont(font9)

        self.gridLayout_6.addWidget(self.label_57, 0, 1, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_55 = QLabel(self.frame_5)
        self.label_55.setObjectName(u"label_55")
        self.label_55.setFont(font9)

        self.gridLayout_6.addWidget(self.label_55, 0, 2, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_59 = QLabel(self.frame_5)
        self.label_59.setObjectName(u"label_59")
        self.label_59.setFont(font9)

        self.gridLayout_6.addWidget(self.label_59, 6, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.motor_5_max_speed = QTextEdit(self.frame_5)
        self.motor_5_max_speed.setObjectName(u"motor_5_max_speed")
        self.motor_5_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_5_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_5_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_5_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_5_max_speed, 5, 1, 1, 1)

        self.motor_5_acc = QTextEdit(self.frame_5)
        self.motor_5_acc.setObjectName(u"motor_5_acc")
        self.motor_5_acc.setMaximumSize(QSize(100, 30))
        self.motor_5_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_5_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_5_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_5_acc, 5, 2, 1, 1)

        self.motor_2_max_speed = QTextEdit(self.frame_5)
        self.motor_2_max_speed.setObjectName(u"motor_2_max_speed")
        self.motor_2_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_2_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_2_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_2_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_2_max_speed, 2, 1, 1, 1)

        self.label_56 = QLabel(self.frame_5)
        self.label_56.setObjectName(u"label_56")
        self.label_56.setFont(font9)

        self.gridLayout_6.addWidget(self.label_56, 2, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.motor_label_2 = QLabel(self.frame_5)
        self.motor_label_2.setObjectName(u"motor_label_2")
        self.motor_label_2.setMaximumSize(QSize(100, 20))
        self.motor_label_2.setFont(font9)

        self.gridLayout_6.addWidget(self.motor_label_2, 0, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.motor_6_max_speed = QTextEdit(self.frame_5)
        self.motor_6_max_speed.setObjectName(u"motor_6_max_speed")
        self.motor_6_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_6_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_6_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_6_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_6_max_speed, 6, 1, 1, 1)

        self.motor_2_acc = QTextEdit(self.frame_5)
        self.motor_2_acc.setObjectName(u"motor_2_acc")
        self.motor_2_acc.setMaximumSize(QSize(100, 30))
        self.motor_2_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_2_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_2_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_2_acc, 2, 2, 1, 1)

        self.motor_3_acc = QTextEdit(self.frame_5)
        self.motor_3_acc.setObjectName(u"motor_3_acc")
        self.motor_3_acc.setMaximumSize(QSize(100, 30))
        self.motor_3_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_3_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_3_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_3_acc, 3, 2, 1, 1)

        self.label_51 = QLabel(self.frame_5)
        self.label_51.setObjectName(u"label_51")
        self.label_51.setFont(font9)

        self.gridLayout_6.addWidget(self.label_51, 3, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_54 = QLabel(self.frame_5)
        self.label_54.setObjectName(u"label_54")
        self.label_54.setMaximumSize(QSize(16777215, 20))
        self.label_54.setFont(font9)

        self.gridLayout_6.addWidget(self.label_54, 1, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.label_58 = QLabel(self.frame_5)
        self.label_58.setObjectName(u"label_58")
        self.label_58.setFont(font9)

        self.gridLayout_6.addWidget(self.label_58, 4, 0, 1, 1, Qt.AlignmentFlag.AlignHCenter)

        self.motor_3_max_speed = QTextEdit(self.frame_5)
        self.motor_3_max_speed.setObjectName(u"motor_3_max_speed")
        self.motor_3_max_speed.setMaximumSize(QSize(100, 30))
        self.motor_3_max_speed.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_3_max_speed.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_3_max_speed.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_3_max_speed, 3, 1, 1, 1)

        self.motor_1_acc = QTextEdit(self.frame_5)
        self.motor_1_acc.setObjectName(u"motor_1_acc")
        self.motor_1_acc.setMaximumSize(QSize(100, 30))
        self.motor_1_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_1_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_1_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_1_acc, 1, 2, 1, 1)

        self.motor_6_acc = QTextEdit(self.frame_5)
        self.motor_6_acc.setObjectName(u"motor_6_acc")
        self.motor_6_acc.setMaximumSize(QSize(100, 30))
        self.motor_6_acc.setFrameShape(QFrame.Shape.WinPanel)
        self.motor_6_acc.setFrameShadow(QFrame.Shadow.Sunken)
        self.motor_6_acc.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.gridLayout_6.addWidget(self.motor_6_acc, 6, 2, 1, 1)


        self.verticalLayout_14.addLayout(self.gridLayout_6)

        self.setup_save_btn = QPushButton(self.setup_container)
        self.setup_save_btn.setObjectName(u"setup_save_btn")
        self.setup_save_btn.setGeometry(QRect(1040, 600, 75, 24))
        self.tabWidget.addTab(self.tab_setup, "")

        self.verticalLayout_10.addWidget(self.tabWidget)

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.cmd_prog_menu_pushButton.toggled.connect(self.cmd_prog_frame.setVisible)
        self.cmd_condi_menu_pushButton.toggled.connect(self.cmd_condi_frame.setVisible)
        self.prog_clear_btn.clicked.connect(self.prog_textEdit.clear)
        self.pushButton.clicked.connect(self.log_textEdit.clear)

        self.tabWidget.setCurrentIndex(1)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
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
        self.label.setText(QCoreApplication.translate("MainWindow", u"Jog speed:", None))
        self.label_1.setText(QCoreApplication.translate("MainWindow", u"Joint 1", None))
        self.jog_j1_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j1_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j1_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Joint 2", None))
        self.jog_j2_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j2_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j2_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Joint 3", None))
        self.jog_j3_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j3_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j3_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Joint 4", None))
        self.jog_j4_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j4_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j4_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Joint 5", None))
        self.jog_j5_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j5_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j5_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.label_12.setText(QCoreApplication.translate("MainWindow", u"Joint 6", None))
        self.jog_j6_neg_btn.setText(QCoreApplication.translate("MainWindow", u"-", None))
        self.jog_j6_label.setText(QCoreApplication.translate("MainWindow", u"0", None))
        self.jog_j6_pos_btn.setText(QCoreApplication.translate("MainWindow", u"+", None))
        self.cmd_prog_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Programs", None))
        self.cmd_ping_btn.setText(QCoreApplication.translate("MainWindow", u"Ping", None))
        self.cmd_pong_btn.setText(QCoreApplication.translate("MainWindow", u"Pong", None))
        self.cmd_switches_btn.setText(QCoreApplication.translate("MainWindow", u"Test_Switches", None))
        self.cmd_home_axis_btn.setText(QCoreApplication.translate("MainWindow", u"Home", None))
        self.cmd_condi_menu_pushButton.setText(QCoreApplication.translate("MainWindow", u"Conditional", None))
        self.cmd_begin_pushButton.setText(QCoreApplication.translate("MainWindow", u"Begin", None))
        self.cmd_end_pushButton.setText(QCoreApplication.translate("MainWindow", u"End", None))
        self.prog_label.setText(QCoreApplication.translate("MainWindow", u"Program:", None))
        self.prog_clear_btn.setText(QCoreApplication.translate("MainWindow", u"Clear", None))
        self.prog_textEdit.setPlaceholderText("")
        self.log_label.setText(QCoreApplication.translate("MainWindow", u"Response log:", None))
        self.pushButton.setText(QCoreApplication.translate("MainWindow", u"Clear", None))
        self.btn_start.setText(QCoreApplication.translate("MainWindow", u"Start", None))
        self.btn_stop.setText(QCoreApplication.translate("MainWindow", u"Stop", None))
        self.btn_load_prog_btn.setText(QCoreApplication.translate("MainWindow", u"Load Program", None))
        self.select_device_label.setText(QCoreApplication.translate("MainWindow", u"Select Device:", None))
        self.con_connect_btn.setText(QCoreApplication.translate("MainWindow", u"Connect", None))
        self.con_status_label1.setText(QCoreApplication.translate("MainWindow", u"Status:", None))
        self.con_status_label2.setText(QCoreApplication.translate("MainWindow", u"Disconnected", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_control), QCoreApplication.translate("MainWindow", u"Control", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"DH Params:", None))
        self.label_15.setText(QCoreApplication.translate("MainWindow", u"3", None))
        self.label_18.setText(QCoreApplication.translate("MainWindow", u"5", None))
#if QT_CONFIG(tooltip)
        self.label_11.setToolTip(QCoreApplication.translate("MainWindow", u"Link length", None))
#endif // QT_CONFIG(tooltip)
        self.label_11.setText(QCoreApplication.translate("MainWindow", u"a", None))
        self.label_17.setText(QCoreApplication.translate("MainWindow", u"1", None))
#if QT_CONFIG(tooltip)
        self.label_9.setToolTip(QCoreApplication.translate("MainWindow", u"Link offset", None))
#endif // QT_CONFIG(tooltip)
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"d", None))
        self.label_14.setText(QCoreApplication.translate("MainWindow", u"2", None))
#if QT_CONFIG(tooltip)
        self.label_7.setToolTip(QCoreApplication.translate("MainWindow", u"Link twist (in radians)", None))
#endif // QT_CONFIG(tooltip)
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"alpha", None))
        self.label_16.setText(QCoreApplication.translate("MainWindow", u"4", None))
        self.label_19.setText(QCoreApplication.translate("MainWindow", u"6", None))
        self.joint_label.setText(QCoreApplication.translate("MainWindow", u"Joint", None))
#if QT_CONFIG(tooltip)
        self.label_53.setToolTip(QCoreApplication.translate("MainWindow", u"Theta offset (in degrees) default 0", None))
#endif // QT_CONFIG(tooltip)
        self.label_53.setText(QCoreApplication.translate("MainWindow", u"theta offset", None))
        self.dh_j1_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.dh_j2_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.dh_j3_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.dh_j4_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.dh_j5_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.dh_j6_theta_off.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.label_39.setText(QCoreApplication.translate("MainWindow", u"Homing Params:", None))
        self.label_44.setText(QCoreApplication.translate("MainWindow", u"away velocity", None))
        self.label_48.setText(QCoreApplication.translate("MainWindow", u"6", None))
        self.motor_label.setText(QCoreApplication.translate("MainWindow", u"Motor", None))
        self.label_41.setText(QCoreApplication.translate("MainWindow", u"5", None))
        self.label_49.setText(QCoreApplication.translate("MainWindow", u"home position", None))
        self.label_40.setText(QCoreApplication.translate("MainWindow", u"3", None))
#if QT_CONFIG(tooltip)
        self.label_46.setToolTip("")
#endif // QT_CONFIG(tooltip)
        self.label_46.setText(QCoreApplication.translate("MainWindow", u"velocity", None))
        self.label_42.setText(QCoreApplication.translate("MainWindow", u"back velocity", None))
        self.label_47.setText(QCoreApplication.translate("MainWindow", u"4", None))
        self.label_45.setText(QCoreApplication.translate("MainWindow", u"2", None))
        self.label_43.setText(QCoreApplication.translate("MainWindow", u"1", None))
        self.label_50.setText(QCoreApplication.translate("MainWindow", u"Speed and Acceleration:", None))
        self.label_52.setText(QCoreApplication.translate("MainWindow", u"5", None))
        self.label_57.setText(QCoreApplication.translate("MainWindow", u"max speed", None))
        self.label_55.setText(QCoreApplication.translate("MainWindow", u"acceleration", None))
        self.label_59.setText(QCoreApplication.translate("MainWindow", u"6", None))
        self.label_56.setText(QCoreApplication.translate("MainWindow", u"2", None))
        self.motor_label_2.setText(QCoreApplication.translate("MainWindow", u"Motor", None))
        self.label_51.setText(QCoreApplication.translate("MainWindow", u"3", None))
        self.label_54.setText(QCoreApplication.translate("MainWindow", u"1", None))
        self.label_58.setText(QCoreApplication.translate("MainWindow", u"4", None))
        self.setup_save_btn.setText(QCoreApplication.translate("MainWindow", u"Save ", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_setup), QCoreApplication.translate("MainWindow", u"Setup", None))
        pass
    # retranslateUi

