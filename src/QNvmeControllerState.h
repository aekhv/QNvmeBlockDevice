/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMECONTROLLERSTATE_H
#define QNVMECONTROLLERSTATE_H

#include <QtCore>
#include "../../mininvme/ioctl.h"

class QNvmeControllerState
{
public:
    QNvmeControllerState();
    QNvmeControllerState(const nvme_controller_state_t &state);

    bool enabled() const { return m_enabled; }
    bool ready() const { return m_ready; }
    bool fatal() const { return m_fatal; }

private:
    bool m_enabled, m_ready, m_fatal;
};

#endif // QNVMECONTROLLERSTATE_H
