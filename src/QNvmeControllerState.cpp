/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeControllerState.h"

QNvmeControllerState::QNvmeControllerState()
    : m_enabled(false),
      m_ready(false),
      m_fatal(false)
{

}

QNvmeControllerState::QNvmeControllerState(const nvme_controller_state_t &state)
{
    m_enabled = state.enabled;
    m_ready = state.ready;
    m_fatal = state.fatal;
}
