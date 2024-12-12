/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeNamespaceInfo.h"

QNvmeNamespaceInfo::QNvmeNamespaceInfo()
    : m_totalSize(0),
      m_capacity(0),
      m_utilization(0),
      m_sectorSize(0)
{

}

QNvmeNamespaceInfo::QNvmeNamespaceInfo(const nvme_namespace_info_t &info)
{
    m_totalSize = info.nsze;
    m_capacity = info.ncap;
    m_utilization = info.nuse;

    const uint8_t flbas = info.flbas & 0x0f;
    const nvme_lba_format_t *lbaf = &(info.lbaf[flbas]);
    m_sectorSize = 1 << lbaf->lbads;
}
