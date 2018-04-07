#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <memory>
#include <queue>
#include <windows.h>

namespace Utils {
	template <typename T>
	class ThreadSaveQueue {
	public:
		ThreadSaveQueue(std::atomic<int>& _workCount)
			: m_workCount(_workCount)
		{}

		void push(const T& value)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			this->q.push(value);
		}
		
		bool pop(T& _ret)
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			if (this->q.empty())
				return false;
			_ret = std::move(this->q.front());
			this->q.pop();

			++m_workCount;
			return true;
		}

		bool empty()
		{
			std::lock_guard<std::mutex> lock(this->mutex);
			return this->q.empty();
		}
	private:
		std::queue<T> q;
		std::mutex mutex;
		std::atomic<int>& m_workCount;
	};

	class ThreadPool
	{
	public:
		class Task : public std::function<void()>
		{
		public:
			template <typename... Args>
			Task(Args&&... _args) :
				std::function<void()>(std::forward<Args>(_args)...),
				isDone(false)
			{}

			std::atomic<bool> isDone;
		};

		typedef std::shared_ptr<Task> TaskHandle;

		ThreadPool(int _numThreads) :
			m_numWorking(0),
			m_shouldStop(false),
			m_queue(m_numWorking)
		{
			m_threads.reserve(_numThreads);
			m_flags.resize(_numThreads);

			for (int i = 0; i < _numThreads; ++i)
			{
				m_threads.emplace_back(NewWorker());
			}
		}

		/* restarts a thread
		* only use this if you know that its current task is still running
		*/
		void ResetThread(size_t ind)
		{
			std::thread t(NewWorker());

			t.swap(m_threads[ind]);
			auto hndl = t.native_handle();

			DWORD exit = 0;
			//	GetExitCodeThread(hndl, &exit);
			bool b = TerminateThread(hndl, exit);
			t.detach();

			auto err = GetLastError();
		//	assert(b && "Thread could not be terminated!");
		}

		~ThreadPool()
		{
			// tell threads to finish
			m_shouldStop = true;

			// wait for them to finish
			for (auto& thread : m_threads)
				if (thread.joinable()) thread.join();
		}

		void Stop()
		{
			m_shouldStop = true;
		}

		template<typename... _Args>
		TaskHandle Push(_Args&&... _args)
		{
			auto ptr = std::make_shared<Task>(std::forward<_Args>(_args)...);
			m_queue.push(ptr);

			return std::move(ptr);
		}

		// Waits for all tasks to be finished.
		// The main thread is used to complete remaining tasks.
		void WaitFinished()
		{
			// take some work instead of just waiting
			TaskHandle f;
			while (m_queue.pop(f))
			{
				(*f)();
				f->isDone = true;
				--m_numWorking;
			} 
			
			// let all other threads finish
			while (m_numWorking);
		}

		int GetNumWorking() const { return m_numWorking; }
	private:
		std::thread NewWorker()
		{
			auto runner = [&]()
			{
				//			std::atomic<bool>& flag = *m_flags[i].get();
				//wait for task
				while (!m_shouldStop)
				{
					TaskHandle f;
					if (m_queue.pop(f))
					{
					//	++m_numWorking;
						//		flag = true;
						(*f)();
						f->isDone = true;
						--m_numWorking;
						//		flag = false;
					}
				}
			};

			return std::thread(std::move(runner));
		}


		std::vector< std::thread > m_threads;
		std::vector < std::unique_ptr<std::atomic<bool>> > m_flags;
		std::atomic<bool> m_shouldStop;
		ThreadSaveQueue< TaskHandle > m_queue;

		std::atomic<int> m_numWorking;
	};

	extern ThreadPool g_threadPool; // defined in main.cpp
}