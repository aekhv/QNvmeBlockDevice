/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEBLOCKDEVICE_H
#define QNVMEBLOCKDEVICE_H

#include <QObject>
#include "QNvmeError.h"
#include "QNvmeDriverVersion.h"
#include "QNvmePciDeviceInfo.h"
#include "QNvmeControllerVersion.h"
#include "QNvmeControllerState.h"
#include "QNvmeControllerInfo.h"
#include "QNvmeNamespaceInfo.h"
#include "QNvmeLogPageHealthInfo.h"

class QNvmeBlockDevice : public QObject
{
    Q_OBJECT
public:
    explicit QNvmeBlockDevice(QObject *parent = nullptr);
    ~QNvmeBlockDevice();
    bool open(const QString &dev);
    bool isOpened() const { return m_fd >= 0; }
    void close();
    QNvmeError lastError() const { return m_error; }
    QNvmeDriverVersion driverVersion();
    QNvmePciDeviceInfo pciDeviceInfo();
    QNvmeControllerVersion controllerVersion();
    QNvmeControllerState controllerState();
    QNvmeControllerInfo controllerInfo();
    QNvmeNamespaceInfo namespaceInfo(int nsid = 1);
    //QNvmeLogPageErrorInfo logPageErrorInfo(); /* a reserve for the future */
    QNvmeLogPageHealthInfo logPageHealthInfo();
    bool read(quint64 offset, quint32 count, char *buffer, int length, int nsid = 1);
    bool write(quint64 offset, quint32 count, char *buffer, int length, int nsid = 1);
    bool controllerReset();

    static QStringList availableDevices();

signals:
    void deviceOpened();
    void deviceClosed();

private:
    int m_fd;
    QNvmeError m_error;
};

#endif // QNVMEBLOCKDEVICE_H
