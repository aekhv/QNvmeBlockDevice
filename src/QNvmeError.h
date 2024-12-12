/****************************************************************************
**
** This file is part of the QNvmeBlockDevice library.
** Copyright (C) 2024 Alexander E. <aekhv@vk.com>
** License: GNU LGPL v2, see file LICENSE.
**
****************************************************************************/

#ifndef QNVMEERROR_H
#define QNVMEERROR_H

#include <QtCore>

class QNvmeError
{
public:
    enum NvmeErrorType {
        NoError,
        DeviceOpeningError, // device opening error
        DeviceIoctlError,   // ioctl error
        DeviceTimeout,      // a command sent, but no response
        DeviceError,        // a command sent, but error received
        UnknownError
    };
    QNvmeError(NvmeErrorType type = NoError);
    QNvmeError(int statusCodeType, int statusCode, bool more, bool doNotRetry);

    bool success() const { return m_type == NoError; }
    NvmeErrorType type() const { return m_type; }
    int deviceStatusCodeType() const { return m_sct; }
    int deviceStatusCode() const { return m_sc; }
    bool deviceMoreBit() const { return m_more; }
    bool deviceDoNotRetryBit() const { return m_dnr; }

    QString toString() const;

    static QString typeToString(NvmeErrorType type);
    static QString statusCodeTypeToString(int type);
    static QString statusCodeToString(int type, int code);
    static QString bitToString(bool bit);

private:
    NvmeErrorType m_type;
    int m_sct;
    int m_sc;
    bool m_more;
    bool m_dnr;
};

#endif // QNVMEERROR_H
