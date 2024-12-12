/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeControllerInfo.h"

QNvmeControllerInfo::QNvmeControllerInfo()
    : m_nsc(0),
      m_mdts(0)
{

}

QNvmeControllerInfo::QNvmeControllerInfo(const nvme_controller_info_t &info)
{
    m_mdl = QString::fromLatin1(info.mn, sizeof(info.mn)).trimmed();
    m_fw = QString::fromLatin1(info.fr, sizeof(info.fr)).trimmed();
    m_sn = QString::fromLatin1(info.sn, sizeof(info.sn)).trimmed();
    m_nsc = info.nn;
    m_mdts = 4096 * (1 << info.mdts);
    m_tcap = info.tnvmcap_lo;
    m_ucap = info.unvmcap_lo;
}
