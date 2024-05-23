#pragma once
#include "net_common.h"

//https://www.youtube.com/watch?v=2hNdkYInj4g

namespace Engine
{
	namespace Network
	{
		template<typename T>
		class tsqueue
		{
		public:

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
		};
	}
}