/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#include "QNvmeError.h"

QNvmeError::QNvmeError(NvmeErrorType type)
    : m_type(type),
      m_sct(0),
      m_sc(0),
      m_more(false),
      m_dnr(false)
{

}

QNvmeError::QNvmeError(int statusCodeType, int statusCode, bool more, bool doNotRetry)
    : m_type(DeviceError),
      m_sct(statusCodeType),
      m_sc(statusCode),
      m_more(more),
      m_dnr(doNotRetry)
{

}

QString QNvmeError::toString() const
{
    QString s = QNvmeError::typeToString(m_type);

    if (m_type == DeviceError) {
        QString m = QString("Code type: %1; Code: %2; More bit: %3; Do not retry bit: %4")
                .arg(QNvmeError::statusCodeTypeToString(m_sct))
                .arg(QNvmeError::statusCodeToString(m_sct, m_sc))
                .arg(QNvmeError::bitToString(m_more))
                .arg(QNvmeError::bitToString(m_dnr));
        s = s + "; " + m;
    }

    return s;
}

QString QNvmeError::typeToString(NvmeErrorType type)
{
    switch (type) {
    case NoError:
        return "No error";
    case DeviceOpeningError:
        return "Device opening error";
    case DeviceIoctlError:
        return "Device IOCTL error";
    case DeviceTimeout:
        return "Device timeout";
    case DeviceError:
        return "Device error";
    default:
        return "Unknown error";
    }
}

QString QNvmeError::statusCodeTypeToString(int type)
{
    switch (type) {
    case 0x00:
        return "Generic command status";
    case 0x01:
        return "Command specific status";
    case 0x02:
        return "Media specific or data integrity error";
    default:
        return QString("Unknown code type (0x%1)").arg(type, 2, 16, QChar('0'));
    }
}

QString QNvmeError::statusCodeToString(int type, int code)
{
    if (type == 0x00)
        switch (code) {
        case 0x00:
            return "Success";
        case 0x01:
            return "Invalid command opcode";
        case 0x02:
            return "Invalid field in command";
        case 0x03:
            return "Command ID conflict";
        case 0x04:
            return "Data transfer error";
        case 0x05:
            return "Commands aborted due to power loss notification";
        case 0x06:
            return "Internal device error";
        case 0x07:
            return "Command abort requested";
        case 0x08:
            return "Command aborted due to SQ deletion";
        case 0x09:
            return "Command aborted due to failed fused command";
        case 0x0a:
            return "Command aborted due to missing fused command";
        case 0x0b:
            return "Invalid namespace or format";
        case 0x0c:
            return "Command sequence error";
        case 0x80:
            return "LBA out of range";
        case 0x81:
            return "Capacity exceeded";
        case 0x82:
            return "Namespace not ready";
        default:
            return QString("Unknown code (0x%1)").arg(code, 2, 16, QChar('0'));
        }

    if (type == 0x01)
        switch (code) {
        case 0x00:
            return "Completion queue invalid";
        case 0x01:
            return "Invalid queue identifier";
        case 0x02:
            return "Maximum queue size exceeded";
        case 0x03:
            return "Abort command limit exceeded";
        case 0x05:
            return "Asynchronous event request limit exceeded";
        case 0x06:
            return "Invalid firmware slot";
        case 0x07:
            return "Invalid firmware image";
        case 0x08:
            return "Invalid interrupt vector";
        case 0x09:
            return "Invalid log page";
        case 0x0a:
            return "Invalid format";
        case 0x0b:
            return "Firmware application requires conventional reset";
        case 0x0c:
            return "Invalid queue deletion";
        case 0x80:
            return "Conflicting attributes";
        case 0x81:
            return "Invalid protection information";
        case 0x82:
            return "Attempted write to read only range";
        default:
            return QString("Unknown code (0x%1)").arg(code, 2, 16, QChar('0'));
        }

    if (type == 0x02)
        switch (code) {
        case 0x80:
            return "Write fault";
        case 0x81:
            return "Unrecovered read error";
        case 0x82:
            return "End-to-end guard check error";
        case 0x83:
            return "End-to-end application tag check error";
        case 0x84:
            return "End-to-end reference tag check error";
        case 0x85:
            return "Compare failure";
        case 0x86:
            return "Access denied";
        default:
            return QString("Unknown code (0x%1)").arg(code, 2, 16, QChar('0'));
        }

    return QString("Unknown code (0x%1)").arg(code, 2, 16, QChar('0'));
}

QString QNvmeError::bitToString(bool bit)
{
    return bit ? "True" : "False";
}
