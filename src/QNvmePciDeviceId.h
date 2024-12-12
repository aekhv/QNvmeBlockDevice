/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEPCIDEVICEID_H
#define QNVMEPCIDEVICEID_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmePciDeviceId
{
public:
    QNvmePciDeviceId();
    QNvmePciDeviceId(const nvme_pci_device_id_t &id);

    quint16 vendorId() const { return m_vendorId; }
    quint16 deviceId() const { return m_deviceId; }
    quint16 classId() const { return m_classId; }
    quint8 revision() const { return m_revision; }

    QString toString() const;

private:
    quint16 m_vendorId, m_deviceId, m_classId;
    quint8 m_revision;
};

#endif // QNVMEPCIDEVICEID_H
