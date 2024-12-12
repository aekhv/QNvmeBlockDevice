/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmePciDeviceId.h"

QNvmePciDeviceId::QNvmePciDeviceId()
    : m_vendorId(0),
      m_deviceId(0),
      m_classId(0),
      m_revision(0)
{

}

QNvmePciDeviceId::QNvmePciDeviceId(const nvme_pci_device_id_t &id)
{
    m_vendorId = id.vendorId;
    m_deviceId = id.deviceId;
    m_classId = id.classId;
    m_revision = id.revision;
}

QString QNvmePciDeviceId::toString() const
{
    return QString("%1:%2:%3:%4")
            .arg(m_vendorId, 4, 16, QChar('0'))
            .arg(m_deviceId, 4, 16, QChar('0'))
            .arg(m_classId, 4, 16, QChar('0'))
            .arg(m_revision, 2, 16, QChar('0'));
}
