/*
 * Copyright (C) 2015 ThomArmax (Thomas COIN)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Thomas COIN <esvcorp@gmail.com> 18/04/2015
 */

#include "ATCommandResponse"
#include <QDebug>

namespace QtXBee {

ATCommandResponse::ATCommandResponse(QObject *parent) :
    XBeeResponse(parent),
    m_atCommand(ATCommand::ATUndefined),
    m_status(Error)
{
    setFrameType(ATCommandResponseId);
}

ATCommandResponse::ATCommandResponse(const QByteArray &packet, QObject *parent)  :
    XBeeResponse(parent),
    m_atCommand(ATCommand::ATUndefined),
    m_status(Error)
{
    setFrameType(ATCommandResponseId);
    setPacket(packet);
}

bool ATCommandResponse::parseApiSpecificData(const QByteArray &data)
{
    QByteArray at;
    int i;
    if(data.size() < 3) {
        qDebug() << Q_FUNC_INFO << "bad packet";
        return false;
    }
    setFrameId(data.at(0));
    at.append(data.at(1));
    at.append(data.at(2));
    setATCommand(at);
    setStatus((Status)(unsigned char)data.at(3));
    for(i=4; i<data.size();i++) {
        m_data.append(data.at(i));
    }

    return true;
}

/**
 * @brief Sets the response's AT Command
 * @param at
 * @sa ATCommand::ATCommandType
 */
void ATCommandResponse::setATCommand(ATCommand::ATCommandType at)
{
    m_atCommand = at;
}

/**
 * @brief Sets the response's AT Command
 * @param at
 * @sa ATCommandResponse::setATCommand(ATCommand::ATCommandType at)
 */
void ATCommandResponse::setATCommand(const QByteArray &at)
{
    if(at.size() != 2) {
        qWarning() << Q_FUNC_INFO << "invalid at command" << at;
        return;
    }

    m_atCommand = ATCommand::atCommandFromByteArray(at);
}

/**
 * @brief Returns the response's AT Command
 * @return the response's AT Command
 * @sa ATCommandResponse::setATCommand(ATCommand::ATCommandType at)
 * @sa ATCommand::ATCommandType
 */
ATCommand::ATCommandType ATCommandResponse::atCommand() const
{
    return m_atCommand;
}

/**
 * @brief Sets the command status
 * @param status
 * @see ATCommandResponse::CommandStatus
 */
void ATCommandResponse::setStatus(const Status status)
{
    m_status = status;
}

/**
 * @brief Returns the command' status
 * @return the command' status
 * @see ATCommandResponse::CommandStatus
 */
ATCommandResponse::Status ATCommandResponse::status() const
{
    return m_status;
}

/**
 * @brief Returns the ATCommandResponse::Status as a human readable string
 * @return the given ATCommandResponse::Status as a human readable string
 * @see ATCommandResponse::Status
 */
QString ATCommandResponse::statusToString() const
{
    return statusToString(status());
}

/**
 * @brief Returns the given ATCommandResponse::Status as a human readable string
 * @param status
 * @return the given ATCommandResponse::Status as a human readable string
 * @see ATCommandResponse::Status
 */
QString ATCommandResponse::statusToString(const ATCommandResponse::Status status)
{
    QString st = "Unknown";
    switch(status) {
    case Ok                 : st = "OK"                 ; break;
    case Error              : st = "Error"              ; break;
    case InvalidCommand     : st = "Invalid Command"    ; break;
    case InvalidParameter   : st = "Invalid Parameter"  ; break;
    case TxFailure          : st = "Tx Failure"         ; break;
    }

    return st;
}

QString ATCommandResponse::toString()
{
    QString str;
    str.append(QString("Raw packet      : 0x%1\n").arg(QString(packet().toHex())));
    str.append(QString("Frame id        : %1 (0x%2)\n").arg(frameId(), 0, 16).arg(frameId(), 0, 16));
    str.append(QString("Frame type      : %1 (0x%2)\n").arg(frameTypeToString(frameType())).arg(QString::number(frameType(), 16)));
    str.append(QString("AT command      : %1 (0x%2)\n").arg(ATCommand::atCommandToString(m_atCommand)).arg(QString::number(m_atCommand, 16)));
    str.append(QString("Start delimiter : 0x%1\n").arg(QString::number(startDelimiter(), 16)));
    str.append(QString("Length          : %1 bytes\n").arg(length()));
    if(!m_data.isEmpty())
    str.append(QString("Data            : 0x%1\n").arg(QString(m_data.toHex())));
    else
    str.append(QString("Data            : No data\n"));
    str.append(QString("Checksum        : %1\n").arg(checksum()));
    str.append(QString("Command Status  : %1 (0x%2)").arg(statusToString()).arg(m_status,0,16));

    return str;
}

void ATCommandResponse::clear()
{
    XBeeResponse::clear();
    setATCommand(ATCommand::ATUndefined);
    setStatus(Error);
}

} // END namepsace
