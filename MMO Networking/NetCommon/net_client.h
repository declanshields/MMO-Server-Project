#pragma once
#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace Engine
{
	namespace Network
	{
		template <typename T>
		class client_interface
		{
		public:
			client_interface() : m_socket(m_context)
			{
				//Initialise the socket with the io context
			}

			virtual ~client_interface()
			{
				//If the client is destroyed, try and disconnect from the server
				Disconnect();
			}

		public:
			//Connect to server with hostname/ip-address and port
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					//Create connection
					m_connection = std::make_unique<connection<T>>();

					//Resolve hostname/ip-address into tangible physical address
					asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					//Tell the connection object to connect to server
					m_connection->ConnectToServer(m_endpoints);

					//Start context thread
					thrContext = std::thread([this]() { m_context.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}

				return false;
			}

			//Disconnect from server
			void Disconnect()
			{
				//If Connection exists
				if (IsConnected())
				{
					//disconnect from server
					m_connection->Disconnect();
				}

				//We're done with the asio context
				m_context.stop();
				if (thrContext.joinable())
					thrContext.join();

				//Destroy connection object
				m_connection.release();
			}

			//Check if the client is actually connected to a server
			bool IsConnected()
			{
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}

			//Retrieve queue of messages from server
			tsqueue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}

		protected:
			//Asio context handles the data transfer
			asio::io_context m_context;
			//Asio needs a thread of its own to execute its work commands
			std::thread thrContext;
			//This is the hardware socket that is connected to the server
			asio::ip::tcp::socket m_socket;
			//The client has a single instance of a connection object, which handles data transfer
			std::unique_ptr<connection<T>> m_connection;

		private:
			//This is the threadsafe queue of the incoming messages from server
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}
