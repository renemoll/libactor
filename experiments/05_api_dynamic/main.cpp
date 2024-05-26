
#include "envelope.h"
#include "traits.h"

#include <iostream>
#include <functional>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

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

void handleMessage1(Message1& msg) {
	std::cout << "Processing Message1: " << msg.m_message << std::endl;
}

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

void handleMessage2(Message2& msg) {
	std::cout << "Processing Message2: " << msg.m_message << " - the code: " << msg.m_code << std::endl;
}

class Worker {
public:
	template <typename T>
	void queue(T msg)
	{
		m_queue.emplace_back(std::move(msg));
	}

	template <typename T>
	void subscribe(T handler) {
		using f_details = function_traits<std::remove_pointer_t<decltype(handler)>>;
		using f_msg = typename f_details::msg_type;

		m_subscriptions[std::type_index(typeid(f_msg))] = generate(handler);
	}

	void process() {
		for (auto& e : m_queue) {
			if (m_subscriptions.contains(e.m_typeIndex)) {
				m_subscriptions[e.m_typeIndex](e);
			}
		}
	}

private:
	std::vector<Envelope> m_queue;
	std::unordered_map<std::type_index, std::function<void(Envelope& msg)>> m_subscriptions;
	
	template <typename T>
	auto generate(T f) {
		using f_details = function_traits<std::remove_pointer_t<decltype(f)>>;
		using f_msg = typename f_details::msg_type;

		return [f](Envelope& msg){
			auto m = dynamic_cast<const Envelope::EnvelopedMessage<f_msg>*>(msg.m_msg.get())->m_payload;
			f(m);
		};
	}
};

int main()
{
	/*
	 */

	Message1 msg1{ "Hello" };
	std::cout << "Msg1: " << msg1.m_message << std::endl;

	Message2 msg2{ "World", 42 };
	std::cout << "Msg2: " << msg2.m_message << std::endl;;

	Worker worker;
	worker.subscribe(handleMessage1);
	worker.subscribe(handleMessage2);
	worker.queue(msg1);
	worker.queue(msg2);
	worker.process();

	return 0;
}
