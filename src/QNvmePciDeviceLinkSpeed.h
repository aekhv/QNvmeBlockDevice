/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEPCIDEVICELINKSPEED_H
#define QNVMEPCIDEVICELINKSPEED_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmePciDeviceLinkSpeed
{
public:
    QNvmePciDeviceLinkSpeed();
    QNvmePciDeviceLinkSpeed(const nvme_pci_device_link_t &link);

    int speed() const { return m_speed; }
    int width() const { return m_width; }

    QString toString() const;

private:
    int m_speed, m_width;
};

#endif // QNVMEPCIDEVICELINKSPEED_H
