/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeDriverVersion.h"

QNvmeDriverVersion::QNvmeDriverVersion()
    : m_major(0),
      m_minor(0),
      m_patch(0)
{

}

QNvmeDriverVersion::QNvmeDriverVersion(const nvme_driver_version_t &version)
{
    m_major = version.major;
    m_minor = version.minor;
    m_patch = version.patch;
}

QString QNvmeDriverVersion::toString() const
{
    return QString("%1.%2.%3")
            .arg(m_major)
            .arg(m_minor)
            .arg(m_patch);
}
