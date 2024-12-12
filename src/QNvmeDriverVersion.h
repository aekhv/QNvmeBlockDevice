/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEDRIVERVERSION_H
#define QNVMEDRIVERVERSION_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmeDriverVersion
{
public:
    QNvmeDriverVersion();
    QNvmeDriverVersion(const nvme_driver_version_t &version);
    QString toString() const;

    int major() const { return m_major; }
    int minor() const { return m_minor; }
    int patch() const { return m_patch; }

private:
    int m_major, m_minor, m_patch;
};

#endif // QNVMEDRIVERVERSION_H
