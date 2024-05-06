
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
	std::cout << "Msg1: " << msg.m_message << std::endl;
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
	std::cout << "Msg2: " << msg.m_message << " - the code: " << msg.m_code << std::endl;
}

std::vector<Envelope> g_queue;

template <typename T>
void queue(T msg)
{
	g_queue.emplace_back(std::move(msg));
}

template <typename T>
auto generate(T f) {
    using f_details = function_traits<std::remove_pointer_t<decltype(f)>>;
	using f_msg = typename f_details::msg_type;

	return [f](Envelope& msg){
		auto m = dynamic_cast<const Envelope::EnvelopedMessage<f_msg>*>(msg.m_msg.get())->m_payload;
		f(m);
	};
}

std::unordered_map<std::type_index, std::function<void(Envelope& msg)>> g_subscriptions;

template <typename T>
void subscribe(T handler) {
    using f_details = function_traits<std::remove_pointer_t<decltype(handler)>>;
	using f_msg = typename f_details::msg_type;

	g_subscriptions[std::type_index(typeid(f_msg))] = generate(handler);
}

int main()
{
	/*
	 * Now that we can queue different types of messages (through type erasure), and generate functions to "recover" 
	 * the orginal type, it is time to combine these. The idea is to have a sort of list of subscriptions for each
	 * kind of event. Such that different actors can handle the same event data.
	 */

	Message1 msg1{ "Hello" };
	queue(msg1);
	subscribe(handleMessage1);

	Message2 msg2{"Hello world", 42};
	queue(msg2);
	subscribe(handleMessage2);

	for (auto& e : g_queue) {
		if (g_subscriptions.contains(e.m_typeIndex)) {
			g_subscriptions[e.m_typeIndex](e);
		}
	}
}
