/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMECONTROLLERVERSION_H
#define QNVMECONTROLLERVERSION_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmeControllerVersion
{
public:
    QNvmeControllerVersion();
    QNvmeControllerVersion(const nvme_controller_version_t &version);
    QString toString() const;

    int major() const { return m_major; }
    int minor() const { return m_minor; }
    int tertiary() const { return m_tertiary; }

private:
    int m_major, m_minor, m_tertiary;
};

#endif // QNVMECONTROLLERVERSION_H
