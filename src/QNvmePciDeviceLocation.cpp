/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmePciDeviceLocation.h"

QNvmePciDeviceLocation::QNvmePciDeviceLocation()
    : m_domain(0),
      m_bus(0),
      m_slot(0),
      m_func(0)
{

}

QNvmePciDeviceLocation::QNvmePciDeviceLocation(const nvme_pci_device_location_t &location)
{
    m_domain = location.domain;
    m_bus = location.bus;
    m_slot = location.slot;
    m_func = location.func;
}

QString QNvmePciDeviceLocation::toString() const
{
    return QString("%1:%2:%3.%4")
            .arg(m_domain, 4, 16, QChar('0'))
            .arg(m_bus, 2, 16, QChar('0'))
            .arg(m_slot, 2, 16, QChar('0'))
            .arg(m_func, 1, 16, QChar('0'));
}
