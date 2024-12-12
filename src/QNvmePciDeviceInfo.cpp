/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmePciDeviceInfo.h"

QNvmePciDeviceInfo::QNvmePciDeviceInfo()
{

}

QNvmePciDeviceInfo::QNvmePciDeviceInfo(const nvme_pci_device_info_t &info)
    : m_location(info.location),
      m_id(info.id),
      m_link(info.link)
{

}
