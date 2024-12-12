QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QNvmeBlockDevice.cpp \
    QNvmeControllerInfo.cpp \
    QNvmeControllerState.cpp \
    QNvmeControllerVersion.cpp \
    QNvmeDriverVersion.cpp \
    QNvmeError.cpp \
    QNvmeLogPageHealthInfo.cpp \
    QNvmeNamespaceInfo.cpp \
    QNvmePciDeviceId.cpp \
    QNvmePciDeviceInfo.cpp \
    QNvmePciDeviceLinkSpeed.cpp \
    QNvmePciDeviceLocation.cpp

HEADERS += \
    QNvmeBlockDevice.h \
    QNvmeCommon.h \
    QNvmeControllerInfo.h \
    QNvmeControllerState.h \
    QNvmeControllerVersion.h \
    QNvmeDriverVersion.h \
    QNvmeError.h \
    QNvmeLogPageHealthInfo.h \
    QNvmeNamespaceInfo.h \
    QNvmePciDeviceId.h \
    QNvmePciDeviceInfo.h \
    QNvmePciDeviceLinkSpeed.h \
    QNvmePciDeviceLocation.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
