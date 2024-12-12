/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEPCIDEVICEINFO_H
#define QNVMEPCIDEVICEINFO_H

#include "QNvmePciDeviceLocation.h"
#include "QNvmePciDeviceId.h"
#include "QNvmePciDeviceLinkSpeed.h"

class QNvmePciDeviceInfo
{
public:
    QNvmePciDeviceInfo();
    QNvmePciDeviceInfo(const nvme_pci_device_info_t &info);

    QNvmePciDeviceLocation location() const { return m_location; }
    QNvmePciDeviceId id() const { return m_id; }
    QNvmePciDeviceLinkSpeed link() const { return m_link; }

private:
    QNvmePciDeviceLocation m_location;
    QNvmePciDeviceId m_id;
    QNvmePciDeviceLinkSpeed m_link;
};

#endif // QNVMEPCIDEVICEINFO_H
