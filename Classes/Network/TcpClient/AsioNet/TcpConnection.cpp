/****************************************************************************
 Copyright (c) 2013 snailgame.net
 Copyright (c) 2013 libo
 ****************************************************************************/

#include <iostream>
#include "TcpConnection.h"

namespace Framework{

TcpConnection::TcpConnection():
	m_socket(m_ioServer), 
	m_heartBeatTimer(m_ioServer),
	m_reconnectTimer(m_ioServer),
	m_delimiter("\0"),
	m_heartBeat("PING"),
	m_heartBeatTimeOut(5),
	m_reconnectTimeOut(5)
{	

}

TcpConnection::~TcpConnection(void)
{
	disconnect();
}

void TcpConnection::update()
{
	// calls the poll() function to process network messages
	m_ioServer.poll();
}

void TcpConnection::connect(const std::string &ip, unsigned short port)
{
	// connect socket
	try 
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
		connect(endpoint);
	}
	catch(const std::exception &e)
	{
		std::cout << "Server exception:" << e.what() << std::endl;
	}
}

void TcpConnection::connect(const std::string &url, const std::string &protocol)
{
	// On Windows, see './Windows/system32/drivers/etc/services' for a list of supported protocols.
	//  You can also explicitly pass a port, like "8080"
	boost::asio::ip::tcp::resolver::query query( url, protocol );
	boost::asio::ip::tcp::resolver resolver( m_ioServer );

	try 
	{
		boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);

		boost::asio::ip::tcp::endpoint endpoint;
		while ( destination != boost::asio::ip::tcp::resolver::iterator() ) 
		{
			endpoint = *destination++;
		}
		connect(endpoint);
	}
	catch(const std::exception &e)
	{
		std::cout << "Server exception:" << e.what() << std::endl;
	}
}

void TcpConnection::connect(boost::asio::ip::tcp::endpoint& endpoint)
{
	m_endPoint = endpoint;

	//std::cout << "Trying to connect to port " << endpoint.port() << " @ " << endpoint.address().to_string() << std::endl;

	// try to connect, then call handle_connect
	m_socket.async_connect(endpoint,
		boost::bind(&TcpConnection::handle_connect, this, boost::asio::placeholders::error));
}

void TcpConnection::disconnect()
{		
	// tell socket to close the connection
	close();
	// tell the IO service to stop
	m_ioServer.stop();
	DisconnectedCallback(m_endPoint); 
}

void TcpConnection::write(const std::string &msg)
{
	// safe way to request the client to write a message
	m_ioServer.post(boost::bind(&TcpConnection::do_write, shared_from_this(), msg));
}

void TcpConnection::close()
{
	// safe way to request the client to close the connection
	m_ioServer.post(boost::bind(&TcpConnection::do_close, shared_from_this()));
}

void TcpConnection::read()
{
	// wait for a message to arrive, then call handle_read
	boost::asio::async_read_until(m_socket, m_buffer, m_delimiter,
		boost::bind(&TcpConnection::handle_read, shared_from_this(), boost::asio::placeholders::error));

}

// callbacks
void TcpConnection::handle_connect(const boost::system::error_code& error) 
{
	if (!error) 
	{
		// let listeners know
		ConnectedCallback(m_endPoint);

		// start heartbeat timer (optional)	
		m_heartBeatTimer.expires_from_now(boost::posix_time::seconds(m_heartBeatTimeOut));
		m_heartBeatTimer.async_wait(boost::bind(&TcpConnection::do_heartbeat, shared_from_this(), boost::asio::placeholders::error));

		// await the first message
		read();
	}
	else
	{
		ExceptionCallback(error);

		// schedule a timer to reconnect after 5 seconds		
		m_reconnectTimer.expires_from_now(boost::posix_time::seconds(m_reconnectTimeOut));
		m_reconnectTimer.async_wait(boost::bind(&TcpConnection::do_reconnect, shared_from_this(), boost::asio::placeholders::error));
	}
}

void TcpConnection::handle_read(const boost::system::error_code& error)
{
	if (!error)
	{
		std::string msg;
		std::istream is(&m_buffer);
		std::getline(is, msg); 
		
		if(msg.empty()) return;

		// TODO: you could do some message processing here, like breaking it up
		//       into smaller parts, rejecting unknown messages or handling the message protocol

		// create function to notify listeners
		MessageCallback(msg);

		// restart heartbeat timer (optional)	
		m_heartBeatTimer.expires_from_now(boost::posix_time::seconds(m_heartBeatTimeOut));
		m_heartBeatTimer.async_wait(boost::bind(&TcpConnection::do_heartbeat, shared_from_this(), boost::asio::placeholders::error));

		// wait for the next message
		read();
	}
	else
	{
		ExceptionCallback(error);

		// try to reconnect if external host disconnects
		if(error.value() != 0) 
		{
			// let listeners know
			DisconnectedCallback(m_endPoint); 
			
			// schedule a timer to reconnect after 5 seconds
			m_reconnectTimer.expires_from_now(boost::posix_time::seconds(m_reconnectTimeOut));
			m_reconnectTimer.async_wait(boost::bind(&TcpConnection::do_reconnect, shared_from_this(), boost::asio::placeholders::error));
		}
		else
		{
			close();
		}	
	}
}

void TcpConnection::handle_write(const boost::system::error_code& error)
{
	if(!error)
	{
		// write next message
		m_messages.pop_front();
		if (!m_messages.empty())
		{
			boost::asio::async_write(m_socket,
				boost::asio::buffer(m_messages.front()),
				boost::bind(&TcpConnection::handle_write, this, boost::asio::placeholders::error));
		}
		else {
			// restart heartbeat timer (optional)	
			m_heartBeatTimer.expires_from_now(boost::posix_time::seconds(m_heartBeatTimeOut));
			m_heartBeatTimer.async_wait(boost::bind(&TcpConnection::do_heartbeat, shared_from_this(), boost::asio::placeholders::error));
		}
	}
	else if (error)
	{
		ExceptionCallback(error);
	}
}

void TcpConnection::do_write(const std::string &msg)
{
	bool write_in_progress = !m_messages.empty();
	m_messages.push_back(msg + m_delimiter);

	if (!write_in_progress)
	{
		boost::asio::async_write(m_socket,
			boost::asio::buffer(m_messages.front()),
			boost::bind(&TcpConnection::handle_write, shared_from_this(), boost::asio::placeholders::error));
	}
}

void TcpConnection::do_close()
{
	if (m_socket.is_open())
	{
		boost::system::error_code err;
		m_socket.shutdown(boost::asio::socket_base::shutdown_both, err);
		m_socket.close(err);
	}
	m_heartBeatTimer.cancel();
	m_reconnectTimer.cancel();
	CloseCallback(m_endPoint);
}

void TcpConnection::do_reconnect(const boost::system::error_code& error)
{
	if(error != boost::asio::error::operation_aborted)
	{
		// close current socket if necessary
		m_socket.close();

		// try to reconnect, then call handle_connect
		m_socket.async_connect(m_endPoint,
			boost::bind(&TcpConnection::handle_connect, shared_from_this(), boost::asio::placeholders::error));
	}
}

void TcpConnection::do_heartbeat(const boost::system::error_code& error)
{
	// here you can regularly send a message to the server to keep the connection alive,
	// I usualy send a PING and then the server replies with a PONG

	if(!error) write( m_heartBeat );
}

}//namespace snailgame

