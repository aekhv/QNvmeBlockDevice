/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeBlockDevice.h"
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <sys/ioctl.h>  // ioctl()
#include "../../mininvme/ioctl.h"
#include "QNvmeCommon.h"

QNvmeBlockDevice::QNvmeBlockDevice(QObject *parent)
    : QObject(parent),
      m_fd(-1)
{

}

QNvmeBlockDevice::~QNvmeBlockDevice()
{
    close();
}

bool QNvmeBlockDevice::open(const QString &dev)
{
    close();
    m_fd = ::open(dev.toLocal8Bit().data(), O_RDWR);
    if (m_fd >= 0) {
        m_error = QNvmeError(QNvmeError::NoError);
        emit deviceOpened();
    } else
        m_error = QNvmeError(QNvmeError::DeviceOpeningError);
    return m_error.success();
}

void QNvmeBlockDevice::close()
{
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
        emit deviceClosed();
    }
}

QNvmeDriverVersion QNvmeBlockDevice::driverVersion()
{
    nvme_driver_version_t version;
    memset(&version, 0, sizeof(version));
    int err = ioctl(m_fd, NVME_IOCTL_GET_DRIVER_VERSION, &version);
    if (err < 0)
        m_error = QNvmeError(QNvmeError::DeviceIoctlError);
    else
        m_error = QNvmeError(QNvmeError::NoError);
    return QNvmeDriverVersion(version);
}

QNvmePciDeviceInfo QNvmeBlockDevice::pciDeviceInfo()
{
    nvme_pci_device_info_t info;
    memset(&info, 0, sizeof(info));
    int err = ioctl(m_fd, NVME_IOCTL_GET_PCI_DEVICE_INFO, &info);
    if (err < 0)
        m_error = QNvmeError(QNvmeError::DeviceIoctlError);
    else
        m_error = QNvmeError(QNvmeError::NoError);
    return QNvmePciDeviceInfo(info);
}

QNvmeControllerVersion QNvmeBlockDevice::controllerVersion()
{
    nvme_controller_version_t version;
    memset(&version, 0, sizeof(version));
    int err = ioctl(m_fd, NVME_IOCTL_GET_CONTROLLER_VERSION, &version);
    if (err < 0)
        m_error = QNvmeError(QNvmeError::DeviceIoctlError);
    else
        m_error = QNvmeError(QNvmeError::NoError);
    return QNvmeControllerVersion(version);
}

QNvmeControllerState QNvmeBlockDevice::controllerState()
{
    nvme_controller_state_t state;
    memset(&state, 0, sizeof(state));
    int err = ioctl(m_fd, NVME_IOCTL_GET_CONTROLLER_STATE, &state);
    if (err < 0)
        m_error = QNvmeError(QNvmeError::DeviceIoctlError);
    else
        m_error = QNvmeError(QNvmeError::NoError);
    return QNvmeControllerState(state);
}

static QNvmeError getError(int err, nvme_status_t *status)
{
    if (err < 0) {
        // IOCTL error
        return QNvmeError(QNvmeError::DeviceIoctlError);
    } else {
        if (status->timeout) {
            // Timeout
            return QNvmeError(QNvmeError::DeviceTimeout);
        } else {
            if (status->sct || status->sc)
                // Device error
                return QNvmeError(status->sct, status->sc, status->more, status->dnr);
            else
                // Success
                return QNvmeError(QNvmeError::NoError);
        }
    }
}

QNvmeControllerInfo QNvmeBlockDevice::controllerInfo()
{
    nvme_controller_info_t info;
    memset(&info, 0, sizeof(info));

    nvme_command_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd.opc = NVME_ADMIN_IDENTIFY;
    packet.cmd.cdw10 = NVME_ADMIN_IDENTIFY_CONTROLLER;
    packet.buffer.pointer = (uint8_t *)&info;
    packet.buffer.length = sizeof(info);

    int err = ioctl(m_fd, NVME_IOCTL_RUN_ADMIN_COMMAND, &packet);
    m_error = getError(err, &packet.status);
    return QNvmeControllerInfo(info);
}

QNvmeNamespaceInfo QNvmeBlockDevice::namespaceInfo(int nsid)
{
    nvme_namespace_info_t info;
    memset(&info, 0, sizeof(info));

    nvme_command_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd.nsid = nsid;
    packet.cmd.opc = NVME_ADMIN_IDENTIFY;
    packet.cmd.cdw10 = NVME_ADMIN_IDENTIFY_NAMESPACE;
    packet.buffer.pointer = (uint8_t *)&info;
    packet.buffer.length = sizeof(info);

    int err = ioctl(m_fd, NVME_IOCTL_RUN_ADMIN_COMMAND, &packet);
    m_error = getError(err, &packet.status);
    return QNvmeNamespaceInfo(info);
}

QNvmeLogPageHealthInfo QNvmeBlockDevice::logPageHealthInfo()
{
    nvme_log_page_health_information_t info;
    memset(&info, 0, sizeof(info));

    nvme_command_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.cmd.nsid = -1;
    packet.cmd.opc = NVME_ADMIN_GET_LOG_PAGE;

    nvme_log_page_cdw10_t *cdw10 = (nvme_log_page_cdw10_t*)&packet.cmd.cdw10;
    cdw10->lid = NVME_ADMIN_GET_LOG_PAGE_HEALTH_INFORMATION;
    cdw10->numd = sizeof (info) / sizeof (uint32_t);

    packet.buffer.pointer = (uint8_t *)&info;
    packet.buffer.length = sizeof(info);

    int err = ioctl(m_fd, NVME_IOCTL_RUN_ADMIN_COMMAND, &packet);
    m_error = getError(err, &packet.status);

    return QNvmeLogPageHealthInfo(info);
}

bool QNvmeBlockDevice::read(quint64 offset, quint32 count, char *buffer, int length, int nsid)
{
    nvme_lba_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.nsid = nsid;
    packet.lba.offset = offset;
    packet.lba.count = count;
    packet.buffer.pointer = (uint8_t *)buffer;
    packet.buffer.length = length;

    int err = ioctl(m_fd, NVME_IOCTL_READ_SECTORS, &packet);
    m_error = getError(err, &packet.status);
    return m_error.success();
}

bool QNvmeBlockDevice::write(quint64 offset, quint32 count, char *buffer, int length, int nsid)
{
    nvme_lba_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.nsid = nsid;
    packet.lba.offset = offset;
    packet.lba.count = count;
    packet.buffer.pointer = (uint8_t *)buffer;
    packet.buffer.length = length;

    int err = ioctl(m_fd, NVME_IOCTL_WRITE_SECTORS, &packet);
    m_error = getError(err, &packet.status);
    return m_error.success();
}

bool QNvmeBlockDevice::controllerReset()
{
    int err = ioctl(m_fd, NVME_IOCTL_CONTROLLER_RESET);
    if (err < 0)
        m_error = QNvmeError(QNvmeError::DeviceIoctlError);
    else
        m_error = QNvmeError(QNvmeError::NoError);
    return m_error.success();
}

QStringList QNvmeBlockDevice::availableDevices()
{
    QStringList list;
    QDir dev("/dev");
    dev.setNameFilters(QStringList() << "mininvme*");
    foreach (const QString item, dev.entryList(QDir::System))
        list.append("/dev/" + item);
    return list;
}
