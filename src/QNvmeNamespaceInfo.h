/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMENAMESPACEINFO_H
#define QNVMENAMESPACEINFO_H

#include <QtCore>
#include "QNvmeCommon.h"

class QNvmeNamespaceInfo
{
public:
    QNvmeNamespaceInfo();
    QNvmeNamespaceInfo(const nvme_namespace_info_t &info);

    quint64 totalSize() const { return m_totalSize; }
    quint64 capacity() const { return m_capacity; }
    quint64 utilization() const { return m_utilization; }
    quint16 sectorSize() const { return m_sectorSize; }

private:
    quint64 m_totalSize, m_capacity, m_utilization;
    quint16 m_sectorSize;
};

#endif // QNVMENAMESPACEINFO_H
