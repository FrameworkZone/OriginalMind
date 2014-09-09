/****************************************************************************
 Copyright (c) 2014-2015 libo.
 
 losemymind.libo@gmail.com

****************************************************************************/

// TODO: use implicitly shared object
// defines the value of _WIN32_WINNT needed by boost asio (WINDOWS ONLY)
#ifndef Framework_TcpConnection_h
#define Framework_TcpConnection_h

#ifdef WIN32
	#include <sdkddkver.h>
#endif

#include <string>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>

namespace Framework{

class TcpConnection : public boost::enable_shared_from_this<TcpConnection> ,private boost::noncopyable
{
public:
	typedef boost::shared_ptr<TcpConnection> pointer;
	typedef boost::weak_ptr  <TcpConnection> weak_pointer;

	TcpConnection();
	~TcpConnection(void);
	
	/**
	 * @brief Call this function to process network messages.
	 */
	void update();

	/**
	 * @brief Send message to server
	 * @param[in] msg Want to send a message.
	 */
	void write(const std::string &msg);

	/**
	 * @brief Connect to server
	 * @param[in] ip Server ip address like "192.168.10.26".
	 * @param[in] port Server listening port for example 8080.
	 */
	void connect(const std::string &ip, unsigned short port);

	/**
	 * @brief Connect to server
	 * @param[in] url Server url like "www.google.com".
	 * @param[in] protocol Connection protocol like "http".
	 */
	void connect(const std::string &url, const std::string &protocol);

	/**
	 * @brief Connect to server
	 * @param[in] endpoint .
	 */
	void connect(boost::asio::ip::tcp::endpoint& endpoint);

	/**
	 * @brief Close network connection, you can call connect function
	 *        to reconnect.
	 */
	void disconnect();

	/**
	 * @brief Check whether the network connection.
	 */
	bool isConnected(){ return m_socket.is_open(); };
	
	/**
	 * @brief Get message end marker.
	 * @return  The message end marker string. 
	 */
	std::string getDelimiter(){ return m_delimiter; };

	/**
	 * @brief Set message end marker.
	 * @param[in] delimiter The message end marker string. 
	 */
	void setDelimiter(const std::string &delimiter){ m_delimiter = delimiter; };

	/**
	 * @brief Get message end marker.
	 * @return  The message end marker string. 
	 */
	std::string getHeartBeat(){ return m_heartBeat; };

	/**
	 * @brief Set message end marker.
	 * @param[in] delimiter The message end marker string. 
	 */
	void setHeartBeat(const std::string& heartBeat){ m_heartBeat = heartBeat; };

	/**
	 * @brief Set heart beat time out.
	 * @param[out] timeOut Time value (second).
	 */
	void setHeartBeatTimeOut(size_t timeOut){m_heartBeatTimeOut = timeOut; };

	/**
	 * @brief Set reconnect time out.
	 * @param[in] timeOut Time value (second).
	 */
	void setreconnectTimeOut(size_t timeOut){m_reconnectTimeOut = timeOut; };
public:

	/**
	 * @brief Callback when the network connected, Disconnected and have message.
	 */
	boost::function<void(const boost::asio::ip::tcp::endpoint&)>	ConnectedCallback;
	boost::function<void(const boost::asio::ip::tcp::endpoint&)>	DisconnectedCallback;
	boost::function<void(const boost::asio::ip::tcp::endpoint&)>    CloseCallback;
	boost::function<void(const std::string&)>				   	    MessageCallback;
	boost::function<void(const boost::system::error_code&)>         ExceptionCallback;

protected:
	virtual void read();
	virtual void close();

	// callbacks
	virtual void handle_connect(const boost::system::error_code& error);
	virtual void handle_read(const boost::system::error_code& error);
	virtual void handle_write(const boost::system::error_code& error);

	virtual void do_write(const std::string &msg);
	virtual void do_close();
	virtual void do_reconnect(const boost::system::error_code& error);
	virtual void do_heartbeat(const boost::system::error_code& error);
protected:
	boost::asio::ip::tcp::endpoint	m_endPoint;

	boost::asio::io_service			m_ioServer;
	boost::asio::ip::tcp::socket	m_socket;

	boost::asio::streambuf			m_buffer;

	//! to be written to server
	std::deque<std::string>			m_messages;	

	boost::asio::deadline_timer		m_heartBeatTimer;
	boost::asio::deadline_timer		m_reconnectTimer;

	std::string						m_delimiter;
	std::string						m_heartBeat;

	size_t                          m_heartBeatTimeOut;
	size_t                          m_reconnectTimeOut;
};

}//namespace Framework

#endif // Framework_TcpConnection_h


