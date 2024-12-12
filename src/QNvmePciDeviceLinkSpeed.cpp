/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmePciDeviceLinkSpeed.h"

QNvmePciDeviceLinkSpeed::QNvmePciDeviceLinkSpeed()
    : m_speed(0),
      m_width(0)
{

}

QNvmePciDeviceLinkSpeed::QNvmePciDeviceLinkSpeed(const nvme_pci_device_link_t &link)
{
    m_speed = link.speed;
    m_width = link.width;
}

QString QNvmePciDeviceLinkSpeed::toString() const
{
    return QString("Gen.%1x%2")
            .arg(m_speed)
            .arg(m_width);
}
