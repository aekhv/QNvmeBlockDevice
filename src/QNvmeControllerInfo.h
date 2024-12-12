/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMECONTROLLERINFO_H
#define QNVMECONTROLLERINFO_H

#include <QtCore>
#include "QNvmeCommon.h"

class QNvmeControllerInfo
{
public:
    QNvmeControllerInfo();
    QNvmeControllerInfo(const nvme_controller_info_t &info);

    QString modelName() const { return m_mdl; }
    QString firmwareRevision() const { return m_fw; }
    QString serialNumber() const { return m_sn; }
    int namespaceCount() const { return m_nsc; }
    int maxDataTransferSize() const { return m_mdts; }
    qint64 totalCapacity() const { return m_tcap; }
    qint64 unallocatedCapacity() const { return m_ucap; }

private:
    QString m_mdl;
    QString m_fw;
    QString m_sn;
    int m_nsc;
    int m_mdts;
    qint64 m_tcap, m_ucap;
};

#endif // QNVMECONTROLLERINFO_H
