#include "remoteatcommandrequest.h"
#include <QDebug>

namespace QtXBee {

/**
 * @brief RemoteATCommandRequest's constructor
 * @param parent parent object
 */
RemoteATCommandRequest::RemoteATCommandRequest(QObject *parent) :
    XBeePacket(parent),
    m_destinationAddress(0),
    m_networkAddress(0),
    m_options(0),
    m_atCommand(ATCommand::Command_Undefined),
    m_commandParameter(0)
{
    setFrameType(XBeePacket::RemoteATCommandRequestFrame);
}
// Reimplemented from XBeePacket
void RemoteATCommandRequest::assemblePacket() {
    int i;

    m_packet.clear();
    m_packet.append(frameType());
    m_packet.append(frameId());
    for(i=7; i>=0; i--) {
        m_packet.append(destinationAddress() >> (i*8)&0xFF);
    }
    for(i=1; i>=0; i--) {
        m_packet.append(networkAddress() >> (i*8)&0xFF);
    }
    m_packet.append(commandOptions());
    for(i=1; i>=0; i--) {
        m_packet.append(atCommand() >> (i*8)&0xFF);
    }
    setLength(m_packet.size());
    createChecksum(m_packet);
    m_packet.append(checksum());
    m_packet.insert(0, startDelimiter());
    m_packet.insert(1, (length()>>8)&0xFF);
    m_packet.insert(2, length()&0xFF);
    qDebug() << Q_FUNC_INFO << m_packet.toHex();
}

void RemoteATCommandRequest::clear() {
    XBeePacket::clear();
    m_destinationAddress    = 0;
    m_networkAddress        = 0;
    m_options               = 0;
    m_atCommand             = ATCommand::Command_Undefined;
    m_commandParameter      = 0;
}

QString RemoteATCommandRequest::toString() {
    return QString();
}

// Setters
/**
 * @brief Sets the address of the destination device
 *
 * The following addresses are also supported:
 * - 0x0000000000000000 - Reserved 64-bit address for the coordinator
 * - 0x000000000000FFFF - Broadcast address
 * @param dest
 */
void RemoteATCommandRequest::setDestinationAddress(const quint64 dest) {
    m_destinationAddress = dest;
}

/**
 * @brief Sets the 16-bit address of the destination device, if known.
 *
 * Set to 0xFFFE if the address is unknown, or if sending a broadcast.
 * @param network
 */
void RemoteATCommandRequest::setNetworkAddress(const quint32 network) {
    m_networkAddress = network;
}

/**
 * @brief Sets the remote command options.
 */
void RemoteATCommandRequest::setCommandOptions(const RemoteCommandOptions options) {
    m_options = options;
}

/**
 * @brief Sets the command to be performed on the remote device
 * @param command
 */
void RemoteATCommandRequest::setATCommand(const ATCommand::ATCommandType command) {
    m_atCommand = command;
}

/**
 * @brief Sets the command's parameter.
 *
 * If present, indicates the requested parameter value to set the given register.
 * If no characters present, the register is queried.
 * @param parameter
 */
void RemoteATCommandRequest::setCommandParameter(const quint8 parameter) {
   m_commandParameter = parameter;
}

// Getters
quint64 RemoteATCommandRequest::destinationAddress() const {
    return m_destinationAddress;
}

quint16 RemoteATCommandRequest::networkAddress() const {
    return m_networkAddress;
}

ATCommand::ATCommandType RemoteATCommandRequest::atCommand() const {
    return m_atCommand;
}

quint8 RemoteATCommandRequest::commandParameter() {
    return m_commandParameter;
}

RemoteATCommandRequest::RemoteCommandOptions RemoteATCommandRequest::commandOptions() const {
    return m_options;
}

} // END namepsace