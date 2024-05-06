#include <iostream>
#include <vector>
#include <memory>

class Envelope
{
public:
	template <typename T>
	Envelope(T&& msg)
		: m_msg(std::make_shared<EnvelopedMessage<T>>(std::forward<T>(msg)))
	{}

	std::string message() const
	{
		return m_msg->message();
	}

	class MessageInterface
	{
	public:
		virtual ~MessageInterface() {}
		virtual std::string message() const = 0;
	};

	template<typename T>
	struct EnvelopedMessage : MessageInterface // note: breaks when I replace struct with class...
	{
	public:
		EnvelopedMessage(const T& msg)
			: m_payload(msg)
		{}

		std::string message() const override
		{
			return m_payload.message();
		}
	// private:
		T m_payload;
	};

	std::shared_ptr<const MessageInterface> m_msg;
};

std::vector<Envelope> g_queue;

template <typename T>
void queue(T msg)
{
	g_queue.emplace_back(std::move(msg));
}

struct Message1
{
	Message1(std::string msg)
		: m_message{ msg }
	{}

	std::string message() const
	{
		return m_message;
	}

	std::string m_message;
};

struct Message2
{
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

int main()
{
	/*
	 * This is an example of type erasure, allowing me to store different messages into a single queue (vector).
	 */
	Message1 msg1{ "Hello" };
	std::cout << "Msg1: " << msg1.m_message << std::endl;
	queue(msg1);

	Message2 msg2{ "World", 42 };
	std::cout << "Msg2: " << msg2.m_message << std::endl;;
	queue(msg2);

	std::cout << "queue size: " << g_queue.size() << std::endl;;
	std::cout << "queue item 1: " << g_queue.front().message() << std::endl;;
	std::cout << "queue item 2: " << g_queue.back().message() << std::endl;;

	return 0;
}
