/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEPCIDEVICELOCATION_H
#define QNVMEPCIDEVICELOCATION_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmePciDeviceLocation
{
public:
    QNvmePciDeviceLocation();
    QNvmePciDeviceLocation(const nvme_pci_device_location_t &location);

    quint8 domain() const { return m_domain; }
    quint8 bus() const { return m_bus; }
    quint8 slot() const { return m_slot; }
    quint8 function() const { return m_func; }

    QString toString() const;

private:
    quint8 m_domain, m_bus, m_slot, m_func;
};

#endif // QNVMEPCIDEVICELOCATION_H
