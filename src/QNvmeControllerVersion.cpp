/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeControllerVersion.h"

QNvmeControllerVersion::QNvmeControllerVersion()
    : m_major(0),
      m_minor(0),
      m_tertiary(0)
{

}

QNvmeControllerVersion::QNvmeControllerVersion(const nvme_controller_version_t &version)
{
    m_major = version.major;
    m_minor = version.minor;
    m_tertiary = version.tertiary;
}

QString QNvmeControllerVersion::toString() const
{
    return QString("%1.%2.%3")
            .arg(m_major)
            .arg(m_minor)
            .arg(m_tertiary);
}
