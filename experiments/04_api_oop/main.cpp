
#include <string>
#include <vector>
#include <iostream>

class MessageBase
{
public:
	virtual ~MessageBase() = default;
};

class Message1 : public MessageBase
{
public:
	Message1(std::string msg)
		: m_message{ msg }
	{}

	std::string message() const
	{
		return m_message;
	}

	std::string m_message;
};

class Message2 : public MessageBase
{
public:
	Message2(std::string msg, int code)
		: m_message{ msg }
		, m_code{code}
	{}

	std::string message() const
	{
		return m_message;
	}

	std::string m_message;
	int m_code;
};

class HandlerBase {
public:
	virtual ~HandlerBase() = default;

	virtual void onMessage1(const Message1&) {};
	virtual void onMessage2(const Message2&) {};
};

class Handler: public HandlerBase {
public:
	void onMessage1(const Message1& msg) override {
		std::cout << "Processing Message1: " << msg.m_message << std::endl;
	}

	void onMessage2(const Message2& msg) override {
		std::cout << "Processing Message2: " << msg.m_message << " - the code: " << msg.m_code << std::endl;
	}
};

class Worker {
public:

	template <typename T>
	void queue(const T& msg)
	{
		m_queue.emplace_back(&msg);
	}

	void process() {
		for (auto el : m_queue) {
			// Note: order matters here
			if (dynamic_cast<const Message2*>(el)) {
				auto msg = dynamic_cast<const Message2*>(el);
				m_handler.onMessage2(*msg);				
			} else if (dynamic_cast<const Message1*>(el)) {
				auto msg = dynamic_cast<const Message1*>(el);
				m_handler.onMessage1(*msg);
			}
		}
	}

	std::vector<const MessageBase*> m_queue;
	Handler m_handler;
};

int main()
{
	/*
	 * This is a simple polymorphic design.
	 *
	 * Pros:
	 * - Straigh forward to implement and maintain.
	 * 
	 * Cons:
	 * - Not flexible with respect to adding new messages, unless I add something like a generator. 
	 */

	Message1 msg1{ "Hello" };
	std::cout << "Msg1: " << msg1.m_message << std::endl;

	Message2 msg2{ "World", 42 };
	std::cout << "Msg2: " << msg2.m_message << std::endl;;

	Worker worker;
	worker.queue(msg1);
	worker.queue(msg2);
	worker.process();

	return 0;
}
